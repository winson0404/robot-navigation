import os
import cv2
import numpy as np

class DataProcessor:
    def __init__(self):
        pass

    def rotate(self, image, angle):
        (h, w) = image.shape[:2]
        center = (w / 2, h / 2)

        # Perform the rotation
        M = cv2.getRotationMatrix2D(center, angle, 1.0)
        rotated = cv2.warpAffine(image, M, (w, h))

        return rotated

    def resize(self, image, width=None, height=None):
        dim = None
        (h, w) = image.shape[:2]

        if width is None and height is None:
            return image

        if width is None:
            r = height / float(h)
            dim = (int(w * r), height)
        else:
            r = width / float(w)
            dim = (width, int(h * r))

        resized = cv2.resize(image, dim, interpolation = cv2.INTER_AREA)

        return resized

    def to_gray(self, image, in_place=False):
        gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

        if in_place:
            image = gray
        return gray
    
    def brightness_contrast_adjustment(self, image, brightness=0, contrast=0):
        if len(image.shape) == 2:  # Check if the image is grayscale
            adjusted = cv2.convertScaleAbs(image, alpha=(1 + contrast / 100), beta=brightness)
        else:
            img = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)
            img = img.astype(np.float32)
            img[:,:,2] = img[:,:,2] * (1 + brightness/100)
            img[:,:,1] = img[:,:,1] * (1 + contrast/100)
            img = np.clip(img, 0, 255)
            adjusted = cv2.cvtColor(img.astype(np.uint8), cv2.COLOR_HSV2BGR)
        return adjusted
    
    def noise_injection(self, image, mean=0, stddev=1):
        noise = np.random.normal(mean, stddev, image.shape).astype(np.uint8)
        noisy_image = cv2.add(image, noise)
        return noisy_image
    
    def crop_roi(self, frame, x, y, w, h):
        x = int(x)
        y = int(y)
        w = int(w)
        h = int(h)
        return frame[y:y+h, x:x+w]