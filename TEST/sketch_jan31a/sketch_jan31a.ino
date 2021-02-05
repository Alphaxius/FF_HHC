

#include <SD.h>
#include <SPI.h>
#include <FS.h>
#include <FSImpl.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define TEMP 34



#define SD_CS 5                              //SD PIN
LiquidCrystal_I2C lcd( 0x27, 16 , 2 );       //LCD OBJECT
RTC_DS3231 rtc;                              // RTC OBJECT
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

#define CLK 14                               //ROTARY ENCODER PIN
#define DT 4                                 //ROTARY ENCODER PIN
#define SW 5                                 //ROTARY ENCODER PIN

//////////////////////////////////////////////////////////

double SetTemp = 0;                          // Set point of temperature
const int analogIn = A0;                     // ANALOG INPUT FROM LM35 TEST TEMPERATURE SENSOR

int temp_counter = 0;                        // ROTARY ENCODER
int buttonCounter = 0;                       // ROTARY ENCODER
int counter = 0;                             // ROTARY ENCODER
int currentStateCLK;                         // ROTARY ENCODER
int lastStateCLK;                            // ROTARY ENCODER
String currentDir = "";                      // ROTARY ENCODER
unsigned long lastButtonPress = 0;           // ROTARY ENCODER
int menuSystem = 0;
unsigned long lastLogToSDCard = 0;

// Define deep sleep options
uint64_t uS_TO_S_FACTOR = 1000000; // Conversion factor for micro seconds to seconds
// Sleep for 10 minutes = 600 seconds
uint64_t TIME_TO_SLEEP = 600;


File myFile;                                  // SD FILE OBJECT
//#define CS_RD 12
String dataMessage;                           // STRING OBJECT, FORMAT IN LOGSDCARD() METHOD TO SAVE MESSAGE TO FILE IN SD CARD

//////////////////////////////////////////////////////////DEFINE VARIABLES

void setup() {                                // SET UP START

  SetTemp = 50;                                 // SetTemp set point default
  pinMode( TEMP, INPUT );                       // PINMODE ANALOG TEMPERATURE INPUT
  pinMode( SD_CS, OUTPUT);                      // PINMODE FOR SD CARD PIN

  pinMode(CLK, INPUT);                          // PINMODE FOR ROTARY ENCODER
  pinMode(DT, INPUT);                           // PINMODE FOR ROTARY ENCODER
  pinMode(SW, INPUT_PULLUP);                    // PINMODE FOR ROTARY ENCODER

  lcd.begin();                                  // LCD BEGIN
  lcd.backlight();                              // LCD LIGHT ON
  Serial.begin( 9600 );                         // SERIAL BEGIN
  lastStateCLK = digitalRead(CLK);              // INITIAL STATE OF CLK FOR ROTARY ENCODER, UPDATE FOR LASTSTATE CLK


  lcd.clear();                                  // INITIAL LCD DEFAULT SCREEN BEGIN
  lcd.print("Set Temp");
  lcd.setCursor(10, 0);
  counter = SetTemp;
  lcd.print(counter);
  lcd.setCursor(0, 1);
  lcd.print("Temp");                            // INITIAL LCD DEFAULT SCREEN BEGIN



  rtc.begin();                                  // RTC OBJECT BEGIN
  if (! rtc.begin()) {                          // RTC BLOCK SETUP BEGIN
    Serial.println("Couldn't find RTC");          //
    while (1);                                    // RTC BLOCK SETUP END
  }
  //rtc.adjust(DateTime(__DATE__, __TIME__));   // IF TIME IS WRONG, ENABLE THIS CODE, BURN THE CODE TO THE ESP32 TO GET RIGHT TIME AND DATE, THEN DISABLE IT.





  SD.begin(SD_CS);                              // SD BLOCK BEGIN
  isSdCheck();                                  // CHECK IF THE SD IS MOUNTED
  isFileCreate();                               // CHECK IF SD HAS FILE NAME DATA, IF NOT, CREATE NEW FILE CALL DATA.TEXT




}// set up end                                // SET UP END


double temp()                                 // TEMPERATURE INPUT
{
  return map( analogRead( TEMP ), 0, 2048, 2, 500 );
}




void loop() {                                  // LOOP STARTS HERE
  DateTime now = rtc.now();                      // INITIAL DATE AND TIME OBJECT (CLOCK)
  ///////////////////////////////////////////////////// Display
  //lcd.clear();                                // DISPLAY TIME, DATT, YEAR, DATE ON LCD IF NEEDED HERE
  //lcd.setCursor(0,0);
  //lcd.print( temp()/2 );
  //lcd.print(now.hour(), DEC);
  //lcd.setCursor(4,0);
  //lcd.print(now.minute(), DEC);
  //lcd.setCursor(8,0);
  //lcd.print(now.second(), DEC);
  //lcd.setCursor(12,0);
  //lcd.print(now.year(), DEC);
  //lcd.setCursor(0,1);
  //lcd.print(dataMessage.c_str());              // display all log on LCD (to test result) (enable if need to test)

  pressButton();                                 // pressButton() method, click hold for 5 secs and release to go to adjust temp menu,click to go back to main menu.













}                                              // LOOP END



