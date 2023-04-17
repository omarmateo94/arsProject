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

void setup()                                 // Built in initialization block
{
  Serial.begin(9600); // Starting Serial Terminal
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
}  
 
void loop() {
  updateDistance();
  //getMax(distanceLeft,distanceFront,distanceRight);
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
  if (front >= right && front >= left) state=0;
  else if (right >= front && right >= left) state=1;
  else if (left >= right && left >= front) state=-1;
 // else if(right < safeDistanceSides && left < safeDistanceSides && front <safeDistance) state=2;
}

void setCommand(int left, int front, int right) {
Serial.println(distanceFront);
  //Max=Front
  if (state==0){
    if(left<safeDistanceSides){
      move(1550, 1350);
      Serial.println("Front-right");
    }
    else if(right<safeDistanceSides){
    move(1650, 1450);
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
      move(1500, 1600);
      Serial.println("Right-Backwards");
    }
    else {
      move(1600, 1500);
      Serial.println("Right");
    }
  }

  //Max=Left
  else if (state==1) {
    if (front < safeDistance) {
      move(1600, 1500);
      Serial.println("Left-Backwards");
    }
    else {
      move(1500, 1600);
      Serial.println("Left");
    }
  }
  else if (state=2) {
      move(1400, 1600);
      Serial.println("Backwards");  
  }
}


/*
void updateState() {
  //Move forward
  if(distanceFront > safeDistance && distanceLeft > safeDistance && distanceRight > safeDistance){
    state = 0;
  }

  // Move to the left
  else if( distanceRight <= safeDistance && distanceLeft > safeDistance){
    state = 1;
  }

  // Move to the right
  else if( distanceFront <= safeDistance && distanceRight <= safeDistance && distanceLeft > safeDistance){
    state = 2;
  }

  // Back up
  else if( distanceFront <= safeDistance && distanceRight <= safeDistance && distanceLeft <= safeDistance){
    state = 3;
  }
}

void  set_command(){
  switch(state) {
    case 0: //Move Forward
      move(1600,1400); //UPDATE VALUES ONCE ROBOT WHEELS ARE TESTED
      Serial.println("Forward");
      break;  
    
    case 1: //Move to the left
      move(1400,1400);
      Serial.println("Left");
      break;

    case 2: //Move to the right
      move(1600,16000);
      Serial.println("Right");
      break;

    case 3: //back up
      move(1400,1600);
      Serial.println("Backwards");
      break;

    case 4: //Emergency stop
      move(1500,1500);
      Serial.println("STOP");
      break;
  }
  
  }
*/
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
   delay(150);
   return cm;
}


long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
