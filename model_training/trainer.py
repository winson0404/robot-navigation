import torch
from torch.utils.data import DataLoader
from utils.util import set_random_state, build_model, save_checkpoint, get_metrics
from utils.transform import Compose
from utils.constants import DATASET_OPERATION
from utils.logger import Logger
from utils.convert_onnx import save_onnx
from dataset.dataset import MixedSurfaceDataset
import logging
from omegaconf import OmegaConf
from tqdm import tqdm
import math
import os
import csv

class ClassificationTrainer:
    def __init__(self, conf:OmegaConf, output_path:str, logger:Logger = None) -> None:
        # class parameters
        self.conf = conf
        self.output_path = output_path
        self.logger = logger
        self._initialize()
        
    def _initialize(self):
        # sequence matter
        
        # training parameters
        set_random_state(self.conf.random_seed)
        self.num_classes = len(self.conf.dataset.targets)
        self.epochs = self.conf.train_params.epochs
        self.log_writter = None
        self.model = self._initiate_model()
        self.optimizer = self._get_optimizer()
        # breakpoint()

    def _initiate_model(self):
        set_random_state(self.conf.random_seed)

        return build_model(
            model_name=self.conf.model.name,
            num_classes=self.num_classes,
            checkpoint=self.conf.model.get("checkpoint", None),
            device=self.conf.device,
            pretrained=self.conf.model.pretrained,
            freezed=self.conf.model.freezed,
            conf=self.conf
        )
        
    def _get_optimizer(self):
        
        trainable_params = [p for p in self.model.parameters() if p.requires_grad]
        
        if self.conf.optimizer.optimizer == "adam":
            optimizer = torch.optim.Adam(trainable_params, lr=self.conf.optimizer.lr, eps=1e-7, betas=(0.9, 0.999))
        elif self.conf.optimizer.optimizer == "sgd":
            optimizer = torch.optim.SGD(trainable_params, lr=self.conf.optimizer.lr, momentum=self.conf.optimizer.momentum)
        elif self.conf.optimizer.optimizer == "rmsprop":
            optimizer = torch.optim.RMSprop(trainable_params, lr=self.conf.optimizer.lr, alpha=0.97, eps=1e-6)
        
        return optimizer
    
    def _run_epoch(self, device:str, current_epoch:int, train_loader:MixedSurfaceDataset, lr_scheduler_warmup: torch.optim.lr_scheduler.LinearLR=None)->None:
        
        # set to train mode
        self.model.train()
        
        # initialize loss function
        criterion = torch.nn.CrossEntropyLoss()
        
        # run based on batch
        log_loss = 0
        with tqdm(train_loader, unit="batch") as batch_loader:
            for i, (images, labels) in enumerate(batch_loader):
                
                # shape to (batch_size, 3, 244, 244)
                # images = torch.stack(list(image.to(device) for image in images))
                images = torch.cat([image.to(device) for image in images], dim=0)
                
                # calculate overall step currently
                step = i + len(train_loader) * current_epoch
                
                # go through the network and get output
                # breakpoint()
                output = self.model(images)

                # breakpoint()
                #convert labels to tensor
                # breakpoint()
                labels = torch.tensor(labels[0], dtype=torch.long, device=device)
                loss = criterion(output, labels)
            
                loss_value = loss.item()
                log_loss = loss_value
                


                if not math.isfinite(loss_value):
                    logging.info("Loss is {}, stopping training".format(loss_value))
                    exit(1)
                    
                
                self.optimizer.zero_grad()
                loss.backward()
                self.optimizer.step()
                
                if lr_scheduler_warmup is not None:
                    lr_scheduler_warmup.step()
        if self.logger is not None:
            self.logger.log("train", {"loss": loss_value, "epoch": current_epoch})

    def _run_eval(self, device:str, current_epoch:int, validation_loader:MixedSurfaceDataset, mode:str="valid")->None:
        f1_score = None
        
        # theres a chance that user dont wanna do validation
        if validation_loader is not None:
            # stop gradient update
            with torch.no_grad():
                # set to non training mode:
                self.model.eval()
                predicts, targets = [], []
                with tqdm(validation_loader, unit="batch") as tepoch:
                    tepoch.set_description(f"{mode} Epoch {current_epoch}")
                    for i, (images, labels) in enumerate(tepoch):
                        images = torch.cat([image.to(device) for image in images], dim=0)
                        labels = torch.cat([label for label in labels], dim=0)
                        # images = torch.stack(list(image.to(self.conf.device) for image in images))
                        output = self.model(images)
                        predicts.extend(output.argmax(dim=1).cpu().numpy())
                        targets.extend(labels.numpy())
                    
                    predicts = torch.tensor(predicts, dtype=torch.long, device=self.conf.device)
                    targets = torch.tensor(targets, dtype=torch.long, device=self.conf.device)
                    
                    metric = get_metrics(self.conf, predicts, targets)
                    f1_score = metric["f1_score"]
                    
                    if self.logger is not None:
                        self.logger.log("validation", metric)
                    
        
        return f1_score
                    
    
    def run_train(self)->None:
        
        model = self._initiate_model()
        
        transform = Compose(input_size=self.conf.dataset.image_size)
        
        train_dataset = MixedSurfaceDataset(
            self.conf,
            op=DATASET_OPERATION.TRAIN,
            transform=transform
        )
        
        validation_dataset = MixedSurfaceDataset(
            self.conf,
            op=DATASET_OPERATION.VALIDATION,
            transform=transform
        )
        
        train_loader = DataLoader(train_dataset, batch_size=self.conf.train_params.train_batch_size, shuffle=False)
        validation_loader = DataLoader(validation_dataset, batch_size=self.conf.train_params.validation_batch_size, shuffle=False)
        
        best_metric = -1
        conf_dictionary = OmegaConf.to_container(self.conf)
        for i in range(self.epochs):
            logging.info(f"Epoch {i+1}/{self.epochs}")
            
            warmup_iters = min(1000, len(train_loader) - 1)
            lr_scheduler_warmup = torch.optim.lr_scheduler.LinearLR(
                self.optimizer, start_factor=self.conf.scheduler.start_factor, total_iters=warmup_iters
            )
            self._run_epoch(device=self.conf.device, current_epoch=i, train_loader=train_loader, lr_scheduler_warmup=lr_scheduler_warmup)
            metric_value = self._run_eval(device=self.conf.device, current_epoch=i, validation_loader=validation_loader)
            
            # early stopping
            if metric_value - best_metric > self.conf.train_params.early_stopping:
                break
            
            # get checkpoint based on f1 score
            if metric_value > best_metric:
                best_metric = metric_value
                self.save_model(self.output_path, name="best_model", mode="onnx")
                # save_checkpoint(self.output_path, conf_dictionary, self.model, self.optimizer, i, "best_model")

        return model
    
    def run_test(self)->None:
        train_dataset = MixedSurfaceDataset(
            self.conf,
            op=DATASET_OPERATION.TEST,
            transform=Compose(input_size=self.conf.dataset.image_size)
        )
        with torch.no_grad():
                # set to non training mode:
            self.model.eval()
            test_loader = DataLoader(train_dataset, batch_size=self.conf.train_params.validation_batch_size, shuffle=False)
            
            predicts, targets, probs = [], [], []
            
            log_image = []
            
            with tqdm(test_loader, unit="batch") as batch_loader:
                for i, (images, labels) in enumerate(batch_loader):
                    images = torch.cat([image.to(self.conf.device) for image in images], dim=0)
                    labels = torch.cat([label for label in labels], dim=0)
                    log_image.extend(images)
                    output = self.model(images)
                    
                    probs.extend(output)
                    predicts.extend(output.argmax(dim=1).cpu().numpy())
                    targets.extend(labels.numpy())
        
            # get metrics i dont know why i did this but its working so not changing now
            predicts = torch.tensor(predicts, dtype=torch.long, device=self.conf.device)
            targets = torch.tensor(targets, dtype=torch.long, device=self.conf.device)
            
            metric = get_metrics(self.conf, predicts, targets)
            f1_score = metric["f1_score"]
            
            # save metrics to csv
            fields = list(metric.keys())
            with open(os.path.join(self.output_path, "test_metrics.csv"), "w") as csvfile:
                csvwriter = csv.writer(csvfile)
                csvwriter.writerow(fields)
                csvwriter.writerow(list(metric.values()))
            
            
            if self.logger is not None:
                
                self.logger.log("test", metric)
                self.logger.log_image_table(log_image, predicts, targets, probs)
        
    

    def save_model(self, path:str, name:str="model", mode:str="torch"):
        if mode == "torch":
            torch.save(self.model.state_dict(), os.path.join(path, name+".pth"))
        elif mode == "onnx":
            # breakpoint()
            save_onnx(self.model, path, name, input_shape=(3, self.conf.dataset.image_size[0],self.conf.dataset.image_size[1]))
            
        logging.info(f"Model saved to {os.path.join(path, name)}")