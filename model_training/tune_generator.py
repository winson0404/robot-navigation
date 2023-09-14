import random
from omegaconf import OmegaConf
import os

setting = {
    "lr": [0.005, 0.001, 0.0005, 0.0001],
    "momentum": [0.9, 0.85, 0.8],
    "weight_decay": [0.0001, 0.0005, 0.001],
    "batch_size": [16, 32],
    "optimizer": ["sgd", "adam", "rmsprop"],
    "epochs": [80, 100, 120],
    "dropout": [0.2, 0.3, 0.4, 0.5],
}

random_names = ["one", "two", "three", "four", "five", "six", "seven", "eight", "nine", "ten"]

variation_numbers = 10

# choose 3 different setting randomly
# random_seed = 42

config_root = "configs/CustomNetV2"

conf = OmegaConf.load("configs/CustomNetV2/default.yaml")
project_name = "CustomNetV2"
image_width = 320
image_height = 96
os.makedirs(os.path.join(config_root, project_name), exist_ok=True)
for i in range(variation_numbers):
    conf.project_name = project_name
    
    # random settings
    conf.model.name = project_name
    conf.dataset.image_size = [image_width, image_height]
    conf.optimizer.optimizer = random.choice(setting["optimizer"])
    conf.optimizer.lr = random.choice(setting["lr"])
    conf.optimizer.momentum = random.choice(setting["momentum"])
    conf.optimizer.weight_decay = random.choice(setting["weight_decay"])
    conf.train_params.epochs = random.choice(setting["epochs"])
    conf.train_params.train_batch_size = random.choice(setting["batch_size"])
    conf.train_params.validation_batch_size = conf.train_params.train_batch_size
    conf.train_params.dropout = random.choice(setting["dropout"])
    experiment_name = random.choice(random_names) + "_" + conf.optimizer.optimizer + "_" + str(conf.train_params.epochs)   
    conf.experiment_name = experiment_name
    
    # save config to config_root
    save_path = os.path.join(config_root, project_name, experiment_name + ".yaml")
    OmegaConf.save(conf, save_path)