/* FILE:watchdog_pir_sensor.ino
 * PROJECT: AutoX Watchdog
 * PROGRAMMER: Cavan Biggs
 * FIRST VERSION: January 27th 2020
 * DESCRIPTION: 
 *          
 *        
 *        
 *            
 *        
*/


//www.elegoo.com
//2016.12.9

#include <Servo.h>
#include <SoftwareSerial.h>

//Arduino Mega 2560 settings TX #11 RX #12
SoftwareSerial mySerial(11,12);
Servo camServo;
Servo rearServo; // create servo object to position camera to the rear

int ledPin = 13;  // LED on Pin 13 of Arduino
int frontPirPin = 7; // Input for HC-S501
int leftPirPin = 8;
int rightPirPin = 5;
int rearPirPin = 4;

int frontPirValue; // Place to store read PIR Value
int leftPirValue;
int rightPirValue;
int rearPirValue;
//Servo trap
int camServoCount = 0;
int rearServoCount = 0;

//Define command
String const LOOKFRONT = "CMD<LOOK_FRONT>";
String const LOOKLEFT = "CMD<LOOK_LEFT>";
String const LOOKRIGHT = "CMD<LOOK_RIGHT>";
String const LOOKBEHIND = "CMD<LOOK_BEHIND>";
String const RESP_BAD_COMMAND = "Invalid command received, please use valid command";
String const RESP_FRONT_IMG_CAPTURED = "Captured front image";
String const RESP_LEFT_IMG_CAPTURED = "Captured left image";
String const RESP_RIGHT_IMG_CAPTURED = "Captured right image";
String const RESP_READ_IMG_CAPTURED = "Captured rear image";

String incomingCMD = "";

//Variables for delaying sensors
unsigned long previousMillis = 0;
//interval constant 10 seconds
const long interval = 10000;

//Debounce setup
//Variables
int lastFrontPIR_State = LOW;
int lastLeftPIR_State = LOW;
int lastRightPIR_State = LOW;
int lastRearPIR_State = LOW;
int frontPIR_State;
int leftPIR_State;
int rightPIR_State;
int rearPIR_State;
//Time variables
unsigned long lastDebounceTime = 0; //the last time the output pin was toggled
unsigned long debounceDelay = 10000; //the debounce time.

void setup() {
  
  pinMode(ledPin, OUTPUT);
  pinMode(frontPirPin, INPUT);
  pinMode(leftPirPin, INPUT);
  pinMode(rightPirPin, INPUT);
  pinMode(rearPirPin, INPUT);

  digitalWrite(frontPirPin, LOW);
  digitalWrite(leftPirPin, LOW);
  digitalWrite(rightPirPin, LOW);
  digitalWrite(rearPirPin, LOW);
 
  digitalWrite(ledPin, LOW);
  Serial.begin(9600);
  //Servo setup
  camServo.attach(10);
  rearServo.attach(3);
  camServo.write(90); //set the servo to face forward
  rearServo.write(90);
  

//GSM setup 
//Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  Serial.print("Command Length: ");
  Serial.println(LOOKFRONT.length());
  //Begin serial communication with Arduino and SIM900
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
  Serial.println("Setting up wireless connection to network provider");
  delay(1000);

  mySerial.println("AT"); //Handshaking with SIM900
  updateSerial();
  
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  //Replace this with a startup method
  updateSerial();

  //Camera setup
  camera_SD_setup();
  
  //initialize sensor for 1 minute
  MotionSensorInitialization(); 
}

