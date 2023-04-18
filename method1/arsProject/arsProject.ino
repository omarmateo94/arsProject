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
const int safeDistanceSides = 5;

//Arrays of history of values
int leftList[ARRAY_SIZE] = {0};
int rightList[ARRAY_SIZE] = {0};
int frontList[ARRAY_SIZE] = {0};

void store_and_normalize_sensors() {
    // Store the value in the respective arrays
    leftList[0] = distanceLeft;
    frontList[0] = distanceFront;
    rightList[0] = distanceRight;

    // Shift the existing values in the arrays
    for (int i = ARRAY_SIZE - 1; i > 0; i--) {
        leftList[i] = leftList[i - 1];
        frontList[i] = frontList[i - 1];
        rightList[i] = rightList[i - 1];
    }

    // Normalize the values in the arrays
    for (int i = 0; i < ARRAY_SIZE; i++) {
        leftList[i] = leftList[i] / distanceLeft;
        frontList[i] = frontList[i] / distanceFront;
        rightList[i] = rightList[i] / distanceRight;
    }
    // Check if the robot has been going in the same direction for too long
    if (leftList[0] == leftList[1] && frontList[0] == frontList[1] && rightList[0] == rightList[1]) {
        sameDirectionCount++;
        if (sameDirectionCount >= MAX_SAME_DIRECTION_COUNT) {
            move(1480,1520);
            Serial.println("STOP REPEATING STEPS");
        }
    } else {
        sameDirectionCount = 0; // Reset same direction count
    }
}

void setup()                                 // Built in initialization block
{
  Serial.begin(9600); // Starting Serial Terminal
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
}  
 
void loop() {
  updateDistance();
  //void store_and_normalize_sensors();
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
  if (front > 5 && left>5 && right>5) state=0;
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
  //Max=Front
  if (state==0){
    if(left<safeDistanceSides){
      move(1500, 1350);
      Serial.println("Front-right");
    }
    else if(right<safeDistanceSides){
    move(1650, 1500);
    Serial.println("Front-left");
    }
    else {
      move(1550, 1450);
      Serial.println("Front");

    }
  }
  //Max=Right
  else if (state==1) {
    if (front < safeDistance) {
      move(1480, 1560);
      Serial.println("Right-Backwards");
      //delay(400);
    }
    else {
      move(1600, 1500);
      Serial.println("Right");
    }
  }

  //Max=Left
  else if (state==1) {
    if (front < safeDistance) {
      move(1550, 1450);
      Serial.println("Left-Backwards");
      //delay(400);
    }
    else {
      move(1500, 1600);
      Serial.println("Left");
    }
  }
  //else if (state=2) {
     // move(1400, 1600);
     // Serial.println("Backwards");  
 // }
}


void updateDistance() {
  // Finding out the range for each sensor
  distanceLeft = getDistance(13,7);
  distanceFront = getDistance(12,6);
  distanceRight = getDistance(11,5);
  //distanceBack = getDistance(10, 4);
}

int getDistance(int pingPin,int echoPin){
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
   //Serial.print(cm);
   //Serial.println();
   delay(100);
   return cm;
}


long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
