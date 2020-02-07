//Demo Unit Code

#include <SoftwareSerial.h>
#include <Servo.h>
// source code citation: https://lastminuteengineers.com/sim900-gsm-shield-arduino-tutorial/
//Create software serial object to communicate with SIM900
//Arduino Mega 2560 settings TX #11 RX #12
SoftwareSerial mySerial(11, 12);
//UNO RX, TX settings
//SoftwareSerial mySerial(11, 12); //SIM900 Tx & Rx is connected to Arduino #11 & #12
//Create servo object for the main camera
Servo camServo;
//Positions the main servo to look behind due to each servo being limited to 180degrees
Servo rearCamServo;

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

void setup()
{
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

  //Setup servo
  camServo.attach(10);
  camServo.write(90); //centre camServo position
}

void loop()
{
  updateSerial();
}

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
          processCommands(inputCMD);
      }
      //Clear the buffer
      incomingCMD = "";
      
    }
  }
  
}

//Process the incoming command
void processCommands(String const command){

  //Look for the following values
  //#define LOOKFRONT "CMD<LOOK_FRONT>"
  //#define LOOKLEFT "CMD<LOOK_LEFT>"
  //#define LOOKRIGHT "CMD<LOOK_RIGHT>"
  //#define LOOKBEHIND "CMD<LOOK_BEHIND>"
      Serial.print("Executing:..... ");
      

  if (command.equals(LOOKFRONT))
  {
    //Move camera to the front and capture image
    camServo.write(90); //centre camServo position
    Serial.println(RESP_FRONT_IMG_CAPTURED);
    prepareResponse("Front Image Captured");
  }
  else if(command.equals(LOOKLEFT)){
  //Move camera to the left and capture image
  camServo.write(180); 
  Serial.println(RESP_LEFT_IMG_CAPTURED);
  prepareResponse("Left Image Captured");
    
  }
  else if(command.equals(LOOKRIGHT)){
    //Move camera to the right and capture image
    camServo.write(0);
    Serial.println(RESP_RIGHT_IMG_CAPTURED);
    prepareResponse("Right Image Captured");
    
  }
  else if(command.equals(LOOKBEHIND)){
    //Move camera to the right and capture image
    Serial.println(RESP_READ_IMG_CAPTURED);
    prepareResponse("Read Image Captured");
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
