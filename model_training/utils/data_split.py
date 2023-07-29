import os
import shutil
import random

data_type = "blue_pad"
save_directory = "data"
raw_source = os.path.join(save_directory, "raw", data_type)

if __name__ == "__main__":
    
    if not os.path.exists(raw_source):
        raise Exception("Raw data does not exist. Please download data first or specify the correct path")
    
    os.makedirs(save_directory, exist_ok=True)
    
    output_root = os.path.join(save_directory, data_type)
    os.makedirs(output_root, exist_ok=True)
    train_output = os.path.join(output_root, "train")
    test_output = os.path.join(output_root, "test")
    
    for category in os.listdir(raw_source):
        curr_path = os.path.join(raw_source, category)
        # split data into train and test
        train_path = os.path.join(train_output, category)
        test_path = os.path.join(test_output, category)
        os.makedirs(train_path)
        os.makedirs(test_path)
        
        # get all images in the current category
        images = os.listdir(curr_path)
        # split images into train and test
        
        #shuffle images with seed
        random.seed(42)
        random.shuffle(images)
        train_images = images[:int(len(images)*0.8)]
        test_images = images[int(len(images)*0.8):]
        
        #copy images to train and test directories
        for image in train_images:
            shutil.copy(os.path.join(curr_path, image), os.path.join(train_path, image))
        for image in test_images:
            shutil.copy(os.path.join(curr_path, image), os.path.join(test_path, image))
            
        print(f"Finished processing category: {category} with size of {len(images)}")
        