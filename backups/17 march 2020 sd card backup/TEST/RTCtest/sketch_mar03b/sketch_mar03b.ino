#include <SD.h>
#include <SPI.h>
#include <FS.h>
#include <FSImpl.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#define TEMP 34


//////////////////////////////////////////////////////////DEFINE VARIABLES
#define SD_CS 5
LiquidCrystal_I2C lcd( 0x27, 16 ,2 );
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
////////////////////////////////////////////////////////// DEFINE VARIABLES

double SetTemp = 0; // set point of temperature  


File myFile;        // MY FILE OBJECT
//#define CS_RD 12
String dataMessage;

void setup(){
  
SetTemp = 50;
pinMode( TEMP, INPUT );
//pinMode( CS_RD, OUTPUT);

lcd.begin();
lcd.backlight();
Serial.begin( 9600 );


/////////////////////////// RTC BLOCK
rtc.begin();
if (! rtc.begin()) {
Serial.println("Couldn't find RTC");
while (1);

////////////////////////// RTC BLOCK
}
rtc.adjust(DateTime(__DATE__, __TIME__));
}





void loop(){


DateTime now = rtc.now();
 ///////////////////////////////////////////////////// Display
lcd.clear();
lcd.setCursor(0,0);
//lcd.print( temp()/2 );
lcd.print(now.hour(), DEC);

lcd.setCursor(4,0);
lcd.print(now.minute(), DEC);
lcd.setCursor(8,0);
lcd.print(now.second(), DEC);
lcd.setCursor(12,0);
lcd.print(now.year(), DEC);
RTCtest ();
lcd.setCursor(0,1);
lcd.print(dataMessage.c_str()); ///////////////// display all log on LCD 
Serial.println(dataMessage.c_str());
delay(1000);
}




void RTCtest (){
  
DateTime now = rtc.now();
  char buf2[] = "YY/MM/DD - hh:mm:ss";
  char buf4[] = "MM-DD-YYYY";
  dataMessage = String(now.toString(buf2));
}
