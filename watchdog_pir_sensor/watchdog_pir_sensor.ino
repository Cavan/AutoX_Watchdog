//www.elegoo.com
//2016.12.9

#include <Servo.h>

Servo camServo;


int ledPin = 13;  // LED on Pin 13 of Arduino
int frontPirPin = 7; // Input for HC-S501
int leftPirPin = 8;
int rightPirPin = 5;
int rearPirPin = 4;

int frontPirValue; // Place to store read PIR Value
int leftPirValue;
int rightPirValue;
int rearPirValue;


void setup() {
  
  pinMode(ledPin, OUTPUT);
  pinMode(frontPirPin, INPUT);
  pinMode(leftPirPin, INPUT);
  pinMode(rightPirPin, INPUT);
  pinMode(rearPirPin, INPUT);
 
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
  //Servo setup
  camServo.attach(10);
  camServo.write(90); //set the servo to face forward

  
  
  //initialize sensor for 1 minute
  MotionSensorInitialization(); 
}

void loop() {
  frontPirValue = digitalRead(frontPirPin);
  leftPirValue = digitalRead(leftPirPin);
  rightPirValue = digitalRead(rightPirPin);
  rearPirValue = digitalRead(rearPirPin);
  digitalWrite(ledPin, frontPirValue);
  digitalWrite(ledPin, leftPirValue);
  digitalWrite(ledPin, rightPirValue);
  digitalWrite(ledPin, rearPirValue);
  //Check for motion detected
  if(frontPirValue == HIGH)
  {
    //Move servo to Front 90 degrees
    
    camServo.write(90);
    
    Serial.println("Front motion sensor: FRONT MOTION DETECTED");
  }
  if (leftPirValue == HIGH)
  {
    //Move servo to Left 180 degrees
    camServo.write(180);
    Serial.println("Left motion sensor: LEFT MOTION DETECTED");
  }
  if (rightPirValue == HIGH)
  {
    //Move servo to the right 0 degrees
     camServo.write(0);
    
    Serial.println("Right motion sensor: RIGHT MOTION DETECTED");
  }
  if (rearPirValue == HIGH){

    //Current servo on hand can only move 180 degrees left to right
    //...will need to auquire a servo capable of 360 degree movment.
    
    //Serial.println("Rear motion sensor: REAR MOTION DETECTED");
  }
}

//PIR Sensors have a 1 minute initialization period when system is powered on
void MotionSensorInitialization(){
    delay(60000);
    
}
