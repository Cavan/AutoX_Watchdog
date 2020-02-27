#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM900
//Settings for Mega 2560
SoftwareSerial mySerial(11, 12); //SIM900 Tx & Rx is connected to Arduino #11 & #12
//Settings for UNO
//SoftwareSerial mySerial(7, 8); //UNO Pins for Tx & Rx #7 & #8
void setup()
{
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM900
  mySerial.begin(9600);

  Serial.println("Initializing..."); 
  delay(1000);

  mySerial.println("AT"); //Handshaking with SIM900
  updateSerial();
  
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  updateSerial();
  mySerial.println("AT+CMMSINIT"); // Initialize MMS mode
  updateSerial();
  //Configure MMS Settings to network provider
  mmsConfiguration();
}

void loop()
{
  updateSerial();
}

void updateSerial()
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


//Set MMS configuration for the Rogers network
void mmsConfiguration(){
  //mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
 // mySerial.println("AT+CMMSINIT"); // Initialize MMS mode
  //updateSerial();
  mySerial.println("AT+CMMSCURL=\"mms.gprs.rogers.com\""); //Set MMS centre
  updateSerial();
  mySerial.println("AT+CMMSCID=1"); //Set bearer context id
  updateSerial();
  mySerial.println("AT+CMMSPROTO=\"10.128.1.69\",80"); //Set the MMS Proxy and Port
  updateSerial();
  mySerial.println("AT+CMMSSENDCFG=6,3,0,0,2,4"); //Parameters for MMS PDU
  updateSerial();
  mySerial.println("AT+SAPBR=3,1,\"Contype\",\"GPRS\""); //Set bearer parameter
  updateSerial();
  mySerial.println("AT+SAPBR=3,1,\"APN\",\"CMWAP\"");
  updateSerial();
  mySerial.println("AT+SAPBR=1,1"); //Active bearer context
  updateSerial();
  mySerial.println("AT+SAPBR=2,1"); 
  updateSerial();
  
}
//When switching between Text Mode and MMS mode be sure to exit out of MMS...
// mode using AT+CMMSTERM, other wise when you send the command 'AT+CMMSINIT" you will get an error.
