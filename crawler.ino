// Pin Definitions
#define TRIG_PIN 9         // Ultrasonic sensor trigger pin
#define ECHO_PIN 10        // Ultrasonic sensor echo pin
#define LEFT_MOTOR_A 5     // Left motor forward (PWM)
#define LEFT_MOTOR_B 6     // Left motor backward (PWM)
#define RIGHT_MOTOR_A 3    // Right motor forward (PWM)
#define RIGHT_MOTOR_B 4    // Right motor backward (PWM)
#define CAMERA_TRIGGER 7   // Camera trigger pin (e.g., OV7670)
#define LED_STATUS 11      // Status LED (green = moving, red = stopped)
#define BUZZER_PIN 12      // Buzzer for alerts
#define BATTERY_PIN A0     // Analog pin for battery voltage monitoring

// Constants
#define MIN_DISTANCE 20    // Minimum distance to obstacle in cm
#define MAX_SPEED 200      // Maximum PWM speed (0-255)
#define MIN_SPEED 100      // Minimum PWM speed for slow mode
#define TURN_DURATION 1200 // Time to turn in milliseconds
#define REVERSE_DURATION 800 // Time to reverse in milliseconds
#define IMAGE_DELAY 200    // Delay to simulate image capture
#define LOW_BATTERY_VOLT 6.5 // Low battery threshold (volts)
#define BUZZER_FREQ 1000   // Buzzer frequency in Hz

// Global Variables
int currentSpeed = MAX_SPEED;  // Current motor speed
bool isMoving = false;         // Track movement state
unsigned long lastImageTime = 0; // Timestamp of last image capture
const unsigned long imageInterval = 5000; // Capture image every 5 seconds

void setup() {
  // Initialize serial communication
  Serial.begin(9600);
  
  // Set pin modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LEFT_MOTOR_A, OUTPUT);
  pinMode(LEFT_MOTOR_B, OUTPUT);
  pinMode(RIGHT_MOTOR_A, OUTPUT);
  pinMode(RIGHT_MOTOR_B, OUTPUT);
  pinMode(CAMERA_TRIGGER, OUTPUT);
  pinMode(LED_STATUS, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BATTERY_PIN, INPUT);
  
  // Initial state
  stopMotors();
  digitalWrite(CAMERA_TRIGGER, LOW);
  digitalWrite(LED_STATUS, LOW);
  noTone(BUZZER_PIN);
  
  // Startup sequence
  Serial.println("Robotic Drain Crawler for Dengue Detection");
  Serial.println("Initializing...");
  startupSequence();
  
  // Check battery on startup
  checkBattery();
}

void loop() {
  // Measure distance to obstacle
  long distance = getDistance();
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  
  // Check battery level periodically
  checkBattery();
  
  // Navigation logic
  if (distance < MIN_DISTANCE) {
    handleObstacle();
  } else {
    moveForward();
    updateStatusLED(true);  // Green LED when moving
    captureImageIfNeeded();
  }
  
  // Slow down in tight spaces (optional feature)
  adjustSpeedBasedOnDistance(distance);
  
  delay(100);  // Stabilize readings
}

// --- Core Functions ---

long getDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  long duration = pulseIn(ECHO_PIN, HIGH);
  long distance = duration * 0.034 / 2;
  return constrain(distance, 0, 100);  // Limit range for reliability
}

void moveForward() {
  analogWrite(LEFT_MOTOR_A, currentSpeed);
  digitalWrite(LEFT_MOTOR_B, LOW);
  analogWrite(RIGHT_MOTOR_A, currentSpeed);
  digitalWrite(RIGHT_MOTOR_B, LOW);
  if (!isMoving) {
    Serial.println("Moving Forward");
    isMoving = true;
  }
}

void moveReverse() {
  analogWrite(LEFT_MOTOR_A, LOW);
  digitalWrite(LEFT_MOTOR_B, currentSpeed);
  analogWrite(RIGHT_MOTOR_A, LOW);
  digitalWrite(RIGHT_MOTOR_B, currentSpeed);
  Serial.println("Reversing");
  isMoving = true;
}

