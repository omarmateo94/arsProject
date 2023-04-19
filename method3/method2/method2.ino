/*
 Robotics with the BOE Shield – ServosOppositeDirections
 Generate a servo full speed counterclockwise signal with pin 13 and
 full speed clockwise signal with pin 12.
 */

#include <Servo.h>                           // Include servo library


Servo servoLeft;                             // Declare left servo signal
Servo servoRight;                            // Declare right servo signal

// Define constants for ultrasonic sensor pins
const int frontSensorPing = 12;
const int frontSensorEcho = 6;

const int leftSensorPing = 13;
const int leftSensorEcho = 7;

const int ARRAY_SIZE = 15;
const int MAX_SAME_DIRECTION_COUNT = 5;
int sameDirectionCount = 0;

const int rightSensorPing = 11;
const int rightSensorEcho = 5;

int state=0; //State is used to change the commands of the robot

// Range on cm for each of the ultrasonics
int distanceLeft;     //Left Ultrasonic
int distanceFront;    //Front Ultrasonic
int distanceRight;    //Right Ultrasonic
int distanceBack;     //Back Ultrasonic

//Safe distance in cm.
const int safeDistance = 5;
const int safeDistanceSides = 10;
int prevFront;
int prevLeft;
int prevRight;


void setup()                                 // Built in initialization block
{
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
  Serial.begin(9600); // Starting Serial Terminal
}  
 
void loop() {
  updateDistance();
  updateState(distanceLeft,distanceFront,distanceRight);
  setCommand(distanceLeft,distanceFront,distanceRight);
  
}

void move(int left, int right){
  servoLeft.writeMicroseconds(left);  
  servoRight.writeMicroseconds(right);
  }

void updateState(int left, int front, int right){
  // -1:left
  // 0:front
  // 1: right
  //2: Backwards
  if (front > 5 /*&& left>5 && right>5*/) state=0;
  else if (right >= front && right >= left) state=1;
  else if (left >= right && left >= front) state=-1;
 // else if(right < safeDistanceSides && left < safeDistanceSides && front <safeDistance) state=2;
}

void setCommand(int left, int front, int right) {
  Serial.print("Front: ");
  Serial.print(distanceFront);
  Serial.print(" | Left: ");
  Serial.print(distanceLeft);
  Serial.print(" | Right: ");
  Serial.println(distanceRight);
 

 if (front < safeDistance) {
   move(1500,1500); //Stop
   if(left > right) move(1400,1500);
   else move(1500,1600);
 }
 else if(left < safeDistance) {
   move(1500,1500); //Stop
   move(1500,1580);
   delay(200);
 }
 else if(right <safeDistance){
  move(1500,1500); //Stop
   move(1420,1500);
   delay(200);
 }
 else move(1600,1400);
}


void updateDistance() {
  // Finding out the range for each sensor
  distanceLeft = getDistance(13,7, prevLeft, -1);
  distanceFront = getDistance(12,6, prevFront, 0);
  distanceRight = getDistance(11,5, prevRight, 1);
}

int getDistance(int pingPin,int echoPin, int prevReading, int whichSensor){
  long duration, cm;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   cm = microsecondsToCentimeters(duration);
   delay(100);
   if (cm < 4) {
     return prevReading;
   }
   else{
     if (whichSensor == -1) prevLeft = cm;
     if (whichSensor == 0) prevFront = cm;
     if (whichSensor ==1) prevRight = cm;
   }
   return cm;
}


long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
