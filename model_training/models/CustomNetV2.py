import torch
from torch import nn, Tensor
import torchvision

# input size: 320x96 (width x height)
class CustomNet(torch.nn.Module):
	def __init__(self, num_classes:int, dropout:float = 0.5):
		super(CustomNet,self).__init__()
		
		self.num_classes = num_classes
		
		self.features = nn.Sequential(
			nn.Conv2d(3, 16, kernel_size=3, stride=1, padding=1), #grayscale
			nn.ReLU(inplace=True),
			nn.MaxPool2d(kernel_size=2, stride=2),
   
			nn.Conv2d(16, 32, kernel_size=3, stride=1, padding=1),
			nn.ReLU(inplace=True),
			nn.MaxPool2d(kernel_size=2, stride=2),
   
			nn.Conv2d(32, 64, kernel_size=3, stride=1, padding=1),
			nn.ReLU(inplace=True),
			nn.MaxPool2d(kernel_size=2, stride=2),
		)

		self.head = nn.Sequential(
			nn.Dropout(p=dropout, inplace=True),
   			# nn.Linear(64 * 40 * 30, 128), # image size should be 320x240
   			nn.Linear(64 * 40 * 12, 128), # image size should be 320x240
			nn.ReLU(inplace=True),
			nn.Linear(128, 128),
			nn.ReLU(inplace=True),
			nn.Linear(128, num_classes)
		)

	def forward(self, x:Tensor)->Tensor:
		x = self.features(x)
		x = torch.flatten(x, 1)
		direction = self.head(x)
		
		return direction