void logSDCard() {                             // LOGSD METHOD, DEFINE HOW DATA SHOULD BE FORMAT AND DISPLAY IN THE FILE
  DateTime now = rtc.now();
  char buf2[] = "YY/MM/DD-hh:mm:ss";           // READ "RTCLIB" LIBARY TO SEE DIFFERENT DATE, TIME TOSTRING() FORMAT
  char buf4[] = "MM-DD-YYYY";                  // READ "RTCLIB" LIBARY TO SEE DIFFERENT DATE, TIME TOSTRING() FORMAT
  dataMessage = String(temp()) + " " +  String(now.toString(buf2));     //////////////change here for the setup of SD Log ( add more oxy, temp, ph level here) exampla: + " " + serial2.read().
  Serial.print("Save data: ");                        //
  Serial.println(dataMessage);                        // CHECK IN SERIAL IF DATA IS CORRECT
  appendFile(SD, "data.txt", dataMessage.c_str());   // APPEND MESSAGE TO THE RIGHT FILE NAME. (CHECK
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
  File file = SD.open("data.txt");
  if (!file) {
    Serial.println("File doens't exist");
    Serial.println("Creating file...");
    writeFile(SD, "data.txt", "Temp, Date \r\n"); //////// Change here for value and set up of Displaying data in SD card
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



void encoder() {                                                                  // ROTARY ENCODER LOGIC, TURN CLOCK WISE OR COUNTER CLOCKWISE CHECK
  currentStateCLK = digitalRead(CLK);                                             // CURRENT STATE OF CLK
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1) {
    if (digitalRead(DT) != currentStateCLK) {
      counter ++;
      currentDir = "CW";
    } else {
      counter --;
      currentDir = "CCW";
    }
    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
  }
  lastStateCLK = currentStateCLK;                                                   // REMEMBER LAST STATE OF CLK
}




void pressButton() {                                                                // WHEN BUTTON IS PRESSED
  int btnState = digitalRead(SW);                                                   // READ BUTTON PIN SIGNAL
  if (btnState == LOW) {                                                            // IF PRESSED
    if (millis() - lastButtonPress > 50) {                                            // DEBOUNCE
//      Serial.println("Button pressed!");
      buttonCounter++;                                                              // BUTTON COUNTER +1 IF BUITTON IS PRESSED
//      Serial.println(buttonCounter);

      if (buttonCounter == 1) {                                                     // IF PRESSED, BUTTON = 0 + 1, JUMP TO ADJUST TEMP MENU
        lcd.clear();
        lcd.print("Adjust Temp  ");

      } else if (buttonCounter == 2) {                                                 // IF PPESSED AGAIN, BUTTON = 1+1 = 2, JUMP TO TEMPERATURE DISPLAY MENU AND RESET BUTTON COUNTER TO 0
        lcd.clear();
        lcd.print("Set Temp");
        lcd.setCursor(10, 0);
        lcd.print(counter);
        lcd.setCursor(0, 1);
        lcd.print("Temp");
        lcd.setCursor(10, 1);
        lcd.print("30");

        buttonCounter = 0;                                                          // BUTTON COUNTER RESET, BUTTONCOUNTER = 0;
      }
    }
    lastButtonPress = millis();                                                     // REMEMBER LAST BUTTON PRESSED EVENT

  }
  else if (buttonCounter == 0 ) {
    int Time = millis()%10000;
   if(Time > (lastLogToSDCard + 5000)%10000)
   {
    logSDCard();                         // IF NOT PRESSED, DATAS WILL KEEP LOGGING ONTO SD CARD, IF PRESSED AND BUTTON COUNTER NOT EQUAL 0, LOG DATA TO SD STOP.
    lastLogToSDCard = Time;
//    delay(5000);             // depend on battery state+++++                         //  DELAY 5 SECS INTERVAL DATA LOGGIN INTO SD, PRESSED AND HOLD ROTARY ENCODER FOR 5 SECS TO GO TO ADJUST TEMP MENU.
   }
  }
  while (buttonCounter == 1) { // using event  {           // LOGIC WHEN BUTTONER COUNTER MENU = 1
    encoder();                             // ENCODER LOGIC, ROTATE CLOCK WISE OR COUNTER CLOCK WISE TO ADJUST TEMP
    lcd.setCursor(12, 0);
    temp_counter = counter;
    lcd.print(temp_counter);
    break;
  }

  while (buttonCounter == 2) { // using event            // BUTTON COUNTER = 2, MAIN DISPLAY MENU
    lcd.clear();
    lcd.print("Set Temp");
    lcd.setCursor(10, 0);
    lcd.print(temp_counter);
    lcd.setCursor(0, 1);
    lcd.print("Temp");
    lcd.setCursor(10, 1);
    lcd.print("30");                        //DATA FROM 1ST MCU WILL BE DISPLAYED HERE ( CHANGE HERE FOR DATA DISPLAY FROM 1ST MCU)
    break;
  }
  //lcd.setCursor(10,0);
  //lcd.print(temp_counter);

  delay(1);                               // SMALL DELAY TO DEBOUNCE THE PRESSED BUTTON, IT IS VERY SENSITIVE


}



//void isr() 
//{
//  esp_sleep_enable_ext0_wakeup(GPIO_NUM_X, level)
//}
