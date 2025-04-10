import pandas as pd
import cv2
import numpy as np
import os

def load_and_preprocess_images(metadata_file='data/sample_metadata.csv', img_size=(64, 64)):
    """Load and preprocess images for CNN."""
    df = pd.read_csv(metadata_file)
    images = []
    labels = []
    
    for _, row in df.iterrows():
        img_path = row['file_path']
        label = row['label']
        if os.path.exists(img_path):
            img = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)
            img = cv2.resize(img, img_size)
            img = img / 255.0  # Normalize
            images.append(img)
            labels.append(label)
        else:
            print(f"Warning: {img_path} not found. Using placeholder.")
            images.append(np.zeros(img_size))
            labels.append(label)
    
    return np.array(images), np.array(labels)

if __name__ == "__main__":
    X, y = load_and_preprocess_images()
    print("Images Shape:", X.shape)
    print("Labels Shape:", y.shape)
