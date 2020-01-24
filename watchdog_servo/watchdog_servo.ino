

//www.elegoo.com
//2018.12.19
#include <Servo.h>
Servo myservo;

void setup(){
  myservo.attach(10);
  myservo.write(90);// move servos to center position -> 90°
  Serial.begin(9600);
} 
void loop(){
 // myservo.write(0);// move servos to center position -> 90°
  //delay(500);
 // myservo.write(30);// move servos to center position -> 60°
 // delay(500);
 // myservo.write(60);// move servos to center position -> 90°
  //delay(500);
  myservo.write(180);// move servos to center position -> 120°
  Serial.println("Move to 180 degrees");
  delay(1000);
 
  myservo.write(0);
  Serial.println("Move to 0 degrees");
  delay(1000);

  myservo.write(90); //move servo to front
  delay(2000);
}

//Setup a trigger to simulate a sensor,a button
//each button push adds 60 degrees when total angle reaches 180 degrees reset servo position
//Add 3 sensors that will each trigger a different angle of view
