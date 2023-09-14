import torch
from torch import nn, Tensor
import torchvision
from utils.constants import MOBILENETV3_SIZE

class MobileNetV3(torch.nn.Module):
	
	def __init__(self, num_classes: int, model_size:MOBILENETV3_SIZE = MOBILENETV3_SIZE.SMALL, pretrained: bool = False, freezed: bool = False):
		super(MobileNetV3, self).__init__()
		
		
		self.num_classes = num_classes
		
		if model_size == MOBILENETV3_SIZE.SMALL:
			torchvision_model = torchvision.models.mobilenet_v3_small(pretrained)
			in_features = 576
			out_features = 1024
		else:
			torchvision_model = torchvision.models.mobilenet_v3_large(pretrained)
			in_features = 960
			out_features = 1280

		if freezed:
			for param in torchvision_model.parameters():
				param.requires_grad = False
				
		self.backbone = nn.Sequential(torchvision_model.features, torchvision_model.avgpool)
		
		self.gesture_classifier = nn.Sequential(
			nn.Linear(in_features=in_features, out_features=out_features),
			nn.Hardswish(),
			nn.Dropout(p=0.2, inplace=True),
			nn.Linear(in_features=out_features, out_features=self.num_classes),
		)
		
	def forward(self, x: Tensor)-> Tensor:
		x = self.backbone(x)
		x = x.view(x.size(0), -1)
		gesture = self.gesture_classifier(x)
		
		return gesture
	

class AlexNet(torch.nn.Module):
	def __init__(self, num_classes: int, pretrained: bool = False, freezed: bool = False, dropout: float = 0.5):
		super(AlexNet, self).__init__()
		
		self.num_classes = num_classes
		
		torchvision_model = torchvision.models.alexnet(pretrained)
		
		if freezed:
			for param in torchvision_model.parameters():
				param.requires_grad = False
				
		self.backbone = torchvision_model.features
		
		self.avgpool = nn.AdaptiveAvgPool2d((6, 6))
		
		self.gesture_classifier = nn.Sequential(
			nn.Dropout(p=dropout),
			nn.Linear(256 * 6 * 6, 4096),
			nn.ReLU(inplace=True),
			nn.Dropout(p=dropout),
			nn.Linear(4096, 4096),
			nn.ReLU(inplace=True),
			nn.Linear(4096, self.num_classes),
		)
		
		self.softmax = nn.Softmax(dim=1)
		
	def forward(self, x:Tensor)->Tensor:
		x = self.backbone(x)
		x = self.avgpool(x)
		x = torch.flatten(x, 1)
		gesture = self.gesture_classifier(x)
		
		return gesture
	
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
  
		# self.features = nn.Sequential(
		# 	nn.Conv2d(3, 32, kernel_size=3, padding=1),
   		# 	nn.ReLU(inplace=True),
   		# 	nn.MaxPool2d(kernel_size=2, stride=2),	
     	# 	nn.Conv2d(32, 32, kernel_size=3, padding=1),
		# 	nn.ReLU(inplace=True),
       	# 	nn.MaxPool2d(kernel_size=2, stride=2)
		# )

		# self.head = nn.Sequential(
		# 	nn.Linear(32*80*60, 128), # 320x240
		# 	nn.ReLU(inplace=True),
		# 	nn.Linear(128, num_classes)
		# )

	def forward(self, x:Tensor)->Tensor:
		x = self.features(x)
		x = torch.flatten(x, 1)
		direction = self.head(x)
		
		return direction