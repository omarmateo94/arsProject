#include <Servo.h>

// Define constants for ultrasonic sensor pins
const int frontSensorPin = 2;
const int leftSensorPin = 3;
const int rightSensorPin = 4;

// Define constants for servo motor pins
const int leftMotorPin = 5;
const int rightMotorPin = 6;

// Define constants for ultrasonic sensor angles and ranges
const int frontAngle = 0;
const int leftAngle = 45;
const int rightAngle = -45;
const int maxRange = 200; // Maximum range of the ultrasonic sensors in cm

Servo leftMotor;
Servo rightMotor;

// Function to calculate the angle between two points
double calculateAngle(double x1, double y1, double x2, double y2) {
  return atan2(y2 - y1, x2 - x1);
}

// Function to determine which direction is more open based on ultrasonic sensor readings
int determineOpenDirection(int front, int left, int right) {
  if (front >= left && front >= right) {
    return frontAngle;
  } else if (left >= front && left >= right) {
    return leftAngle;
  } else {
    return rightAngle;
  }
}

// Function to convert ultrasonic sensor readings to a range between 0 and 1
double normalizeRange(int sensorReading) {
  return min((double)sensorReading / maxRange, 1.0);
}

void setup() {
  // Initialize servo motors
  leftMotor.attach(leftMotorPin);
  rightMotor.attach(rightMotorPin);

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read ultrasonic sensor readings
  int frontSensorReading = analogRead(frontSensorPin);
  int leftSensorReading = analogRead(leftSensorPin);
  int rightSensorReading = analogRead(rightSensorPin);

  // Convert sensor readings to ranges between 0 and 1
  double frontRange = normalizeRange(frontSensorReading);
  double leftRange = normalizeRange(leftSensorReading);
  double rightRange = normalizeRange(rightSensorReading);

  // Calculate the angle to the more open direction
  int theta = determineOpenDirection(frontRange, leftRange, rightRange);

  // Set servo motor speeds based on front sensor reading
  int leftSpeed = frontRange * 4;
  int rightSpeed = frontRange * 4;

  // Adjust servo motor speeds based on calculated angle
  if (theta == leftAngle) {
    leftSpeed = leftRange * 4;
    rightSpeed = rightRange * 4;
  } else if (theta == rightAngle) {
    leftSpeed = rightRange * 4;
    rightSpeed = leftRange * 4;
  }

  // Set servo motor speeds
  leftMotor.writeMicroseconds(1500 + leftSpeed);
  rightMotor.writeMicroseconds(1500 - rightSpeed);

  // Print sensor readings and motor speeds for debugging
  Serial.print("Front: ");
  Serial.print(frontSensorReading);
  Serial.print(" | Left: ");
  Serial.print(leftSensorReading);
  Serial.print(" | Right: ");
  Serial.print(rightSensorReading);
  Serial.print(" | Left Speed: ");
  Serial.print(leftSpeed);
  Serial.print(" | Right Speed: ");
  Serial.println(rightSpeed);

  delay(100);
}
