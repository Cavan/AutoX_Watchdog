#include <Servo.h>

#define TURN_TIME 175

Servo myservo;
int stepClock = -1;
int stepCounterClock = -1;

void setup(){
    myservo.attach(9);
    // Initially the servo must be stopped 
    myservo.write(90);
    stepClock = 0;
    Serial.begin(9600);
}

void loop() {

    if(stepClock >= 0 && stepClock < 3)
    {
      Serial.print("stepClock: ");
      Serial.println(stepClock);
      
      moveClockWise();
      stepClock = stepClock + 1;
      if(stepClock == 3){
        stepCounterClock = 0;
        stepClock = -1;  
        
      }
        
    }else if(stepCounterClock >= 0 && stepCounterClock < 3)
    {
      Serial.print("stepCounterClock: ");
      Serial.println(stepCounterClock);
      
      moveCounterClockWise();
      stepCounterClock++;
      if(stepCounterClock == 3)
      {
        stepClock = 0;
        stepCounterClock = -1;
      }
      
    }
      
    // Go on turning for the right duration
    delay(TURN_TIME);
    // Stop turning
    myservo.write(90);
   
    // Wait for 12h
    //delay(12 * 3600 * 1000);
    delay(3000);
}

void moveClockWise(){
  // Start turning clockwise
    myservo.write(0);
}

void moveCounterClockWise(){
  // Start turning clockwise
    myservo.write(180);
}
