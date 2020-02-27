#include <SoftwareSerial.h>

//Arduino Mega 2560 settings TX #11 RX #12
SoftwareSerial mySerial(11,12);




void setup() {
  // put your setup code here, to run once:
  //Begin serial communication with Arduino and SIM900
  mySerial.begin(9600);
  Serial.println("Initializing..."); 
  Serial.println("Setting up wireless connection to network provider for sending MMS messasges");
  delay(1000);
  //MMS configuration
  //mySerial.println("AT"); //Handshaking with SIM900
  
  //MMS Setup
  //mmsConfiguration();

  
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("System Running");
  delay(2000);
}


//The code in this file will retrieve the stored image and send it to the user.

//Set MMS configuration for the Rogers network
void mmsConfiguration(){
  //mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  mySerial.println("AT+CMMSINIT"); // Initialize MMS mode
  updateSerial();
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

//Set Mode to MMS
void setMMS_Mode(){
  
}


//Send MMS to registered user
void sendMMS(){
  
}
