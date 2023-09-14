import torch
from torch import nn, Tensor
import numpy as np
import cv2
import random
from models.classification import MobileNetV3, AlexNet, CustomNet
from utils.constants import MOBILENETV3_SIZE
import os
from typing import Dict, Tuple, List
from omegaconf import OmegaConf
from torchmetrics.functional import accuracy, auroc, confusion_matrix, f1_score, precision, recall
import torch.onnx


def get_metrics(conf:OmegaConf, preds: Tensor, labels: Tensor) -> Dict:
    average = conf.metric_params["average"]
    metrics = conf.metric_params["metrics"]
    num_classes = len(conf.dataset.targets)
    
    # preds = preds.cpu().detach().numpy()
    # labels = labels.cpu().detach().numpy()
    
    scores = {
        "accuracy": accuracy(preds, labels, task="multiclass", average=average, num_classes=num_classes),
        "f1_score": f1_score(preds, labels, task="multiclass", average=average, num_classes=num_classes),
        "precision": precision(preds, labels, task="multiclass", average=average, num_classes=num_classes),
        "recall": recall(preds, labels, task="multiclass", average=average, num_classes=num_classes),
    }
    
    needed_scores = {}    
    for metric in metrics:
        needed_scores[metric] = round(float(scores[metric]), 6)
        
    return needed_scores

def full_frame_preprocess(im, new_shape=(320, 320), color=(114, 114, 114), auto=True, scaleup=True, stride=32):
    # Resize and pad image while meeting stride-multiple constraints
    shape = im.shape[:2]  # current shape [height, width]
    if isinstance(new_shape, int):
        new_shape = (new_shape, new_shape)

    # Scale ratio (new / old)
    r = min(new_shape[0] / shape[0], new_shape[1] / shape[1])
    if not scaleup:  # only scale down, do not scale up (for better val mAP)
        r = min(r, 1.0)

    # Compute padding
    new_unpad = int(round(shape[1] * r)), int(round(shape[0] * r))
    dw, dh = new_shape[1] - new_unpad[0], new_shape[0] - new_unpad[1]  # wh padding

    if auto:  # minimum rectangle
        dw, dh = np.mod(dw, stride), np.mod(dh, stride)  # wh padding

    dw /= 2  # divide padding into 2 sides
    dh /= 2

    if shape[::-1] != new_unpad:  # resize
        im = cv2.resize(im, new_unpad, interpolation=cv2.INTER_LINEAR)
    top, bottom = int(round(dh - 0.1)), int(round(dh + 0.1))
    left, right = int(round(dw - 0.1)), int(round(dw + 0.1))
    im = cv2.copyMakeBorder(im, top, bottom, left, right, cv2.BORDER_CONSTANT, value=color)  # add border
    im = im.transpose((2, 0, 1))
    im = np.expand_dims(im, 0)
    im = np.ascontiguousarray(im)
    
    im = im.astype(np.float32)
    
    return im, r, (dw, dh)

def full_frame_postprocess(model_output, ratio, dwdh, threshold):
    box = []
    score = -1
    # breakpoint()
    for i,(batch_id,x0,y0,x1,y1,_,score) in enumerate(model_output):
        if score < threshold:
            continue
        if batch_id >= 6:
            break
        # breakpoint()
        box = np.array([x0,y0,x1,y1])
        box -= np.array(dwdh*2)
        box /= ratio
        box = box.round().astype(np.int32).tolist()
        
        score = round(float(score),3)
    return box, score



def draw_image(image, box, score, text):
        color = (0,255,0)
        cv2.rectangle(image,box[:2],box[2:],color,2)
        cv2.putText(image,f"{text} {score}",(box[0], box[1] - 2),cv2.FONT_HERSHEY_SIMPLEX,0.75,[225, 255, 255],thickness=2)


