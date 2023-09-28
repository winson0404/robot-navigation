import torch
import torch.nn as nn
import os
from typing import Tuple
from omegaconf import OmegaConf
from utils.constants import MOBILENETV3_SIZE
from utils.transform import Compose
from models.classification import MobileNetV3

def save_onnx(model: nn.Module, output_dir: str, name: str, input_shape: Tuple[int, int, int] = (3, 224, 224)) -> None:
    """
    Save model to onnx format

    Parameters
    ----------
    model : nn.Module
        Model for checkpoint save
    output_dir : str
        Path to directory model checkpoint
    name : str
        Model name
    input_shape : Tuple[int, int, int]
        Input shape for model
    """

    onnx_path = os.path.join(output_dir, f"{name}.onnx")
    dummy_input = torch.randn(1, *input_shape)
    transform = Compose(input_shape[1:])
    # breakpoint()
    dummy_input = transform(dummy_input[0])
    # change to cuda both model and dummy_input to cpu
    model = model.to("cpu")
    dummy_input = dummy_input.to("cpu")
    # add 1 dimension
    dummy_input = dummy_input.unsqueeze(0)
    # Export the model to onnx
    torch.onnx.export(
        model, 
        dummy_input, 
        onnx_path, 
        export_params=True,
        opset_version=11,
        input_names=["images"],
        output_names=["output"])
    
    model = model.to("cuda")
    print(f"Model saved to {onnx_path}")


if __name__ == "__main__":
    # print("Hello world")
    path = r"output\HaGRID_Test\with_scheduler\best_model.pth"
    #load config
    conf = OmegaConf.load("configs/default.yaml")

    model = MobileNetV3(
            num_classes=len(conf.dataset.targets), 
            model_size=MOBILENETV3_SIZE.SMALL, 
            pretrained=False, 
            freezed=False)
    # logging.basicConfig(format="[LINE:%(lineno)d] %(levelname)-8s [%(asctime)s]  %(message)s", level=logging.INFO)

    # transform = Compose()
    # # set up dataset
    # train_dataset = ClassificationHaGridDataset(
    #         conf,
    #         op=DATASET_OPERATION.TRAIN,
    #         transform=transform
    #     )
    # # train_dataset = ClassificationHaGridDataset(conf, DATASET_OPERATION.TRAIN, Compose(conf, DATASET_OPERATION.TRAIN))
    # train_loader = DataLoader(train_dataset, batch_size=conf.train_params.train_batch_size, shuffle=False)
    # breakpoint()
    #load model
    state_dict = torch.load(path)["state_dict"]
    # breakpoint()
    model.load_state_dict(state_dict)
    # for i, (img, label) in enumerate(train_loader):
    #     output = model(img)
    #     breakpoint()
    
    save_path = r"output\HaGRID_Test\with_scheduler\\"
    save_onnx(model, save_path, "gesture_model", input_shape=(3, conf.dataset.input_size[0], conf.dataset.input_size[1]))