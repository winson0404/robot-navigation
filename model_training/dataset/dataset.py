import torch
from typing import List, Tuple, Optional
import json
from omegaconf import OmegaConf
import random
import numpy as np
from utils.constants import DATASET_OPERATION
from utils.transform import Compose
from utils.data_processor import DataProcessor
import cv2
import os


class BluePadDataset(torch.utils.data.Dataset):
    def __init__(self, conf:OmegaConf, op:DATASET_OPERATION, transform:Optional[Compose]=None):
        
        # # set seed
        
        self.conf = conf
        random.seed(self.conf.random_seed)
        self.transform = transform
        self.op:DATASET_OPERATION = op
        #load data from data_path
        self.processor = DataProcessor()
        self.labels = {
            label: num for (label, num) in zip(self.conf.dataset.targets, range(len(self.conf.dataset.targets)))
        }
        self.data = self._get_images()

    def __len__(self):
        return len(self.data[0])

    def __getitem__(self, idx):
        images, labels = self.data
        image_o = [cv2.imread(images[idx])]
        label_o = [labels[idx]]
        if self.op == DATASET_OPERATION.TRAIN:
            image_o.append(self.processor.brightness_contrast_adjustment(image_o[0], brightness=30))
            label_o.append(label_o[0])
            image_o.append(self.processor.brightness_contrast_adjustment(image_o[0], brightness=-30))
            label_o.append(label_o[0])
            image_o.append(self.processor.brightness_contrast_adjustment(image_o[0], contrast=50))
            label_o.append(label_o[0])
            image_o.append(self.processor.brightness_contrast_adjustment(image_o[0], contrast=50))
            label_o.append(label_o[0])
            image_o.append(self.processor.noise_injection(image_o[0], mean=0.4, stddev=0.6))
            label_o.append(label_o[0])
        
        if self.transform:
            for(i, image) in enumerate(image_o):
                image_o[i] = self.transform(image)
        
        
        return image_o, label_o
        
    def _get_images(self)->Tuple[List[str], List[int]]:
        """Read Images path and files

        Returns:
            List[str]: _description_
        """
        
        images, labels, = [], []
        for target in self.conf.dataset.targets:
            image_root = os.path.join(self.conf.dataset.dataset_path, DATASET_OPERATION.TRAIN.value if self.op.value != DATASET_OPERATION.TEST.value else DATASET_OPERATION.TEST.value, target)
            for image in os.listdir(os.path.join(self.conf.dataset.dataset_path, DATASET_OPERATION.TRAIN.value if self.op.value != DATASET_OPERATION.TEST.value else DATASET_OPERATION.TEST.value, target)):
                images.append(os.path.join(image_root, image))
                labels.append(self.labels[target])

        # zip and shuffle
        data = list(zip(images, labels))
        random.shuffle(data)
        
        # unzip
        images, labels = zip(*data)
        
        # perform train, test, validation split
        if not self.op == DATASET_OPERATION.TEST:
            if self.op == DATASET_OPERATION.TRAIN:
                images = images[:int(len(images)*0.8)]
                labels = labels[:int(len(labels)*0.8)]
                
            if self.op == DATASET_OPERATION.VALIDATION:
                images = images[int(len(images)*0.8):]
                labels = labels[int(len(labels)*0.8):]
    
        return images, labels
    
    # pad images from 320,240 to 320,320
    def _pad_image(self, image):
        cv2.copyMakeBorder(image, 0, 80, 0, 0, cv2.BORDER_CONSTANT, value=[0,0,0])
        
        
class MixedSurfaceDataset(torch.utils.data.Dataset):
    def __init__(self, conf:OmegaConf, op:DATASET_OPERATION, transform:Optional[Compose]=None):
        
        # # set seed
        
        self.conf = conf
        random.seed(self.conf.random_seed)
        self.transform = transform
        self.op:DATASET_OPERATION = op
        #load data from data_path
        self.processor = DataProcessor()
        self.labels = {
            label: num for (label, num) in zip(self.conf.dataset.targets, range(len(self.conf.dataset.targets)))
        }
        self.data = self._get_images()

    def __len__(self):
        return len(self.data[0])

    def __getitem__(self, idx):
        images, labels = self.data
        image_o = [cv2.imread(images[idx])]
        label_o = [labels[idx]]
        # if self.op == DATASET_OPERATION.TRAIN:
        #     image_o.append(self.processor.brightness_contrast_adjustment(image_o[0], brightness=30))
        #     label_o.append(label_o[0])
        #     image_o.append(self.processor.brightness_contrast_adjustment(image_o[0], brightness=-30))
        #     label_o.append(label_o[0])
        #     image_o.append(self.processor.brightness_contrast_adjustment(image_o[0], contrast=50))
        #     label_o.append(label_o[0])
        #     image_o.append(self.processor.brightness_contrast_adjustment(image_o[0], contrast=50))
        #     label_o.append(label_o[0])
        #     image_o.append(self.processor.noise_injection(image_o[0], mean=0.4, stddev=0.6))
        #     label_o.append(label_o[0])
        
        if self.transform:
            for(i, image) in enumerate(image_o):
                image = self.processor.crop_roi(image, 0, 240*0.6, 320, 240*0.4)
                # breakpoint()
                image_o[i] = self.transform(image)
        
        
        return image_o, label_o
        
    def _get_images(self)->Tuple[List[str], List[int]]:
        """Read Images path and files

        Returns:
            List[str]: _description_
        """
        
        images, labels, = [], []
        for target in self.conf.dataset.targets:
            image_root = os.path.join(self.conf.dataset.dataset_path, DATASET_OPERATION.TRAIN.value if self.op.value != DATASET_OPERATION.TEST.value else DATASET_OPERATION.TEST.value, target)
            for image in os.listdir(os.path.join(self.conf.dataset.dataset_path, DATASET_OPERATION.TRAIN.value if self.op.value != DATASET_OPERATION.TEST.value else DATASET_OPERATION.TEST.value, target)):
                images.append(os.path.join(image_root, image))
                labels.append(self.labels[target])

        # zip and shuffle
        data = list(zip(images, labels))
        random.shuffle(data)
        
        # unzip
        images, labels = zip(*data)
        
        # perform train, test, validation split
        if not self.op == DATASET_OPERATION.TEST:
            if self.op == DATASET_OPERATION.TRAIN:
                images = images[:int(len(images)*0.8)]
                labels = labels[:int(len(labels)*0.8)]
                
            if self.op == DATASET_OPERATION.VALIDATION:
                images = images[int(len(images)*0.8):]
                labels = labels[int(len(labels)*0.8):]
    
        return images, labels
    
    # pad images from 320,240 to 320,320
    def _pad_image(self, image):
        cv2.copyMakeBorder(image, 0, 80, 0, 0, cv2.BORDER_CONSTANT, value=[0,0,0])