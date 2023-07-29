import wandb
from omegaconf import OmegaConf
from typing import Dict, List, Tuple
import torchvision.transforms as transforms
import cv2

def inverse_normalize(tensor, mean, std):
    de_norm = transforms.Normalize(
        mean=[-m / s for m, s in zip(mean, std)],
        std=[1.0 / s for s in std]
    )

    return de_norm(tensor)


class Logger:
    def __init__(self, project_name:str, experiment_name:str, conf:OmegaConf) -> None:
        self.conf = conf
        self.project_name = project_name
        self.experiment_name = experiment_name
        self.wandb = wandb
        self.target_dict = {i: label for i, label in enumerate(self.conf.dataset.targets)}
        self.init()

    def init(self):
        if not self.is_logged_in():
            self.wandb.login()
        
        self.wandb.init(
            project=self.project_name, 
            name=self.experiment_name, 
            config={
                "model": self.conf.model.name,
                "epochs": self.conf.train_params.epochs,
                "train_batch_size": self.conf.train_params.train_batch_size,
                "validation_batch_size": self.conf.train_params.validation_batch_size,
                "optimizer": self.conf.optimizer.optimizer,
                "learning_rate": self.conf.optimizer.lr,
                "momentum": self.conf.optimizer.momentum if self.conf.optimizer.optimizer != "adam" else None,
                "weight_decay": self.conf.optimizer.weight_decay
            })

    def is_logged_in(self):
        return wandb.api.api_key is not None
    
    def log(self, category:str, metrics:Dict):
        new_dict = {}
        for key, value in metrics.items():
            new_dict[f"{category}/{key}"] = value
        self.wandb.log(new_dict)

    def log_image_table(self, images, predicted, labels, probs):
        "Log a wandb.Table with (img, pred, target, scores)"
        # Create a wandb Table to log images, labels and predictions to
        table = self.wandb.Table(columns=["image", "pred", "target"]+[f"score_{self.target_dict[i]}" for i in range(len(probs[0]))])
        for img, pred, targ, prob in zip(images, predicted.to("cpu"), labels.to("cpu"), probs):
            # Define the mean and standard deviation
            mean = (0.485, 0.456, 0.406)
            std = (0.229, 0.224, 0.225)
            # Assuming `normalized_image` is the normalized tensor
            # original_image= inverse_normalize(tensor=img, mean=(0.485, 0.456, 0.406), std=(0.229, 0.224, 0.225))

            original_image = img.permute(1, 2, 0)
            # breakpoint()
            #show image
            # cv2.imshow("image", original_image.cpu().numpy()*255)
            table.add_data(self.wandb.Image(original_image.cpu().numpy()), self.target_dict[pred.item()], self.target_dict[targ.item()], *prob.cpu().numpy())
        self.wandb.log({"predictions_table":table})
    
    def finish(self):
        self.wandb.finish()