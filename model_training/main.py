import torch
import cv2
import onnxruntime as ort
import os
import numpy as np
from utils.util import full_frame_preprocess, full_frame_postprocess, crop_roi_image, draw_image
from omegaconf import OmegaConf

hand_model_path = "../data/models/YoloV7_Tiny.onnx"
gesture_model_path = r"output\MobileNetV3_small\three_rmsprop_30\model.onnx"
provider = ['CPUExecutionProvider']
threshold = 0.5
conf = OmegaConf.load("configs/default.yaml")
target_dict = {i: label for i, label in enumerate(conf.dataset.targets)}

if __name__ == "__main__":
    
    camera_name = "/dev/video0"
    cap = cv2.VideoCapture(0)
    hand_inf_session = ort.InferenceSession(hand_model_path, providers=provider)
    hand_outname = [i.name for i in hand_inf_session.get_outputs()]
    gesture_inf_session = ort.InferenceSession(gesture_model_path, providers=provider)
    gesture_out_name = [i.name for i in gesture_inf_session.get_outputs()]
        
    while cap.isOpened():
        ret, frame = cap.read()
        
        try:
            if ret:
                image, ratio, dwdh = full_frame_preprocess(frame, auto=False)
                out = hand_inf_session.run(hand_outname, {'images': image/255})[0]
                bbox, score = full_frame_postprocess(out, ratio, dwdh, threshold)
                if score >= threshold:
                    cropped_image = crop_roi_image(frame, bbox, (224, 224))
                    out = gesture_inf_session.run(gesture_out_name, {'images': cropped_image/255})[0]
                    gesture = target_dict[out.argmax()]
                    draw_image(frame, bbox, score, gesture)
                cv2.imshow(camera_name, frame)
        except Exception as e:
            print(e)
            
            
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break