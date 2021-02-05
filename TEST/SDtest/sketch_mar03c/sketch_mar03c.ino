

#include <SD.h>
#include <SPI.h>
#include <FS.h>
#include <FSImpl.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define SD_CS 5                              //SD PIN
LiquidCrystal_I2C lcd( 0x27, 16 , 2 );       //LCD OBJECT

File myFile;                                  // SD FILE OBJECT
//#define CS_RD 12
String dataMessage;                           // STRING OBJECT, FORMAT IN LOGSDCARD() METHOD TO SAVE MESSAGE TO FILE IN SD CARD



void setup() {  
 lcd.begin();                                  // LCD BEGIN
 lcd.backlight();                              // LCD LIGHT ON
 Serial.begin( 9600 );                         // SERIAL BEGIN

  SD.begin(SD_CS);                              // SD BLOCK BEGIN
  isSdCheck();                                  // CHECK IF THE SD IS MOUNTED
  isFileCreate();                               // CHECK IF SD HAS FILE NAME DATA, IF NOT, CREATE NEW FILE CALL DATA.TEXT
}

void loop () {
  isSdCheck();                                  // CHECK IF THE SD IS MOUNTED
  isFileCreate();   
  logSDCard();
  delay(3000);
  }



//---------- SD FUNCTIONS

void logSDCard() {                             // LOGSD METHOD, DEFINE HOW DATA SHOULD BE FORMAT AND DISPLAY IN THE FILE
  String dataMessage;
  //DateTime now = rtc.now();
  //char buf2[] = "YY/MM/DD-hh:mm:ss";           // READ "RTCLIB" LIBARY TO SEE DIFFERENT DATE, TIME TOSTRING() FORMAT
  //char buf4[] = "MM-DD-YYYY";                  // READ "RTCLIB" LIBARY TO SEE DIFFERENT DATE, TIME TOSTRING() FORMAT
  int number1 = 0;
  int number2 = rand();
  dataMessage = String(number2);                    //////////////change here for the setup of SD Log ( add more oxy, temp, ph level here) exampla: + " " + serial2.read().
  Serial.print("Save data: ");                        //
  Serial.println(dataMessage);                        // CHECK IN SERIAL IF DATA IS CORRECT
  appendFile(SD, "/data.txt", dataMessage.c_str());   // APPEND MESSAGE TO THE RIGHT FILE NAME. (CHECK
}



void writeFile(fs::FS &fs, const char * path, const char * message) {            // WRITE TO SD CARD
  Serial.printf("Writing file: %s\n", path);
  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.println(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
  file.close();
}




void appendFile(fs::FS &fs, const char * path, const char * message) {            // APPEND TO SD CARD
  Serial.printf("Appending to file: %s\n", path);
  File file = fs.open(path, FILE_APPEND);
  if (!file) {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.println(message)) {
    Serial.println("Message appended");
  } else {
    Serial.println("Append failed");
  }
  file.close();
}


void isFileCreate() {                                                           // CHECK IF FILE IS CREATED, IF NOT, CREATE FILE
  File file = SD.open("/data.txt");
  if (!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "/data.txt", "Temp, Date \r\n"); //////// Change here for value and set up of Displaying data in SD card
  }
  else {
    Serial.println("File already exists");
  }
  file.close();
}



void isSdCheck() {                                                              // CHECK IS SD IS MOUNT OR NOT
  SD.begin(SD_CS);
  if (!SD.begin(SD_CS)) {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("No SD card attached");
    return;
  }
  Serial.println("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    Serial.println("ERROR - SD card initialization failed!");
    return; // init failed
  }
}


  
