import matplotlib.pyplot as plt
from cnn_model import train_model

def visualize_training(history):
    plt.figure(figsize=(10, 5))
    plt.plot(history.history['accuracy'], label='Train Accuracy')
    plt.plot(history.history['val_accuracy'], label='Validation Accuracy')
    plt.title('CNN Training Accuracy')
    plt.xlabel('Epoch')
    plt.ylabel('Accuracy')
    plt.legend()
    plt.savefig('training_accuracy.png')
    plt.close()

if __name__ == "__main__":
    _, history = train_model()
    visualize_training(history)
    print("Visualization saved as 'training_accuracy.png'")
