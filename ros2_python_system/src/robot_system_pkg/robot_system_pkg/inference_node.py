import torch
import cv2
import onnxruntime as ort
import os
import numpy as np
from .utils.helper_model import full_frame_preprocess, full_frame_postprocess, crop_roi_image, draw_image
from omegaconf import OmegaConf
from .utils.helper_transform import Compose
from .utils.helper_data_processor import DataProcessor
import socket
import time

import rclpy
from rclpy.node import Node


# hand_model_path = "../data/models/YoloV7_Tiny.onnx"
model_path = r"output/CustomNetV2/two_adam_80_default/best_model.onnx"
threshold = 0.5
fps = 15

provider = ['CPUExecutionProvider']
conf_path = "output/CustomNetV2/two_adam_80_default/default.yaml"
camera_name = "/dev/video2"

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



class InferenceNode(Node):
    def __init__(self):
        super().__init__('comm_node')
        self.conf = OmegaConf.load(conf_path)
        self.input_shape = (3, self.conf.dataset.image_size[0], self.conf.dataset.image_size[1])
        self.target_dict = {i: label for i, label in enumerate(self.conf.dataset.targets)}
        
        self.cap = cv2.VideoCapture(camera_name)
        self.inf_session = ort.InferenceSession(model_path, providers=provider)
        self.model_out_name = [i.name for i in self.inf_session.get_outputs()]
        self.transform = Compose(self.input_shape[1:])
        self.processor = DataProcessor()
        
        
        self.inference_output_publisher = self.create_publisher(InferenceResult, 'inference_result', 10)
        self.create_time(1/fps, self.inference_callback)
        
        
        
    def inference_callback(self):
        if self.cap.isOpened():
            ret, frame = self.cap.read()
            try:
                if ret:
                    frame = cv2.resize(frame, (320, 240))
                    roi = self.processor.crop_roi(frame, 0, 240*0.6, 320, 240*0.4)
                    out = self.inf_session.run(self.model_out_name, {'images': self.transform(roi).unsqueeze(0).numpy()})[0]
                    direction = self.target_dict[out.argmax()]
                    frame = post_process(frame, direction)
                    frame = cv2.resize(frame, (640, 480))
                    # cv2.imshow(camera_name, frame)
                    msg = InferenceResult()
                    msg.result = out.argmax()
                    self.inference_output_publisher.publish(msg)
            except Exception as e:
                print(e)
                exit()
        
def main(args=None):
    rclpy.init(args=args)
    node = InferenceNode()
    rclpy.spin(node)
    rclpy.shutdown()

if __name__ == '__main__':
    main()

# if __name__ == "__main__":
    
#     while cap.isOpened():
#         ret, frame = cap.read()
        
#         try:
#             if ret:
#                 # image, ratio, dwdh = full_frame_preprocess(frame, auto=False)
#                 # breakpoint()
#                 #convert from tensor to numpy
#                 # np_image = transform(frame).unsqueeze(0).numpy()
#                 # resize frame 
#                 frame = cv2.resize(frame, (320, 240))
#                 roi = processor.crop_roi(frame, 0, 240*0.6, 320, 240*0.4)
#                 # roi = cv2.resize(roi, (96, 320))
#                 out = inf_session.run(model_out_name, {'images': transform(roi).unsqueeze(0).numpy()})[0]
#                 # bbox, score = full_frame_postprocess(out, ratio, dwdh, threshold)
#                 # if score >= threshold:
#                 #     cropped_image = crop_roi_image(frame, bbox, (224, 224))
#                 #     out = model_path.run(gesture_out_name, {'images': cropped_image/255})[0]
#                 #     gesture = target_dict[out.argmax()]
#                 #     draw_image(frame, bbox, score, gesture)
#                 client.Send('0', out.argmax())
#                 direction = target_dict[out.argmax()]
#                 print(direction)
#                 # print(direction)
#                 # breakpoint()
#                 frame = post_process(frame, direction)
#                 frame = cv2.resize(frame, (640, 480))
#                 cv2.imshow(camera_name, frame)
#                 # time.sleep(2)
#         except Exception as e:
#             print(e)
#             # breakpoint()
#             exit()
            
            
#         if cv2.waitKey(1) & 0xFF == ord('q'):
#             break