void loop() {
  frontPirValue = digitalRead(frontPirPin);
  leftPirValue = digitalRead(leftPirPin);
  rightPirValue = digitalRead(rightPirPin);
  rearPirValue = digitalRead(rearPirPin);

//   // Check if value changed due to noise or trigger
//  if (frontPirValue != lastFrontPIR_State ||
//      leftPirValue  != lastLeftPIR_State ||
//      rightPirValue != lastRightPIR_State ||
//      rearPirValue  != lastRearPIR_State)
//      {
//        lastDebounceTime = millis();      
//      }

    Serial.print("Camera Servo has moved: ");
    Serial.println(camServoCount);
    Serial.print("Rear Servo has moved: ");
    Serial.println(rearServoCount);

  if((millis() - lastDebounceTime) > debounceDelay)
  {
  
    Serial.print("Checking debounce time: ");
    Serial.println((millis() - lastDebounceTime));
    
  //Check if Front PIR state has changed
  //Check for motion detected
    if (frontPirValue == HIGH)
    {
        
          //Check if servos are already in position
          //Move servo to Front 90 degrees
          if (rearServo.read() != 90)
          {
            rearServo.write(90);
              rearServoCount++;
          }
          if (camServo.read() != 90)
          {
            camServo.write(90);
            camServoCount++;
            
          }
            Serial.println("Front motion sensor: FRONT MOTION DETECTED");
            
            //Send alert SMS
            processCommands(LOOKFRONT, "| ALERT:->Motion was detected at front of vehicle");
            //lastFrontPIR_State = frontPirValue;
            lastDebounceTime = millis();
       }
      else if (leftPirValue == HIGH)
        {
              //Move servo to Left 180 degrees
              if (rearServo.read() != 90)
              {
                rearServo.write(90);
                rearServoCount++;
              }
              if (camServo.read() != 180)
              {
                camServo.write(180);
                camServoCount++;  
              }
              
              Serial.println("Left motion sensor: LEFT MOTION DETECTED");
               //Send alert SMS
              processCommands(LOOKLEFT, ",| ALERT:->Motion was detected at left side of vehicle");
              //lastLeftPIR_State = leftPirValue;
              lastDebounceTime = millis();
        }
      else if (rightPirValue == HIGH)
          {
              //Move servo to the right 0 degrees
               if (rearServo.read() != 90)
              {
                rearServo.write(90);
                rearServoCount++;
              }
              if (camServo.read() != 0)
              {
               camServo.write(0);
               camServoCount++; 
              }
               
              Serial.println("Right motion sensor: RIGHT MOTION DETECTED");
               //Send alert SMS
              processCommands(LOOKRIGHT, "| ALERT:->Motion was detected at right side of vehicle");
              //lastRightPIR_State = rightPirValue;
              lastDebounceTime = millis();
          }
      else if (rearPirValue == HIGH){
    
              //Current servo on hand can only move 180 degrees left to right
              //...will need to auquire a servo capable of 360 degree movment.
              if (rearServo.read() != 180)
              {
                rearServo.write(180);
                rearServoCount++;  
              }
              if (camServo.read() != 180)
              {
                camServo.write(180);
                camServoCount++;  
              }
               
              Serial.println("Rear motion sensor: REAR MOTION DETECTED");
               //Send alert SMS
              processCommands(LOOKBEHIND, "| ALERT:->Motion was detected at the rear of the vehicle");
              //lastRearPIR_State = rearPirValue;
              lastDebounceTime = millis();
          }
  }

  //GSM module code
  updateSerial();
}

//PIR Sensors have a 1 minute initialization period when system is powered on
void MotionSensorInitialization(){
    delay(60000);
    
}




//GSM code

void updateSerial()
{
  int cmdIndex = 0;
  String inputCMD = "";
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    char inputChar = mySerial.read();
    incomingCMD.concat(inputChar);
    //Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
    if(inputChar == '\n')
    {
      //Parse for command
      //Get index of command
      cmdIndex = incomingCMD.indexOf("CMD<");
      if(cmdIndex != -1)
      {  
          //Get the command as substring
          inputCMD = incomingCMD.substring(cmdIndex);
          Serial.print("Received: ");
          Serial.println(inputCMD);
          //Parse methods here
          Serial.print("Sent command length: ");
          inputCMD.trim();
          Serial.println(inputCMD.length());
          //Serial.print(inputCMD, HEX);
          processCommands(inputCMD, "");
      }
      //Clear the buffer
      incomingCMD = "";
      
    }
  }
  
}

//Process the incoming command
void processCommands(String const command, String moreInfo){

  //Look for the following values
  //#define LOOKFRONT "CMD<LOOK_FRONT>"
  //#define LOOKLEFT "CMD<LOOK_LEFT>"
  //#define LOOKRIGHT "CMD<LOOK_RIGHT>"
  //#define LOOKBEHIND "CMD<LOOK_BEHIND>"
      Serial.print("Executing:..... \n");
      //Serial.print("Time of execution: ");
      //mySerial.println("AT+CCLK?");

  if (command.equals(LOOKFRONT))
  {
    //Move camera to the front and capture image
    rearServo.write(90);
    camServo.write(90); //centre camServo position
    Serial.println(RESP_FRONT_IMG_CAPTURED);
    //Capture image
    myCAMSaveToSDFile();
    prepareResponse("Front Image Captured " + moreInfo);
  }
  else if(command.equals(LOOKLEFT)){
  //Move camera to the left and capture image
  rearServo.write(90);
  camServo.write(180); 
  Serial.println(RESP_LEFT_IMG_CAPTURED);
  //Capture image
    myCAMSaveToSDFile();
  prepareResponse("Left Image Captured " + moreInfo);
    
  }
  else if(command.equals(LOOKRIGHT)){
    //Move camera to the right and capture image
    rearServo.write(90);
    camServo.write(0);
    Serial.println(RESP_RIGHT_IMG_CAPTURED);
    //Capture image
    myCAMSaveToSDFile();
    prepareResponse("Right Image Captured " + moreInfo);
    
  }
  else if(command.equals(LOOKBEHIND)){
    //Move camera to the right and capture image
    camServo.write(180);
    rearServo.write(180);
    Serial.println(RESP_READ_IMG_CAPTURED);
    //Capture image
    myCAMSaveToSDFile();
    prepareResponse("Rear Image Captured " + moreInfo);
  }
  else{
    Serial.println(RESP_BAD_COMMAND);
    prepareResponse(RESP_BAD_COMMAND);
  }

  
}

void prepareResponse(String responseMSG){

  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  responseSMS();
  mySerial.println("AT+CMGS=\"+15196088364\"");//change ZZ with country code and xxxxxxxxxxx with phone number to sms
  responseSMS();
  mySerial.print(responseMSG); //text content
  responseSMS();
  mySerial.write(26);
}

void responseSMS()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
