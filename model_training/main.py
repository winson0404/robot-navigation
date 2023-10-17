import torch
import cv2
import onnxruntime as ort
import os
import numpy as np
from utils.util import full_frame_preprocess, full_frame_postprocess, crop_roi_image, draw_image
from omegaconf import OmegaConf
from utils.transform import Compose
from utils.data_processor import DataProcessor
import socket
import time

# hand_model_path = "../data/models/YoloV7_Tiny.onnx"
model_path = r"output/CustomNetV2/two_adam_80_default/best_model.onnx"
provider = ['CPUExecutionProvider']
threshold = 0.5
conf = OmegaConf.load("output/CustomNetV2/two_adam_80_default/default.yaml")
input_shape=(3, conf.dataset.image_size[0], conf.dataset.image_size[1])
target_dict = {i: label for i, label in enumerate(conf.dataset.targets)}

def draw_rectangle(frame, x, y, w, h):
    x = int(x)
    y = int(y)
    w = int(w)
    h = int(h)
    cv2.rectangle(frame, (x,y), (x+w, y+h), (255,0,0), 2)
    return frame

def post_process(frame, prediction):
    frame = draw_rectangle(frame, 0, 240*0.6, 320, 240*0.4)
    if prediction == "no_objects":
        return frame
    elif prediction == "left":
        frame = draw_rectangle(frame, 10, 240*0.6+5, 100, 240*0.4-10)
    elif prediction == "left_mid":
        frame = draw_rectangle(frame, 10, 240*0.6+5, 200, 240*0.4-10)
    elif prediction == "middle":
        frame = draw_rectangle(frame, 110, 240*0.6+5, 100, 240*0.4-10)
    elif prediction == "right":
        frame = draw_rectangle(frame, 210, 240*0.6+5, 100, 240*0.4-10)
    elif prediction == "right_mid":
        frame = draw_rectangle(frame, 110, 240*0.6+5, 200, 240*0.4-10)
    elif prediction == "all":
        frame = draw_rectangle(frame, 10, 240*0.6+5, 300, 240*0.4-10)
        
    return frame

class TCP_Client():
    def __init__(self, host, port:int) -> None:
        try:
            # Create a TCP/IP socket
            self.client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

            # Connect to the server
            self.client_socket.connect((host, port))
        except Exception as e:
            print("Error:", e)

        # finally:
        #     # Close the socket
        #     self.client_socket.close()
            
    def Send(self, tag:str, prediction:int):
        tag = (0).to_bytes(1, byteorder='big')
        items = 1
        item_length = 1
        item = int(prediction)
        message =  items.to_bytes(1, byteorder='big') + item_length.to_bytes(1, byteorder='big') + item.to_bytes(1, byteorder='big')
        message_length = len(message)
        data = tag + message_length.to_bytes(1, byteorder='big') + message
        
        self.client_socket.send(data)
        
    
    def Close(self,):
        self.client_socket.close()

            
if __name__ == "__main__":
    
    camera_name = "/dev/video2"
    cap = cv2.VideoCapture(camera_name)
    inf_session = ort.InferenceSession(model_path, providers=provider)
    model_out_name = [i.name for i in inf_session.get_outputs()]
    transform = Compose(input_shape[1:])
    client = TCP_Client('127.0.0.1', 12345)
    processor = DataProcessor()
        
    while cap.isOpened():
        ret, frame = cap.read()
        
        try:
            if ret:
                # image, ratio, dwdh = full_frame_preprocess(frame, auto=False)
                # breakpoint()
                #convert from tensor to numpy
                # np_image = transform(frame).unsqueeze(0).numpy()
                # resize frame 
                frame = cv2.resize(frame, (320, 240))
                roi = processor.crop_roi(frame, 0, 240*0.6, 320, 240*0.4)
                # roi = cv2.resize(roi, (96, 320))
                out = inf_session.run(model_out_name, {'images': transform(roi).unsqueeze(0).numpy()})[0]
                # bbox, score = full_frame_postprocess(out, ratio, dwdh, threshold)
                # if score >= threshold:
                #     cropped_image = crop_roi_image(frame, bbox, (224, 224))
                #     out = model_path.run(gesture_out_name, {'images': cropped_image/255})[0]
                #     gesture = target_dict[out.argmax()]
                #     draw_image(frame, bbox, score, gesture)
                client.Send('0', out.argmax())
                direction = target_dict[out.argmax()]
                print(direction)
                # print(direction)
                # breakpoint()
                frame = post_process(frame, direction)
                frame = cv2.resize(frame, (640, 480))
                cv2.imshow(camera_name, frame)
                # time.sleep(2)
        except Exception as e:
            print(e)
            # breakpoint()
            exit()
            
            
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break