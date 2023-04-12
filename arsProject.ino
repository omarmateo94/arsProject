 /*
 Robotics with the BOE Shield – ServosOppositeDirections
 Generate a servo full speed counterclockwise signal with pin 13 and
 full speed clockwise signal with pin 12.
 */

#include <Servo.h>                           // Include servo library
 
Servo servoLeft;                             // Declare left servo signal
Servo servoRight;                            // Declare right servo signal
const int pingPin; // Trigger Pin of Ultrasonic Sensor
const int echoPin; // Echo Pin of Ultrasonic Sensor
const int state=0; //State is used to change the commands of the robot

// Range on cm for each of the ultrasonics
const int distanceLeft;     //Left Ultrasonic
const int distanceFront;    //Front Ultrasonic
const int distanceRight;    //Right Ultrasonic
const int distanceBack;     //Back Ultrasonic

void setup()                                 // Built in initialization block
{
  Serial.begin(9600); // Starting Serial Terminal
  servoLeft.attach(13);                      // Attach left signal to pin 13
  servoRight.attach(12);                     // Attach right signal to pin 12
}  
 
void loop() {
  updateDistance();
  updateState();
  set_command(state);
  
}

void move(int left, int right){
  servoLeft.writeMicroseconds(left);  
  servoRight.writeMicroseconds(right);
  }

void updateState() {
  //Move forward
  if(distanceFront > 10 && distanceLeft > 10 && distanceRight > 10){
    state = 0;
  }

  // Move to the left
  else if( distanceFront <= 10 && distanceRight <= 10 && distanceLeft > 10){
    state = 1;
  }

  // Move to the right
  else if( distanceFront <= 10 && distanceRight <= 10 && distanceLeft > 10){
    state = 2;
  }

  // Back up
  else if( distanceFront <= 10 && distanceRight <= 10 && distanceLeft <= 10){
    state = 3;
  }
}

void  set_command(state){
  switch(case) {
    case 0: //Move Forward
      move(1700,1700); //UPDATE VALUES ONCE ROBOT WHEELS ARE TESTED
      break;
    
    case 1: //Move to the left
      move()
      break;

    case 2: //Move to the left
      move()
      break;

    case 3: //Move to the left
      move()
      break;

    case 4: //Emergency stop
      move(1500,1500)
      break;
  }
  
  }

void updateDistance() {
  // Finding out the range for each sensor
  distanceLeft = getDistance(13,7);
  distanceFront = getDistance(12,6);
  distanceRight = getDistance(11,5);
  distanceBack = getDistance(10, 4);
}

int getDistance(pingPin, echoPin){
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
   Serial.print(cm);
   Serial.println();
   delay(100);
   return cm;
}


long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
