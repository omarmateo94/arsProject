#include <Servo.h>

// Define constants for ultrasonic sensor pins
const int frontSensorPing = 12;
const int frontSensorEcho = 6;

const int leftSensorPing = 13;
const int leftSensorEcho = 7;

const int rightSensorPing = 11;
const int rightSensorEcho = 5;

int leftSensorReading;
int frontSensorReading;
int rightSensorReading;

// Define constants for ultrasonic sensor angles and ranges
const int frontAngle = 0;
const int leftAngle = -45;
const int rightAngle = 45;
const int maxRange = 200; // Maximum range of the ultrasonic sensors in cm

Servo leftMotor;
Servo rightMotor;

void getDistance() {
  long frontDuration, leftDuration, rightDuration;

  //Front sensor
   pinMode(frontSensorPing, OUTPUT);
   digitalWrite(frontSensorPing, LOW);
   delayMicroseconds(2);
   digitalWrite(frontSensorPing, HIGH);
   delayMicroseconds(10);
   digitalWrite(frontSensorPing, LOW);
   pinMode(frontSensorEcho, INPUT);
   frontDuration = pulseIn(frontSensorEcho, HIGH);
   frontSensorReading = microsecondsToCentimeters(frontDuration);

  //Left sensor
   pinMode(leftSensorPing, OUTPUT);
   digitalWrite(leftSensorPing, LOW);
   delayMicroseconds(2);
   digitalWrite(leftSensorPing, HIGH);
   delayMicroseconds(10);
   digitalWrite(leftSensorPing, LOW);
   pinMode(leftSensorEcho, INPUT);
   leftDuration = pulseIn(leftSensorEcho, HIGH);
   leftSensorReading = microsecondsToCentimeters(leftDuration);

   //Right Sensor
   pinMode(rightSensorPing, OUTPUT);
   digitalWrite(rightSensorPing, LOW);
   delayMicroseconds(2);
   digitalWrite(rightSensorPing, HIGH);
   delayMicroseconds(10);
   digitalWrite(rightSensorPing, LOW);
   pinMode(rightSensorEcho, INPUT);
   rightDuration = pulseIn(rightSensorEcho, HIGH);
   rightSensorReading = microsecondsToCentimeters(rightDuration);
   
   /*
   Serial.print(cm);
   Serial.print("cm");
   Serial.println();
   delay(100);
   */
}
// Function to calculate the angle between two points
double calculateAngle(double x1, double y1, double x2, double y2) {
  return atan2(y2 - y1, x2 - x1);
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

// Function to determine which direction is more open based on ultrasonic sensor readings
int determineOpenDirection(double front, double left, double right) {
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
  leftMotor.attach(13);
  rightMotor.attach(12);

  // Initialize serial communication
  Serial.begin(9600);
}


void loop() {
  // Update distances from ultrasonics
  getDistance();

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
    leftSpeed = -200;
    rightSpeed = 200;
  } else if (theta == rightAngle) {
    leftSpeed = 200;
    rightSpeed = -200;
  } else {
    leftSpeed = 200;
    rightSpeed = 200;
  }

  // Set servo motor speeds
  leftMotor.writeMicroseconds(1500 + leftSpeed);
  rightMotor.writeMicroseconds(1500 - rightSpeed);

  // Print sensor readings and motor speeds for debugging
  Serial.print("Theta: ");
  Serial.println(theta);  /*
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
*/
  delay(100);
}
