import cv2
import numpy as np
from tensorflow.keras.models import load_model

def detect_eggs(image_path, model_path='dengue_egg_cnn.h5'):
    model = load_model(model_path)
    img = cv2.imread(image_path, cv2.IMREAD_GRAYSCALE)
    img = cv2.resize(img, (64, 64)) / 255.0
    img = img[np.newaxis, ..., np.newaxis]  # Add batch and channel dimensions
    
    pred = model.predict(img)[0][0]
    return pred > 0.5, pred  # Boolean result, probability

if __name__ == "__main__":
    result, prob = detect_eggs('data/images/img1.jpg')
    print(f"Dengue Eggs Detected: {result}, Probability: {prob:.4f}")