void turnRight() {
  analogWrite(LEFT_MOTOR_A, currentSpeed);
  digitalWrite(LEFT_MOTOR_B, LOW);
  digitalWrite(RIGHT_MOTOR_A, LOW);
  analogWrite(RIGHT_MOTOR_B, currentSpeed);
  Serial.println("Turning Right");
  isMoving = true;
}

void turnLeft() {
  digitalWrite(LEFT_MOTOR_A, LOW);
  analogWrite(LEFT_MOTOR_B, currentSpeed);
  analogWrite(RIGHT_MOTOR_A, currentSpeed);
  digitalWrite(RIGHT_MOTOR_B, LOW);
  Serial.println("Turning Left");
  isMoving = true;
}

void stopMotors() {
  digitalWrite(LEFT_MOTOR_A, LOW);
  digitalWrite(LEFT_MOTOR_B, LOW);
  digitalWrite(RIGHT_MOTOR_A, LOW);
  digitalWrite(RIGHT_MOTOR_B, LOW);
  if (isMoving) {
    Serial.println("Stopped");
    isMoving = false;
  }
  updateStatusLED(false);  // Red LED when stopped
}

// --- Feature Functions ---

void handleObstacle() {
  stopMotors();
  delay(500);
  
  // Decide to reverse or turn based on random choice (simulating decision-making)
  if (random(2) == 0) {
    moveReverse();
    delay(REVERSE_DURATION);
    stopMotors();
    delay(500);
    turnRight();
    delay(TURN_DURATION);
  } else {
    turnLeft();
    delay(TURN_DURATION);
  }
}

void adjustSpeedBasedOnDistance(long distance) {
  if (distance < 40 && distance >= MIN_DISTANCE) {
    currentSpeed = MIN_SPEED;
    Serial.println("Slowing down in tight space");
  } else {
    currentSpeed = MAX_SPEED;
  }
}

void captureImageIfNeeded() {
  unsigned long currentTime = millis();
  if (currentTime - lastImageTime >= imageInterval) {
    digitalWrite(CAMERA_TRIGGER, HIGH);
    delay(IMAGE_DELAY);  // Simulate camera capture time
    digitalWrite(CAMERA_TRIGGER, LOW);
    Serial.println("Captured Image for Dengue Detection");
    lastImageTime = currentTime;
    
    // Simulate egg detection alert (placeholder)
    if (random(10) < 2) {  // 20% chance of "detection"
      alertEggDetection();
    }
  }
}

void updateStatusLED(bool moving) {
  digitalWrite(LED_STATUS, moving ? HIGH : LOW);  // Green when moving, off when stopped
}

void checkBattery() {
  int rawValue = analogRead(BATTERY_PIN);
  float voltage = (rawValue / 1023.0) * 5.0 * 2;  // Assuming voltage divider for 9V battery
  Serial.print("Battery Voltage: ");
  Serial.print(voltage);
  Serial.println(" V");
  
  if (voltage < LOW_BATTERY_VOLT) {
    Serial.println("WARNING: Low Battery!");
    alertLowBattery();
  }
}

void alertEggDetection() {
  Serial.println("ALERT: Possible Dengue Eggs Detected!");
  tone(BUZZER_PIN, BUZZER_FREQ, 500);  // Short beep
  delay(600);
  tone(BUZZER_PIN, BUZZER_FREQ, 500);  // Double beep for emphasis
}

void alertLowBattery() {
  Serial.println("ALERT: Low Battery Detected!");
  for (int i = 0; i < 3; i++) {
    tone(BUZZER_PIN, BUZZER_FREQ, 200);
    delay(300);
  }
}

void startupSequence() {
  // Blink LED and beep to indicate startup
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED_STATUS, HIGH);
    tone(BUZZER_PIN, BUZZER_FREQ, 100);
    delay(200);
    digitalWrite(LED_STATUS, LOW);
    delay(200);
  }
  Serial.println("Startup Complete");
}
