/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

Servo mainServo;  // create servo object to control a servo
Servo rearServo; // create servo object to position camera to the rear
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  mainServo.attach(8);  // attaches the servo on pin 9 to the servo object
  rearServo.attach(9);
  mainServo.write(90);
  rearServo.write(90);
}

void loop() {

  //move main servo left
  mainServo.write(180);
  delay(2000);
  //move main servo right
  mainServo.write(0);
  delay(2000);
  //move main servo to centre
  mainServo.write(90);
  delay(2000);
  //move main servo to the left hold position...
  //then move secondary servo left  
  mainServo.write(180);
  rearServo.write(180);
  delay(2000);
  rearServo.write(90);
  delay(1000);

  
//  for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
//    mainServo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
//  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//    mainServo.write(pos);              // tell servo to go to position in variable 'pos'
//    delay(15);                       // waits 15ms for the servo to reach the position
//  }
}
