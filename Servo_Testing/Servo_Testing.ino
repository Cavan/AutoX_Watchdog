
#include <Servo.h>
Servo camServo;
Servo rearServo; // create servo object to position camera to the rear

//Variables for delaying sensors
unsigned long previousMillis = 0;
//interval constant 10 seconds
const long interval = 10000;
//Time variables
unsigned long lastDebounceTime = 0; //the last time the output pin was toggled
unsigned long debounceDelay = 2000; //the debounce time.
int counter = 0;

void setup() {
  // put your setup code here, to run once:
  camServo.attach(10);
  rearServo.attach(3);
  camServo.write(90); //set the servo to face forward
  rearServo.write(90);
}

void loop() {
  // put your main code here, to run repeatedly:
  if((millis() - lastDebounceTime) > debounceDelay)
  {
    //Set the servo switch back to true now that 10 seconds have passed
   // moveCamera = true;
    Serial.print("Checking debounce time: ");
    Serial.println((millis() - lastDebounceTime));

    if (counter == 0){
      //if counter == 0 move left
      
      moveServos(180,90);
      counter++;
      lastDebounceTime = millis();
    }
    else if (counter == 1){
      //if counter == 1 move right  
      moveServos(0,90);
      counter++;
      lastDebounceTime = millis();  
    }
    else if (counter == 2){
      //if counter == 2 move behind
      moveServos(180,180);
      counter++;
      lastDebounceTime = millis();  
    }
    else if (counter == 3){
      //if counter == 3 move front
      moveServos(90,90);
      counter = 0;
      lastDebounceTime = millis(); 
    }
    

   

    
  
  
  
  }
}
