# Robotic-Drain-Crawler-for-Dengue-Detection

An autonomous robot designed to navigate drainage systems and detect dengue mosquito eggs using a Convolutional Neural Network (CNN). The robot maneuvers through drains, captures images, and processes them to identify egg presence, aiding in dengue prevention.

## Features
- Autonomous navigation through drains using an Arduino-controlled robot.
- CNN model to detect dengue eggs from camera-captured images.
- Real-time image processing and visualization of detection results.

## Hardware Requirements
- Arduino Uno (or compatible)
- DC Motors (x2) with motor driver (e.g., L298N)
- HC-SR04 Ultrasonic Sensor (for obstacle avoidance)
- OV7670 Camera Module
- Breadboard, wires, and power supply (e.g., 9V battery)

## Software Requirements
- Arduino IDE
- Python 3.8+
- Libraries: `tensorflow`, `opencv-python`, `numpy`, `pandas`, `matplotlib`

## Setup
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/kanjibro/Robotic-Drain-Crawler.git
   cd Robotic-Drain-Crawler