def crop_roi_image(img, bbox, target_size):
        width, height = img.shape[1], img.shape[0]
        
        # get the actual bbox
        x1, y1, x2, y2 = bbox
        

        crop_image = img[y1:y2, x1:x2]
        
        # scale back to input size and pad
        
        # get dimension
        cropped_height, cropped_width, _ = crop_image.shape
        target_width, target_height = target_size
        
        # choose scaling factor based on the longest height/width
        side = max(cropped_height, cropped_width)
        scale = (target_width if target_width >= target_height else target_height) / side
        
        
        # calculate new dimension and resize
        resized_width, resized_height = int(cropped_width * scale), int(cropped_height * scale)
        resized_image = cv2.resize(crop_image, (resized_width, resized_height))
        
        # pad the cropped gesture image to the input size
        padded_image = np.zeros((target_height, target_width, 3), dtype=np.uint8)
        padded_image[:] = (0, 0, 0)
        
        # Calculate the padding required
        pad_top = (target_height - resized_height) // 2
        pad_bottom = target_height - resized_height - pad_top
        pad_left = (target_width - resized_width) // 2
        pad_right = target_width - resized_width - pad_left

        try:
            image_resized = cv2.copyMakeBorder(resized_image, pad_top, pad_bottom, pad_left, pad_right, cv2.BORDER_CONSTANT, value=(0, 0, 0))
        except:
            print("Error!")
            breakpoint()
            
        image_resized = image_resized.astype(np.float32)
        #resize to (1, 3, 224, 224)
        image_resized = image_resized.transpose((2, 0, 1))
        image_resized = np.expand_dims(image_resized, 0)
        image_resized = np.ascontiguousarray(image_resized)
        return image_resized

def set_random_state(seed: int)-> None:
    """
    Set random seed for torch, numpy, random

    Parameters
    ----------
    random_seed: int
        Random seed from config
    """
    torch.manual_seed(seed)
    np.random.seed(seed)
    random.seed(seed)
    
    
def build_model(
    model_name: str,
    num_classes: int,
    device: str,
    checkpoint: str = None,
    pretrained: bool = False,
    freezed: bool = False,
    conf:OmegaConf = None
) -> nn.Module:
    """
    Build modela and load checkpoint

    Parameters
    ----------
    model_name : str
        Model name e.g. ResNet18, MobileNetV3_small, Vitb32
    num_classes : int
        Num classes for each task
    checkpoint : str
        Path to model checkpoint
    device : str
        Cpu or CUDA device
    pretrained : false
        Use pretrained model
    freezed : false
        Freeze model layers
    """
    models = {
        "MobileNetV3_large": MobileNetV3(
            num_classes=num_classes, model_size=MOBILENETV3_SIZE.LARGE, pretrained=pretrained, freezed=freezed
        ),
        "MobileNetV3_small": MobileNetV3(
            num_classes=num_classes, model_size=MOBILENETV3_SIZE.SMALL, pretrained=pretrained, freezed=freezed
        ),
        "AlexNet": AlexNet(num_classes=num_classes, pretrained=pretrained, freezed=freezed, dropout=conf.train_params.dropout),
        "CustomNet": CustomNet(num_classes=num_classes, dropout=conf.train_params.dropout),
        "CustomNetV2": CustomNet(num_classes=num_classes, dropout=conf.train_params.dropout),
    }

    model = models[model_name]

    if checkpoint is not None:
        # checkpoint = os.path.expanduser(checkpoint)
        if os.path.exists(checkpoint):
            checkpoint = torch.load(checkpoint, map_location=torch.device(device))["state_dict"]
            model.load_state_dict(checkpoint)

    model.to(device)
    return model


def save_checkpoint(
    output_dir: str, config_dict: Dict, model: nn.Module, optimizer: torch.optim.Optimizer, epoch: int, name: str
) -> None:
    """
    Save checkpoint dictionary

    Parameters
    ----------
    output_dir : str
        Path to directory model checkpoint
    config_dict : Dict
        Config dictionary
    model : nn.Module
        Model for checkpoint save
    optimizer : torch.optim.Optimizer
        Optimizer
    epoch : int
        Epoch number
    name : str
        Model name
    """
    if not os.path.exists(output_dir):
        os.makedirs(os.path.join(output_dir), exist_ok=True)

    # .pth is something like a zip file for pickle (hence can include additional informations)
    checkpoint_path = os.path.join(output_dir, f"{name}.pth")

    checkpoint_dict = {
        "state_dict": model.state_dict(),
        "optimizer_state_dict": optimizer.state_dict(),
        "epoch": epoch,
        "config": config_dict,
    }
    torch.save(checkpoint_dict, checkpoint_path)
    
