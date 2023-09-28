import os
import shutil

category = ["no_objects", "left", "middle", "right", "all" ,"left_mid", "right_mid"]


if __name__ == "__main__":
    dataset_root = "dataset"
    output_root = "output"
    test_name = "pine_wood"
    
    for table in os.listdir(dataset_root):
        table_path = os.path.join(dataset_root, table)
        if table != test_name: # create as train set
            for category_name in category:
                category_path = os.path.join(table_path, category_name)
                for image_name in os.listdir(category_path):
                    new_name = image_name.split(".")[0] + "_" + table + ".png"
                    image_path = os.path.join(category_path, image_name)
                    output_path = os.path.join(output_root, "train", category_name)
                    os.makedirs(output_path, exist_ok=True)
                    os.path.join(output_path, new_name)
                    output_path = os.path.join(output_path, new_name)
                    shutil.copy(image_path, output_path)
                    # os.rename(image_path, output_path)
                    print("Moved", image_path, "to", output_path)
        else: # create as test set
            for category_name in category:
                category_path = os.path.join(table_path, category_name)
                new_name = image_name.split(".")[0] + "_" + table + ".png"
                for image_name in os.listdir(category_path):
                    image_path = os.path.join(category_path, image_name)
                    output_path = os.path.join(output_root, "test", category_name)
                    os.makedirs(output_path, exist_ok=True)
                    output_path = os.path.join(output_path, new_name)
                    shutil.copy(image_path, output_path)
                    # os.rename(image_path, output_path)
                    print("Moved", image_path, "to", output_path)