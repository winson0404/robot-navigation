import torch
import torchvision.transforms as transforms
from utils.constants import DATASET_OPERATION
from typing import List, Tuple

class Compose(object):
    def __init__(self, input_size: Tuple[int, int]):
        image_height, image_width = input_size
        self.transform = transforms.Compose([
            transforms.ToPILImage(),
            # transforms.Resize((image_height, image_width)),
            # transforms.Resize((224, 224)),
            # transforms.Grayscale(num_output_channels=3),
            transforms.ToTensor(),
            transforms.Normalize((0.485, 0.456, 0.406), (0.229, 0.224, 0.225))
        ])
    def __call__(self, img):
        transformed_img = self.transform(img)

        return transformed_img
