#include <SoftwareSerial.h>
// include the SD library:
#include <SPI.h>
#include <SD.h>


//Create software serial object to communicate with SIM900
//Settings for Mega 2560
SoftwareSerial mySerial(11, 12); //SIM900 Tx & Rx is connected to Arduino #11 & #12
//Settings for UNO
//SoftwareSerial mySerial(7, 8); //UNO Pins for Tx & Rx #7 & #8

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

const int SDchipSelect = 7;

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


//File handling source code
 

    //Check SD Module
    if (checkSDModule()){
      //Read image bytes
       readImageBytes();
      //Get the information about the sd card.
      //SD_Info();
  }else{
    Serial.println("Problem was detected with SD module");
  }
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
//When switching between Text Mode and MMS mode be sure to exit out of MMS...
// mode using AT+CMMSTERM, other wise when you send the command 'AT+CMMSINIT" you will get an error.


//Initialize SD Module
bool checkSDModule(){
  bool SD_OK = false;
     // we'll use the initialization code from the utility libraries
  // since we're just testing if the card is working!
  if (!card.init(SPI_HALF_SPEED, SDchipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    Serial.println("* did you change the chipSelect pin to match your shield or module?");
    while (1);
  } else {
    Serial.println("Wiring is correct and a card is present.");
    SD_OK = true;
  }
  return SD_OK;
}

void SD_Info(){

  // print the type of card
  Serial.println();
  Serial.print("Card type:         ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }

  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    while (1);
  }

  Serial.print("Clusters:          ");
  Serial.println(volume.clusterCount());
  Serial.print("Blocks x Cluster:  ");
  Serial.println(volume.blocksPerCluster());

  Serial.print("Total Blocks:      ");
  Serial.println(volume.blocksPerCluster() * volume.clusterCount());
  Serial.println();

  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("Volume type is:    FAT");
  Serial.println(volume.fatType(), DEC);

  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize /= 2;                           // SD card blocks are always 512 bytes (2 blocks are 1KB)
  Serial.print("Volume size (Kb):  ");
  Serial.println(volumesize);
  Serial.print("Volume size (Mb):  ");
  volumesize /= 1024;
  Serial.println(volumesize);
  Serial.print("Volume size (Gb):  ");
  Serial.println((float)volumesize / 1024.0);

  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);

  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
}


//AT+CMMSDOWN="PIC",12963,20000

void readImageBytes(){

  if (!SD.begin(SDchipSelect)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  
  //237.JPG
  File imageMMS;
  imageMMS = SD.open("/237.JPG");
  if(imageMMS){
    Serial.println("Reading image file: ");
    // read from file until all the contents have been read
    while (imageMMS.available()){
     // Serial.write(imageMMS.read());
      imageMMS.read();
    }
    //Print the size of the file
    Serial.println(imageMMS.size());
    //close the file
    imageMMS.close();
    Serial.println("Done reading file");
  }else {
    //could not open file 
    Serial.println("Error opening file");
  }
  
}


  
