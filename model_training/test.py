import torch
import torch.nn as nn
import torch.optim as optim
import torch.utils.data as data
import torchvision.transforms as transforms
from torchvision.datasets import CIFAR10
from ax import optimize
from ax.utils.notebook.plotting import render

# Define the PyTorch model with flexible hidden layer combinations
class FlexibleCNN(nn.Module):
    def __init__(self, in_channels, num_classes, hidden_dims):
        super(FlexibleCNN, self).__init__()
        layers = []
        prev_channels = in_channels

        for hidden_dim in hidden_dims:
            layers.append(nn.Conv2d(prev_channels, hidden_dim, kernel_size=3, padding=1))
            layers.append(nn.ReLU())
            layers.append(nn.MaxPool2d(kernel_size=2))
            prev_channels = hidden_dim

        self.conv_layers = nn.Sequential(*layers)
        self.fc = nn.Linear(prev_channels * 4 * 4, num_classes)

    def forward(self, x):
        x = self.conv_layers(x)
        x = x.view(x.size(0), -1)
        x = self.fc(x)
        return x

# ... Rest of the code remains the same ...

# Define the training function
def train_model(params):
    lr = params.get('lr', 0.01)
    hidden_dims = params.get('hidden_dims', [32])

    model = FlexibleCNN(in_channels=3, num_classes=10, hidden_dims=hidden_dims)
    criterion = nn.CrossEntropyLoss()
    optimizer = optim.SGD(model.parameters(), lr=lr)

    num_epochs = 5
    for epoch in range(num_epochs):
        for inputs, targets in train_loader:
            outputs = model(inputs)
            loss = criterion(outputs, targets)

            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

    # Return negative accuracy as Ax minimizes the objective
    accuracy = evaluate_model(model)
    return {"accuracy": -accuracy}

# ... Rest of the code remains the same ...

# Define the search space for hyperparameters and architecture
search_space = [
    {"name": "lr", "type": "range", "bounds": [1e-4, 1e-2], "log_scale": True},
    {"name": "hidden_dims", "type": "choice", "values": [[32], [64], [32, 32], [64, 64]]},
]

# ... Rest of the code remains the same ...
