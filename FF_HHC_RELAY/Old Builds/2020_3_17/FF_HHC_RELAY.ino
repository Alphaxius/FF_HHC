/*********************************************************************
 * Jacob Barrett, Gregg Dawson, Sothearith Ly, Minh Tri-Truong
 * 
 * FF_HHC_SENSORS
 * Program for relay side board's ESP32 DEVKIT V1
 * 
 * Last updated 16 March 2020
 *********************************************************************/

//#define DEBUG

// INCLUDES
#include <SD.h>
#include <SPI.h>
#include <FS.h>
#include <FSImpl.h>
#include <RTClib.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

// D LATCH PINS
#define A1  33
#define A2  13
#define A3  15
#define G1  25
#define G2  26
#define CLR 27
#define DD  32

// ROTARY ENCODER PINS
#define CLK 14
#define DT 4
#define SW 35                      

// SD CARD PINS
#define SD_CS 5

// TIMING
#define tenMinutes 600000

//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP32CORE_BLE

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_BLUETOOTH_NAME "RemoteXY"

// RemoteXY configurate
// Go to http://remotexy.com/en/editor/bc9747631b4c9b4c1b6edb363e5b8349/ to change the configuration
// such as background, color, and add more configuration.
// Select "Get source code" and copy the code from #pragma pack(push, 1) to #pragma pack(pop)
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,0,0,242,0,219,1,8,16,0,
  67,4,5,3,20,5,2,26,11,67,
  4,28,3,20,5,2,26,11,67,4,
  51,3,20,5,2,26,11,67,4,74,
  3,20,5,2,26,11,67,4,5,12,
  20,5,2,26,11,67,4,28,12,20,
  5,2,26,11,67,4,51,12,20,5,
  2,26,11,67,4,5,21,20,5,2,
  26,11,67,4,28,21,20,5,2,26,
  11,67,4,51,21,20,5,2,26,11,
  67,4,74,21,20,5,2,26,11,67,
  4,5,30,20,5,2,26,11,67,4,
  28,30,20,5,2,26,11,67,4,51,
  30,20,5,2,26,11,67,4,5,39,
  20,5,2,26,11,67,4,28,39,20,
  5,2,26,11,67,4,51,39,20,5,
  2,26,11,67,4,74,39,20,5,2,
  26,11,67,4,5,48,20,5,2,26,
  11,67,4,28,48,20,5,2,26,11,
  67,4,51,48,20,5,2,26,11,67,
  4,5,57,20,5,2,26,11,129,0,
  5,0,9,3,134,84,101,109,112,49,
  0,129,0,28,0,8,3,134,84,101,
  109,112,50,0,129,0,51,0,8,3,
  134,84,101,109,112,51,0,129,0,74,
  0,8,3,134,84,101,109,112,52,0,
  129,0,5,9,8,3,134,84,101,109,
  112,53,0,129,0,28,9,8,3,134,
  84,101,109,112,54,0,129,0,51,9,
  8,3,134,84,101,109,112,55,0,129,
  0,5,18,8,3,134,112,72,95,49,
  0,129,0,28,18,8,3,134,112,72,
  95,50,0,129,0,51,18,8,3,134,
  80,104,95,51,0,129,0,74,18,8,
  3,134,112,72,95,52,0,129,0,5,
  27,8,3,134,112,72,95,53,0,129,
  0,28,27,8,3,134,112,72,95,54,
  0,129,0,51,27,8,3,134,112,72,
  95,55,0,129,0,5,36,8,3,134,
  68,79,95,49,0,129,0,28,36,8,
  3,134,68,79,95,50,0,129,0,51,
  36,8,3,134,68,79,95,51,0,129,
  0,74,36,8,3,134,68,79,95,52,
  0,129,0,5,45,8,3,134,68,79,
  95,53,0,129,0,28,45,8,3,134,
  68,79,95,54,0,129,0,51,45,8,
  3,134,68,79,95,55,0,129,0,5,
  54,8,3,134,66,97,116,116,101,114,
  121,0 };
  
// this structure defines all the variables of your control interface 
struct {
  // output variable
  char Temp_1[11];  // string UTF8 end zero 
  char Temp_2[11];  // string UTF8 end zero 
  char Temp_3[11];  // string UTF8 end zero 
  char Temp_4[11];  // string UTF8 end zero 
  char Temp_5[11];  // string UTF8 end zero 
  char Temp_6[11];  // string UTF8 end zero 
  char Temp_7[11];  // string UTF8 end zero 
  char pH_1[11];  // string UTF8 end zero 
  char pH_2[11];  // string UTF8 end zero 
  char pH_3[11];  // string UTF8 end zero 
  char pH_4[11];  // string UTF8 end zero 
  char pH_5[11];  // string UTF8 end zero 
  char pH_6[11];  // string UTF8 end zero 
  char pH_7[11];  // string UTF8 end zero 
  char DO_1[11];  // string UTF8 end zero 
  char DO_2[11];  // string UTF8 end zero 
  char DO_3[11];  // string UTF8 end zero 
  char DO_4[11];  // string UTF8 end zero 
  char DO_5[11];  // string UTF8 end zero 
  char DO_6[11];  // string UTF8 end zero 
  char DO_7[11];  // string UTF8 end zero 
  char Battery[11];  // string UTF8 end zero 

  // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
///////////////////////////////////////////// 

// input pin structure for generalized use
// Do NOT change default values
struct InputPin
{
  //type: determines how the value should be interpretted
  // 0 = off
  // 1 = analog
  // 2 = digital
  // 3 = ndigital
  // 4 = battery
  // 6 = oneWire
  unsigned char type = 0;

  // What value should this input be at its MINIMUM? Lower than
  // this would indicate that there is a major issue.
  int minValue = 0;

  // What value should this input be at its MAXIMUM? Higher than
  // this would indicate that there is a major issue
  int maxValue = 0;

  // What output should be turned on to INCREASE the value that this input
  // reads? Stores the index of the OutputPins array. -1 indicates not
  // connected. For a digital pin, this is the only connected output
  int outputPinNumberMin = -1;

  // What output should be turned on to DECREASE the value that this input
  // reads? Stores the index of the OutputPins array. -1 indicates not
  // connected.
  int outputPinNumberMax = -1;

  // A place to store the most recently read sensor value
  int value = 0;
};

// output pin structure for generalized use
// Do NOT change default values
struct OutputPin
{
  // is this output enabled? 0 means no, 1 means yes
  byte enable = 0;

  // is this output currently outputting? 0 means no, 1 means yes
  byte outputOn = 0;

  // how much power does this output nominally consume? User should ensure
  // that units are consistent
  int power = 0;
};

// control system variables ////////////////////////////
// generic 20 bit buffer that can be used to read files and read from other mcu
unsigned char comBuffer[20] = {0};
// place to store current location in the buffer
int comBufferIndex = 0;
// input and output arrays, makes keeping track of all the input and output information easier
InputPin inputs[22] = {InputPin()};
OutputPin outputs[16] = {OutputPin()};
// index for inputs array that has the battery value
int batteryInputIndex = 0;
// integer in Watts (could be another unit of power, just be consistent)
// represents how much power the battery system can handle
int powerLimit = 0;
// one more string used to buffer writing to sd files
char writeToBuffer[500] = {0};
// additional buffer for printing to lcd
char lcdBuffer[80] = {0};

// LCD setup
LiquidCrystal_I2C lcd( 0x27, 16 , 2 ); //LCD OBJECT

// real time clock variables
RTC_DS3231 rtc; // RTC OBJECT
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// timing variables
unsigned long prevTime = 0;

/*
 * convert an integer (parameter n) to string
 * store string in parameter str.
 * parameter strsz is the maximum size of strsz
 */
int int2str(int n, char *str, int strsz)
{
  int divisor = 10;
  int strLength = 1;
  while (n % divisor != n && strLength < strsz)
  {
    strLength += 1;
    divisor *= 10;
  }
  for ( int i = 0; i < strLength; i++ )
  {
    divisor /= 10;
    str[i] = 48 + n / divisor;
    n -= divisor * (n / divisor);
  }
  return strLength;
}

/*
  interpret uses the comBuffer to determine what type an input is.
  This is a helper function.
  Parameter length tells this function how long the word in the comBuffer is.
*/
int interpret( int length )
{
  if ( length < 2 ) return -1;
  int sum = comBuffer[0] + comBuffer[1];
  switch ( sum )
  {
    case 131: return 4; // BATTERY
    case 136: return 6; // 1WIRE
    case 141: return 2; // DIGITAL
    case 143: return 1; // ANALOG
    case 146: return 3; // NDIGITAL
    case 149: return 0; // OFF
    case 157: return 5; // ON
    default: return -1; // INCORRECT ENTRY
  }
}

/*
  buf2num uses the comBuffer and assumes that it holds a number
  in ASCII encoding.
  This function returns that number as an integer.
  Parameter length is how long the number is.
*/
int buf2num( int length )
{
  int num = 0;
  int coef = 1;
  for ( int i = 0; i < length; i++ )
  {
    num = num + coef * (comBuffer[length - 1 - i] - 48);
    coef = coef * 10;
  }
  return num;
}

/*
  bufClear sets all the values of the comBuffer to 0
*/
void bufClear()
{
  for ( int i = 0; i < 20; i++ )
  {
    comBuffer[i] = 0;
  }
}

/*
 * lcdBufClear sets all the values of lcdBuffer to 0
 */
void lcdBufClear()
{
  for ( int i = 0; i < 80; i++ )
  {
    lcdBuffer[i] = 0;
  }
}

/*
 * writeToBufClear sets all the values of writeToBuffer to 0
 */
 void writeToBufClear()
{
  for ( int i = 0; i < 500; i++ )
  {
    writeToBuffer[i] = 0;
  }
}

/*
   interpretOutput assumes that the buffer contains the following string: "OUTxx"
   xx is a two digit number (ascii encoding) that should be between 01 and 16
   the function returns the digit as an integer between 0 and 15 corresponding to an
   index of the outputs array
   parameter length is the amount of data to be checked in the buffer. This should be no more
   and no less than 5.
   And the number of the counting shall be five.
*/
int interpretOutput( int length )
{
  if ( length == 5 )
  {
    comBuffer[0] = comBuffer[3];
    comBuffer[1] = comBuffer[4];
    return buf2num( 2 ) - 1;
  }

  return -1;
}

/*
   print the contents of parameter lcdBuffer to the 4x20 lcd
   expects there to be an end of string character (standard c string encoding)
*/
void lcdPrint(const char *lcdBuffer)
{
  lcd.clear();
  int i = 0;
  while ( lcdBuffer[i] != '\0' )
  {
    lcd.setCursor(i-(i / 20)*20+(i/40)*20,(i/20)%2);
    lcd.write(lcdBuffer[i]);
    i++;
  }
}

/*
 * inputNames converts input array indeces to names of the index
 * parameter i is the input index
 * name is stored in parameter str
 * parameter strsz is the maximum size of strsz
 * returns the size of the name
 */
int inputNames( int i, char *str, int strsz )
{
  if ( strsz < 3 ) return 0;
  switch (i)
  {
    case 0: str[0] = 'I'; str[1] = '2'; str[2] = 'C'; str[3] = '0'; return 4;
    case 1: str[0] = 'I'; str[1] = '2'; str[2] = 'C'; str[3] = '1'; return 4;
    case 2: str[0] = 'I'; str[1] = '2'; str[2] = 'C'; str[3] = '2'; return 4;
    case 3: str[0] = 'I'; str[1] = '2'; str[2] = 'C'; str[3] = '3'; return 4;
    case 4: str[0] = 'I'; str[1] = '2'; str[2] = 'C'; str[3] = '4'; return 4;
    case 5: str[0] = 'I'; str[1] = '2'; str[2] = 'C'; str[3] = '5'; return 4;
    case 6: str[0] = 'I'; str[1] = 'N'; str[2] = '0'; return 3;
    case 7: str[0] = 'I'; str[1] = 'N'; str[2] = '1'; return 3;
    case 8: str[0] = 'I'; str[1] = 'N'; str[2] = '2'; return 3;
    case 9: str[0] = 'I'; str[1] = 'N'; str[2] = '3'; return 3;
    case 10: str[0] = 'I'; str[1] = 'N'; str[2] = '4'; return 3;
    case 11: str[0] = 'I'; str[1] = 'N'; str[2] = '5'; return 3;
    case 12: str[0] = 'I'; str[1] = 'N'; str[2] = '6'; return 3;
    case 13: str[0] = 'I'; str[1] = 'N'; str[2] = '7'; return 3;
    case 14: str[0] = 'I'; str[1] = 'N'; str[2] = '8'; return 3;
    case 15: str[0] = 'I'; str[1] = 'N'; str[2] = '9'; return 3;
    case 16: str[0] = 'I'; str[1] = 'N'; str[2] = 'A'; return 3;
    case 17: str[0] = 'I'; str[1] = 'N'; str[2] = 'B'; return 3;
    case 18: str[0] = 'I'; str[1] = 'N'; str[2] = 'C'; return 3;
    case 19: str[0] = 'I'; str[1] = 'N'; str[2] = 'D'; return 3;
    case 20: str[0] = 'I'; str[1] = 'N'; str[2] = 'E'; return 3;
    case 21: str[0] = 'I'; str[1] = 'N'; str[2] = 'F'; return 3;
  }
  return 0;
}

// WRITE TO SD CARD
void writeFile(const char * path, const char * message) {
  File file = SD.open(path, FILE_WRITE);
  if (!file)
  {
    lcdPrint("File could not be opened for writing");
    delay(1000);
    return;
  }
  file.println(message);
  file.close();
}

// APPEND TO SD CARD
void appendFile(const char * path, const char * message)
{
  File file = SD.open(path, FILE_APPEND);
  if (!file)
  {
    lcdPrint("File could not be opened for appending");
    delay(1000);
    return;
  }
  file.println(message);
  file.close();
}

/*
   logSDCard appends all readings to the data.txt file
*/
void logSDCard() {
  writeToBufClear();
  DateTime now = rtc.now();
  char buf2[18] = "YY/MM/DD-hh:mm:ss";
  now.toString(buf2);
  int wtbi = 0;
  for (; wtbi < 17; wtbi++ )
  {
    writeToBuffer[wtbi] = buf2[wtbi];
  }
  writeToBuffer[wtbi++] = '\n';
  for ( int i = 0; i < 22; i++ )
  {
    if ( inputs[i].type == 0 ) continue;
    lcdBufClear();
    int combl = inputNames(i,lcdBuffer,20);
    for ( int j = 0; j < combl; j++ )
    {
      writeToBuffer[wtbi++] = lcdBuffer[j];
    }
    writeToBuffer[wtbi++] = ' ';
    int divisor = 10;
    int strLength = 1;
    int n = inputs[i].value;
    while (n % divisor != n)
    {
      strLength += 1;
      divisor *= 10;
    }
    for ( int j = 0; j < strLength; j++ )
    {
      divisor /= 10;
      writeToBuffer[wtbi++] = 48 + n / divisor;
      n -= divisor * (n / divisor);
    }
    writeToBuffer[wtbi++] = '\n';
  }
  writeToBuffer[wtbi++] = '\n';
  appendFile("/data.txt", writeToBuffer);
}

// CHECK IS SD IS MOUNT OR NOT
void isSdCheck() {
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    lcd.backlight();
    lcdPrint("Err no SD card attached");
    while ( 1 );
  }
}

/*
   dclk1 clocks the first d latch
*/
void dclk1()
{
  digitalWrite(G1, LOW);
  digitalWrite(G1, HIGH);
}

/*
   dclk2 clocks the second d latch
*/
void dclk2()
{
  digitalWrite(G2, LOW);
  digitalWrite(G2, HIGH);
}

/*
   dclr clears all d latches
*/
void dclr()
{
  digitalWrite(CLR, LOW);
  digitalWrite(CLR, HIGH);
}

/*
   daddress sets which address is being accessed
   parameter a is the address as int modulo between 0 and 7
   both d latches share the address bus
*/
void daddress( int a )
{
  switch ( a % 8 )
  {
    case 0:
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
      break;
    case 1:
      digitalWrite(A1, HIGH);
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
      break;
    case 2:
      digitalWrite(A1, LOW);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, LOW);
      break;
    case 3:
      digitalWrite(A1, HIGH);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, LOW);
      break;
    case 4:
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
      digitalWrite(A3, HIGH);
      break;
    case 5:
      digitalWrite(A1, HIGH);
      digitalWrite(A2, LOW);
      digitalWrite(A3, HIGH);
      break;
    case 6:
      digitalWrite(A1, LOW);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, HIGH);
      break;
    case 7:
      digitalWrite(A1, HIGH);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, HIGH);
      break;
  }
}

/*
   dd sets the single bit data bus state
*/
void dd( int d )
{
  digitalWrite(DD, d);
}

/*
   dwrite combines set address, set data, and clock into one function
   d is HIGH or LOW
   a is the address 0 through 15
*/
void dwrite( int d, int a )
{
  daddress(a);
  dd(d);
  if (a < 8) dclk1();
  else dclk2();
}

/*
 * daclr (anti-clear) sets all latches to high
 */
void daclr()
{
  for ( int i = 0; i < 16; i++ )
    dwrite( HIGH, i );
}

/*
   changeOutputPinState turns on or off an output if enable conditions are met.
   Negates logic to account for darlington switch between dlatches and relays
*/
void changeOutputPinState( int address, int state )
{
  if ( outputs[address].enable == 0 ) return;
  else if ( state == 1 ) //negative logic
  {
    outputs[address].outputOn = 1;
    dwrite(LOW, address);
    return;
  }
  else
  {
    outputs[address].outputOn = 0;
    dwrite(HIGH, address);
    return;
  }
} //changeOutputPinState

/*
 * turn off all outputs
 */
void turnOffOutputs()
{
  for ( int i = 0; i < 16; i++ )
  {
    changeOutputPinState(i, 1);
  }
}

/*
   getCurrentNominalPowerUsage returns the total amount of nominal power being used.
*/
int getCurrentNominalPowerUsage()
{
  int powerSum = 0;
  for ( int i = 0; i < 16; i++ )
  {
    if ( (outputs[i].enable == 1) && (outputs[i].outputOn == 1) )
    {
      powerSum = powerSum + outputs[i].power;
    }
  }

  #ifdef DEBUG
    lcdBufClear();
    int2str(powerSum,lcdBuffer,80);
    lcdPrint(lcdBuffer);
    delay(1000);
  #endif
  
  return powerSum;
}

/*
   checkPower determines whether turning on an output would be within the power limit.
   parameter i is the index to the outputs vector which contains the output we want to turn on.
   This will dynamically update the power limit based on the battery charge
*/
byte checkPower( int i )
{
  if ( getCurrentNominalPowerUsage() + outputs[i].power <= powerLimit ) return 1;
  else return 0;
}

/*
 * batteryTimeModify maps the battery level to an amount of time to wait before reading again
 */
int batteryTimeModify()
{
  if ( inputs[batteryInputIndex].value == 0 ) return tenMinutes;
  if ( inputs[batteryInputIndex].value < 12 ) return 3*tenMinutes;
  else if ( inputs[batteryInputIndex].value < 14 ) return tenMinutes;
  else if ( inputs[batteryInputIndex].value < 16 ) return 0.5*tenMinutes;
  else return 0.3*tenMinutes;
}

/*
   updates the inputs array with all current values
*/
void readInputs()
{
  int attemptCount = 0;
  byte readSuccess = false;
  unsigned long t0 = 0;
  unsigned long t1 = 0;
  unsigned long t2 = 0;
  unsigned long t3 = 0;
  for ( unsigned char i = 0; i < 22; i++ )
  {
    // skip if off
    if ( inputs[i].type == 0 ) continue;

    // if last read was a success, reset read_success variables
    if ( readSuccess )
    {
      attemptCount = 0;
      readSuccess = false;
    }

    byte readyToRead = true;

    #ifdef DEBUG
      // print to lcd current read
      lcdBufClear();
      inputNames(i, lcdBuffer, 80);
      lcdPrint(lcdBuffer);
      delay(1000);
    #endif

    // get ready to send data
    bufClear();
    comBuffer[0] = 254;
    comBuffer[1] = 0;
    comBuffer[2] = i;
    comBuffer[3] = inputs[i].type;
    comBuffer[4] = 255;

    #ifdef DEBUG
      Serial.println((int)i);
      Serial.println(attemptCount);
      Serial.print("Sending: ");
      for ( int j = 0; j < 5; j++ )
      {
        Serial.print((int)comBuffer[j]); 
        Serial.print(' ');
      }
      Serial.println();
    #endif

    // make sure there is nothing already waiting in Serial2
    t0 = millis(); //timing variables
    t1 = millis();
    while ( Serial2.available() && (unsigned long)(t1-t0)<60000 )
    {
      t1 = millis();
      Serial.read();
    }
    
    // timout occured?
    if ( (unsigned long)(t1-t0)>=60000 )
    {
      readSuccess = false;
      readyToRead = false;
      #ifdef DEBUG
        lcdPrint("Serial communication is busy");
        delay(1000);
      #endif
    }

    if ( readyToRead )
    {
      // send data
      Serial2.write(comBuffer,5);
      Serial2.flush();

      // reset communication buffer
      bufClear();
      comBufferIndex = 0;

      // wait for response from other mcu
      t0 = millis(); // timing variables
      t1 = millis();
      while (!Serial2.available() && (unsigned long)(t1-t0)<60000)
      { 
        t1 = millis();
      }
      // timeout occured
      if (((unsigned long)(t1-t0))>=60000)
      {
        readSuccess = false;
        readyToRead = false;
        writeToBufClear();
        writeToBuffer[0] = 'N';
        writeToBuffer[1] = 'o';
        writeToBuffer[2] = ' ';
        writeToBuffer[3] = 'r';
        writeToBuffer[4] = 'e';
        writeToBuffer[5] = 's';
        writeToBuffer[6] = 'p';
        writeToBuffer[7] = 'o';
        writeToBuffer[8] = 'n';
        writeToBuffer[9] = 's';
        writeToBuffer[10] = 'e';
        writeToBuffer[11] = ' ';
        writeToBuffer[12] = 'a';
        writeToBuffer[13] = 't';
        writeToBuffer[14] = ' ';
        DateTime now = rtc.now();
        char buf2[18] = "YY/MM/DD-hh:mm:ss";
        now.toString(buf2);
        for ( int i = 0; i < 17; i++ )
        {
          writeToBuffer[15+i]=buf2[i];
        }
        writeToBuffer[32] = ' ';
        writeToBuffer[33] = 'w';
        writeToBuffer[34] = 'h';
        writeToBuffer[35] = 'i';
        writeToBuffer[36] = 'l';
        writeToBuffer[37] = 'e';
        writeToBuffer[38] = ' ';
        writeToBuffer[39] = 'r';
        writeToBuffer[40] = 'e';
        writeToBuffer[41] = 'a';
        writeToBuffer[42] = 'd';
        writeToBuffer[43] = 'i';
        writeToBuffer[44] = 'n';
        writeToBuffer[45] = 'g';
        writeToBuffer[46] = ' ';
        lcdBufClear();
        int nameLength = inputNames(i, lcdBuffer, 80);
        for ( int j = 0; j < nameLength; j++ )
        {
          writeToBuffer[47+j]=lcdBuffer[i];
        }
        writeToBuffer[47+nameLength] = '\n';
        appendFile("\error.txt",writeToBuffer);
        
        #ifdef DEBUG
          lcdPrint("read timeout, no reply");
          delay(1000);
        #endif
      }
    }

    if ( readyToRead )
    {
      // reply is ready - begin reading
      t0 = millis(); // timing variables
      t1 = millis();
      unsigned char incomingByte = 0; // temporary byte storage
      byte comBegun = false;
      while ( (unsigned long)(t1-t0)<60000 )
      {
        // just in case reply stops in the middle
        t2 = millis();
        t3 = millis();
        while ( !Serial2.available() && (unsigned long)(t3-t2)<60000 )
        {
          t3 = millis();
        }
        if ( (unsigned long)(t3-t2)>=60000 )
        {
          // this will bring us to the incomplete timeout condition
          t1 = millis();
          break;
        }
        
        incomingByte = Serial2.read();
  
        #ifdef DEBUG
          Serial.println((int)incomingByte);
        #endif
  
        // too many incoming bytes
        if ( comBufferIndex == 20 )
        {
          readSuccess = false;
          writeToBufClear();
          writeToBuffer[0] = 'R';
          writeToBuffer[1] = 'e';
          writeToBuffer[2] = 'c';
          writeToBuffer[3] = 'e';
          writeToBuffer[4] = 'i';
          writeToBuffer[5] = 'v';
          writeToBuffer[6] = 'e';
          writeToBuffer[7] = 'd';
          writeToBuffer[8] = ' ';
          writeToBuffer[9] = 't';
          writeToBuffer[10] = 'o';
          writeToBuffer[11] = 'o';
          writeToBuffer[12] = ' ';
          writeToBuffer[13] = 'm';
          writeToBuffer[14] = 'a';
          writeToBuffer[15] = 'n';
          writeToBuffer[16] = 'y';
          writeToBuffer[17] = ' ';
          writeToBuffer[18] = 'b';
          writeToBuffer[19] = 'y';
          writeToBuffer[20] = 't';
          writeToBuffer[21] = 'e';
          writeToBuffer[22] = 's';
          writeToBuffer[23] = ' ';
          writeToBuffer[24] = 'a';
          writeToBuffer[25] = 't';
          writeToBuffer[26] = ' ';
          DateTime now = rtc.now();
          char buf2[18] = "YY/MM/DD-hh:mm:ss";
          now.toString(buf2);
          for ( int i = 0; i < 17; i++ )
          {
            writeToBuffer[27+i]=buf2[i];
          }
          writeToBuffer[44] = ' ';
          writeToBuffer[45] = 'w';
          writeToBuffer[46] = 'h';
          writeToBuffer[47] = 'i';
          writeToBuffer[48] = 'l';
          writeToBuffer[49] = 'e';
          writeToBuffer[50] = ' ';
          writeToBuffer[51] = 'r';
          writeToBuffer[52] = 'e';
          writeToBuffer[53] = 'a';
          writeToBuffer[54] = 'd';
          writeToBuffer[55] = 'i';
          writeToBuffer[56] = 'n';
          writeToBuffer[57] = 'g';
          writeToBuffer[58] = ' ';
          lcdBufClear();
          int nameLength = inputNames(i, lcdBuffer, 80);
          for ( int j = 0; j < nameLength; j++ )
          {
            writeToBuffer[59+j]=lcdBuffer[i];
          }
          writeToBuffer[59+nameLength] = '\n';
          appendFile("\error.txt",writeToBuffer);
          
          #ifdef DEBUG
            lcdPrint("err too many bytes recieved");
            delay(1000);
          #endif
          
          break;
        }
  
        // incoming byte is inside 254 and 255
        else if ( incomingByte != 254 && incomingByte != 255 && comBegun )
        {
          
          // 253 is error code from the other mcu
          if ( incomingByte == 253 )
          {
            readSuccess = false;
            writeToBufClear();
            writeToBuffer[0] = 'R';
            writeToBuffer[1] = 'e';
            writeToBuffer[2] = 'c';
            writeToBuffer[3] = 'e';
            writeToBuffer[4] = 'i';
            writeToBuffer[5] = 'v';
            writeToBuffer[6] = 'e';
            writeToBuffer[7] = 'd';
            writeToBuffer[8] = ' ';
            writeToBuffer[9] = 'e';
            writeToBuffer[10] = 'r';
            writeToBuffer[11] = 'r';
            writeToBuffer[12] = 'o';
            writeToBuffer[13] = 'r';
            writeToBuffer[14] = ' ';
            writeToBuffer[15] = 'c';
            writeToBuffer[16] = 'o';
            writeToBuffer[17] = 'd';
            writeToBuffer[18] = 'e';
            writeToBuffer[19] = ' ';
            writeToBuffer[20] = 'a';
            writeToBuffer[21] = 't';
            writeToBuffer[22] = ' ';
            DateTime now = rtc.now();
            char buf2[18] = "YY/MM/DD-hh:mm:ss";
            now.toString(buf2);
            for ( int i = 0; i < 17; i++ )
            {
              writeToBuffer[23+i]=buf2[i];
            }
            writeToBuffer[40] = ' ';
            writeToBuffer[41] = 'w';
            writeToBuffer[42] = 'h';
            writeToBuffer[43] = 'i';
            writeToBuffer[44] = 'l';
            writeToBuffer[45] = 'e';
            writeToBuffer[46] = ' ';
            writeToBuffer[47] = 'r';
            writeToBuffer[48] = 'e';
            writeToBuffer[49] = 'a';
            writeToBuffer[50] = 'd';
            writeToBuffer[51] = 'i';
            writeToBuffer[52] = 'n';
            writeToBuffer[53] = 'g';
            writeToBuffer[54] = ' ';
            lcdBufClear();
            int nameLength = inputNames(i, lcdBuffer, 80);
            for ( int j = 0; j < nameLength; j++ )
            {
              writeToBuffer[55+j]=lcdBuffer[i];
            }
            writeToBuffer[55+nameLength] = '\n';
            appendFile("\error.txt",writeToBuffer);
      
            #ifdef DEBUG
              lcdPrint("err code received");
              delay(1000);
            #endif
            
            break;
          }
  
          // incoming byte is a number
          else if ( incomingByte < 58 && incomingByte > 47 )
          {
            comBuffer[comBufferIndex++] = incomingByte;
          }
  
          // incoming byte is invalid
          else
          {
            readSuccess = false;
            writeToBufClear();
            writeToBuffer[0] = 'R';
            writeToBuffer[1] = 'e';
            writeToBuffer[2] = 'c';
            writeToBuffer[3] = 'e';
            writeToBuffer[4] = 'i';
            writeToBuffer[5] = 'v';
            writeToBuffer[6] = 'e';
            writeToBuffer[7] = 'd';
            writeToBuffer[8] = ' ';
            writeToBuffer[9] = 'n';
            writeToBuffer[10] = 'o';
            writeToBuffer[11] = 'n';
            writeToBuffer[12] = '-';
            writeToBuffer[13] = 'n';
            writeToBuffer[14] = 'u';
            writeToBuffer[15] = 'm';
            writeToBuffer[16] = 'b';
            writeToBuffer[17] = 'e';
            writeToBuffer[18] = 'r';
            writeToBuffer[19] = ' ';
            writeToBuffer[20] = 'a';
            writeToBuffer[21] = 't';
            writeToBuffer[22] = ' ';
            DateTime now = rtc.now();
            char buf2[18] = "YY/MM/DD-hh:mm:ss";
            now.toString(buf2);
            for ( int i = 0; i < 17; i++ )
            {
              writeToBuffer[23+i]=buf2[i];
            }
            writeToBuffer[40] = ' ';
            writeToBuffer[41] = 'w';
            writeToBuffer[42] = 'h';
            writeToBuffer[43] = 'i';
            writeToBuffer[44] = 'l';
            writeToBuffer[45] = 'e';
            writeToBuffer[46] = ' ';
            writeToBuffer[47] = 'r';
            writeToBuffer[48] = 'e';
            writeToBuffer[49] = 'a';
            writeToBuffer[50] = 'd';
            writeToBuffer[51] = 'i';
            writeToBuffer[52] = 'n';
            writeToBuffer[53] = 'g';
            writeToBuffer[54] = ' ';
            lcdBufClear();
            int nameLength = inputNames(i, lcdBuffer, 80);
            for ( int j = 0; j < nameLength; j++ )
            {
              writeToBuffer[55+j]=lcdBuffer[i];
            }
            writeToBuffer[55+nameLength] = '\n';
            appendFile("\error.txt",writeToBuffer);
            
            #ifdef DEBUG
              lcdPrint("non-number received");
              delay(1000);
            #endif
            
            break;
          }
          
        }
  
        // 254 is start byte
        else if ( incomingByte == 254 )
        {
          comBegun = true;
          continue; // begin read
        }
  
        // 255 is end byte
        else if ( incomingByte == 255 && comBegun ) // end read
        {
          readSuccess = true;
          while(Serial2.available());
          break;
        }
  
        // get new time
        t1 = millis();
      } // while loop, read next byte
      
      // timout occured?
      if ( (unsigned long)(t1-t0)>=60000 )
      {
        writeToBufClear();
        writeToBuffer[0] = 'I';
        writeToBuffer[1] = 'n';
        writeToBuffer[2] = 'c';
        writeToBuffer[3] = 'o';
        writeToBuffer[4] = 'm';
        writeToBuffer[5] = 'p';
        writeToBuffer[6] = 'l';
        writeToBuffer[7] = 'e';
        writeToBuffer[8] = 't';
        writeToBuffer[9] = 'e';
        writeToBuffer[10] = ' ';
        writeToBuffer[11] = 'r';
        writeToBuffer[12] = 'e';
        writeToBuffer[13] = 's';
        writeToBuffer[14] = 'p';
        writeToBuffer[15] = 'o';
        writeToBuffer[16] = 'n';
        writeToBuffer[17] = 's';
        writeToBuffer[18] = 'e';
        writeToBuffer[19] = ' ';
        writeToBuffer[20] = 'a';
        writeToBuffer[21] = 't';
        writeToBuffer[22] = ' ';
        DateTime now = rtc.now();
        char buf2[18] = "YY/MM/DD-hh:mm:ss";
        now.toString(buf2);
        for ( int i = 0; i < 17; i++ )
        {
          writeToBuffer[23+i]=buf2[i];
        }
        writeToBuffer[40] = ' ';
        writeToBuffer[41] = 'w';
        writeToBuffer[42] = 'h';
        writeToBuffer[43] = 'i';
        writeToBuffer[44] = 'l';
        writeToBuffer[45] = 'e';
        writeToBuffer[46] = ' ';
        writeToBuffer[47] = 'r';
        writeToBuffer[48] = 'e';
        writeToBuffer[49] = 'a';
        writeToBuffer[50] = 'd';
        writeToBuffer[51] = 'i';
        writeToBuffer[52] = 'n';
        writeToBuffer[53] = 'g';
        writeToBuffer[54] = ' ';
        lcdBufClear();
        int nameLength = inputNames(i, lcdBuffer, 80);
        for ( int j = 0; j < nameLength; j++ )
        {
          writeToBuffer[55+j]=lcdBuffer[i];
        }
        writeToBuffer[55+nameLength] = '\n';
        appendFile("\error.txt",writeToBuffer);
        
        #ifdef DEBUG
          lcdPrint("read timeout, incomplete receipt");
          delay(1000);
        #endif
        
        readSuccess = false;
      }
    } // end if ready to read
    
    // did any of the errors occur?
    if ( !readSuccess && !readyToRead )
    {
      // if already tried 5 times, try the next input
      if ( attemptCount > 3 ) // five times (0 1 2 3 _4_ ) triggers on 4 (5th)
      {
        
        #ifdef DEBUG
          lcdPrint("failed to read from input");
          delay(1000);
        #endif
        
        readSuccess = true; // this is not actually true but we want to skip this one since it is apparently not working
        continue;
      }

      // otherwise try again
      else
      {
        i--;
        attemptCount++;
        continue;
      }
    }

    // read was a success!
    else
    {
      #ifdef DEBUG
        Serial.print("Received: ");
        for ( int j = 0; j < comBufferIndex; j++ )
        {
          Serial.print((int)comBuffer[j]);
          Serial.print(' ');
        }
        Serial.println();
      #endif

      // save the incoming data
      inputs[i].value = buf2num( comBufferIndex );
    }

  } // for loop, next input
} //readInputs

/*
  scoreInput returns a value between 0 and 1 or greater.
  A value greater than 1 may indicate a problem.
  This value tells how important it is for action to be taken.
  If an input is connected to an output, another function may turn on said
  output based on the number that this function returns.
  For a digital pin, this returns 1 when active and 0 when inactive
*/
float scoreInput(int inputID)
{
  if ( inputs[inputID].type == 1 || inputs[inputID].type == 6 )
  {
    float score = 0.5 - (float)(inputs[inputID].value - inputs[inputID].minValue) / (float)(inputs[inputID].maxValue - inputs[inputID].minValue);
    score = 4 * score * score;
    return score;
  }
  else if ( inputs[inputID].type == 2 )
  {
    return inputs[inputID].value;
  }
  else if ( inputs[inputID].type == 3 )
  {
    return 1 - inputs[inputID].value;
  }
  else
  {
    return 0;
  }
} // scoreInput

/*
  output attempts to set an output of a given input pin using its current value.
  If the value is less than 3/8 between its min and max, it will turn on
  outputPinNumberMin. If the value is greater than 5/8 between its min and max,
  it will turn on outputPinNumberMax
  e.g. if min=100, max=300 and value<175, and if outputPinNumberMin is set,
  then this function would turn on outputPinNumberMin. Conversely, if min
  and max are the same, but value>225 and outputPinNumberMax is set, then
  this function would turn on outputPinNumberMax
  For a digital input, if it is active, then it will turn on the connected output
  If an output was successfully turned on, this function returns 1. Otherwise it
  returns 0.
*/

byte output(int inputID)
{
  // case that input is not connected to any outputs
  if ( inputs[inputID].outputPinNumberMin == -1 && inputs[inputID].outputPinNumberMax == -1 )
  {
    #ifdef DEBUG
      lcdPrint("no connected outputs!");
      delay(1000);
    #endif
    
    return 0;
  }

  // type is analog
  if ( inputs[inputID].type == 1 || inputs[inputID].type == 6 )
  {
    
    #ifdef DEBUG
      lcdPrint("analog or oneWire");
      delay(1000);
    #endif
    
    // case that input is low enough to turn on its corresponding output
    // as long as the corresponding output exists
    float threshold = 0.625 * inputs[inputID].minValue + 0.375 * inputs[inputID].maxValue;
    if ( inputs[inputID].value < threshold && inputs[inputID].outputPinNumberMin != -1 && checkPower(inputs[inputID].outputPinNumberMin) )
    {
      
      #ifdef DEBUG
        lcdPrint("attempting to change output (min)");
        delay(1000);
      #endif
      
      changeOutputPinState(inputs[inputID].outputPinNumberMin, 1);
      return 1;
    }

    // case that input is high enough to turn on its corresponding output
    // as long as the corresponding output exists
    threshold = 0.375 * inputs[inputID].minValue + 0.675 * inputs[inputID].maxValue;
    if ( inputs[inputID].value > threshold && inputs[inputID].outputPinNumberMax != -1 && checkPower(inputs[inputID].outputPinNumberMax) )
    {
      
      #ifdef DEBUG
        lcdPrint("attempting to change output (max)");
        delay(1000);
      #endif
      
      changeOutputPinState(inputs[inputID].outputPinNumberMax, 1);
      return 1;
    }

    // case that direction of input does not match direction of output
    
    #ifdef DEBUG
      lcdPrint("cannot change corresponding output");
      delay(1000);
    #endif
    
    return 0;
  }

  else if ( inputs[inputID].type == 2 || inputs[inputID].type == 3 )
  {
    
    #ifdef DEBUG
      lcdPrint("digital or ndigital");
      delay(1000);
    #endif
    
    if ( inputs[inputID].outputPinNumberMin != -1 && checkPower(inputs[inputID].outputPinNumberMin))
    {
    
      #ifdef DEBUG
        lcdPrint("attempting to change output (digital)");
        delay(1000);
      #endif
      
      changeOutputPinState(inputs[inputID].outputPinNumberMin, 1);
      return 1;
    }
    
    #ifdef DEBUG
      lcdPrint("cannot change corresponding output");
      delay(1000);
    #endif
      
    return 0;
  }

  #ifdef DEBUG
    lcdPrint("some other problem?");
    delay(1000);
  #endif
  
  return 0;
} // output

/*
  turnOnOutputs is the logic which decides which outputs should be turned on
  based on scores returned by scoreInput
*/
void turnOnOutputs()
{
  int triedIndeces[22] = {0}; // holds previously used indeces
  for ( int triedIndecesIndex = 0; triedIndecesIndex < 22; triedIndecesIndex++ )
  {
    #ifdef DEBUG
      lcdPrint("triedIndecesIndex =");
      delay(1000);
      lcdBufClear();
      int2str(triedIndecesIndex,lcdBuffer,80);
      lcdPrint(lcdBuffer);
      delay(1000);
    #endif
    int highestScoringInputIndex = 0; // that hasn't yet been used and is the correct type
    byte foundInitial = false;
    for ( int indexToTryInitial = 0; indexToTryInitial < 22; indexToTryInitial++ )
    {
      if ( inputs[indexToTryInitial].type == 0 || inputs[indexToTryInitial].type == 4 )
      {
        // wrong type
        continue;
      }
      byte alreadyUsed = false;
      for ( int prevIndex = 0; prevIndex < triedIndecesIndex; prevIndex++ )
      {
        if ( triedIndeces[prevIndex] == indexToTryInitial )
        {
          // we have already used indexToTryInitial
          alreadyUsed = true;
          break;
        }
      }
      if ( !alreadyUsed )
      {
        highestScoringInputIndex = indexToTryInitial;
        foundInitial = true;
        break;
      }
    }
    if ( !foundInitial )
    {
      break;
    }
    for ( int inputsIndexToTry = 0; inputsIndexToTry < 22; inputsIndexToTry++ )
    {
      // check that type is not 0 or 4
      if ( inputs[inputsIndexToTry].type == 0 || inputs[inputsIndexToTry].type == 4 )
      {
        // wrong type
        continue;
      }
      #ifdef DEBUG
        lcdPrint("trying input index");
        delay(1000);
        lcdBufClear();
        int2str(inputsIndexToTry,lcdBuffer,80);
        lcdPrint(lcdBuffer);
        delay(1000);
      #endif
      // check that it has not been used before
      byte alreadyUsed = false;
      for ( int prevIndex = 0; prevIndex < triedIndecesIndex; prevIndex++ )
      {
          if ( triedIndeces[prevIndex] == inputsIndexToTry )
          {
            // we have already used inputsIndexToTry
            alreadyUsed = true;
            break;
          }
      }
      if ( alreadyUsed )
      {
        #ifdef DEBUG
          lcdPrint("already used");
          delay(1000);
        #endif
        continue;
      }
      // check that score is bigger
      if ( scoreInput(inputsIndexToTry) > scoreInput(highestScoringInputIndex) )
      {
        #ifdef DEBUG
          lcdPrint("it is bigger");
          delay(1000);
        #endif
        highestScoringInputIndex = inputsIndexToTry;
      }
    }
    #ifdef DEBUG
      lcdPrint("biggest input found is");
      delay(1000);
      lcdBufClear();
      int2str(highestScoringInputIndex,lcdBuffer,80);
      lcdPrint(lcdBuffer);
      delay(1000);
    #endif
    triedIndeces[triedIndecesIndex] = highestScoringInputIndex;

    // temporarily store the maximum score
    float score = scoreInput(triedIndeces[triedIndecesIndex]);

    #ifdef DEBUG
      lcdBufClear();
      inputNames(highestScoringInputIndex,lcdBuffer,80);
      Serial.println(lcdBuffer);
      Serial.println(score);
    #endif
    // if the maximum is too low, all done
    // all inputs are regular and no action needs to be taken
    if ( score < 0.08 )
    {
      
      #ifdef DEBUG
        lcdPrint("output is too low");
        delay(1000);
      #endif
      
      break;
    }

    // if the maximum is too high, problem indicated!
    else if ( score > 1 )
    {
      lcdBufClear();
      writeToBufClear();
      int nameLength = inputNames(triedIndeces[triedIndecesIndex],lcdBuffer,80);
      for ( int i = 0; i < nameLength; i++ )
      {
        writeToBuffer[i] = lcdBuffer[i];
      }
      writeToBuffer[nameLength] = ' ';
      writeToBuffer[nameLength+1] = 'o';
      writeToBuffer[nameLength+2] = 'u';
      writeToBuffer[nameLength+3] = 't';
      writeToBuffer[nameLength+4] = ' ';
      writeToBuffer[nameLength+5] = 'o';
      writeToBuffer[nameLength+6] = 'f';
      writeToBuffer[nameLength+7] = ' ';
      writeToBuffer[nameLength+8] = 'b';
      writeToBuffer[nameLength+9] = 'o';
      writeToBuffer[nameLength+10] = 'u';
      writeToBuffer[nameLength+11] = 'n';
      writeToBuffer[nameLength+12] = 'd';
      writeToBuffer[nameLength+13] = 's';
      writeToBuffer[nameLength+14] = ' ';
      writeToBuffer[nameLength+15] = 'a';
      writeToBuffer[nameLength+16] = 't';
      writeToBuffer[nameLength+17] = ' ';
      DateTime now = rtc.now();
      char buf2[18] = "YY/MM/DD-hh:mm:ss";
      now.toString(buf2);
      for ( int i = 0; i < 17; i++ )
      {
        writeToBuffer[nameLength+18+i]=buf2[i];
      }
      writeToBuffer[nameLength+35] = ' ';
      writeToBuffer[nameLength+36] = 'w';
      writeToBuffer[nameLength+37] = 'i';
      writeToBuffer[nameLength+38] = 't';
      writeToBuffer[nameLength+39] = 'h';
      writeToBuffer[nameLength+40] = ' ';
      lcdBufClear();
      int dataLength = int2str(inputs[triedIndeces[triedIndecesIndex]].value,lcdBuffer,80);
      for ( int i = 0; i < dataLength; i++ )
      {
        writeToBuffer[nameLength+41+i]=lcdBuffer[i];
      }
      writeToBuffer[nameLength+dataLength+41]='\n';
      appendFile( "/errors.txt", writeToBuffer );

      #ifdef DEBUG
        lcdPrint(writeToBuffer);
        delay(1000);
      #endif
    }
    
    // action needs to be taken
    if ( output( triedIndeces[triedIndecesIndex] ) == 1 )
    {
      // output was successful
      #ifdef DEBUG
        lcdPrint("output successful");
        delay(1000);
      #endif
    }
    else
    {
      #ifdef DEBUG
        lcdPrint("output unsuccessful");
        delay(1000);
      #endif
    }
  } // for loop triedIndecesIndex
} //turnOnOutputs

/*
   i2cSetupAddress reads the data from the settings file and sends the i2c address to the other mcu
   for the other mcu to store and use when reading the corresponding sensor.
*/
void i2cSetupAddress(File *settingsFile, int inputsIndex)
{
  bufClear();
  comBufferIndex = 0;
  while ( settingsFile->available() )
  {
    comBuffer[comBufferIndex] = settingsFile->read();
    if ( comBuffer[comBufferIndex] == ',' || comBuffer[comBufferIndex] == ';'  )
    {
      
      int address = buf2num( comBufferIndex );

      // check if address is too big
      if ( address > 127 || address < 0 )
      {
        lcdBufClear();
        int2str(address,lcdBuffer,80);
        while (1)
        {
          lcdPrint("invalid i2c address");
          delay(1500);
          lcdPrint(lcdBuffer);
          delay(1500);
        }
      }
      else
      {
        bufClear();
        comBuffer[0] = 254;
        comBuffer[1] = 1;
        comBuffer[2] = inputsIndex;
        comBuffer[3] = address;
        comBuffer[4] = 255;
        Serial2.write(comBuffer,5);
      }

      break;
    }
    comBufferIndex = comBufferIndex + 1;
  }
}

/*
   inputSetupMin reads data from the settings file and sets up the minimum value
   of that input
*/
void inputSetupMin(File *settingsFile, int inputsIndex)
{
  bufClear();
  comBufferIndex = 0;
  while ( settingsFile->available() )
  {
    comBuffer[comBufferIndex] = settingsFile->read();
    if ( comBuffer[comBufferIndex] == ',' || comBuffer[comBufferIndex] == ';' )
    {
      inputs[inputsIndex].minValue = buf2num( comBufferIndex );
      
      #ifdef DEBUG
        lcdBufClear();
        int2str(inputs[inputsIndex].minValue, lcdBuffer, 80);
        lcdPrint(lcdBuffer);
        delay(1000);
      #endif
      
      break;
    }
    comBufferIndex++;
  }
}

/*
   inputSetupMax reads data from the settings file and sets up the maximum value
   of that input
*/
void inputSetupMax(File *settingsFile, int inputsIndex)
{
  bufClear();
  comBufferIndex = 0;
  while ( settingsFile->available() )
  {
    comBuffer[comBufferIndex] = settingsFile->read();
    if ( comBuffer[comBufferIndex] == ',' || comBuffer[comBufferIndex] == ';' )
    {
      inputs[inputsIndex].maxValue = buf2num( comBufferIndex );
      
      #ifdef DEBUG
        lcdBufClear();
        int2str(inputs[inputsIndex].maxValue, lcdBuffer, 80);
        lcdPrint(lcdBuffer);
        delay(1000);
      #endif
      
      break;
    }
    comBufferIndex++;
  }
}

/*
   inputSetupOutputMin reads data from the settings file and sets up the output
   which will be turned on when the value of the sensor is too low.
*/
void inputSetupOutputMin(File *settingsFile, int inputsIndex)
{
  bufClear();
  comBufferIndex = 0;
  while ( settingsFile->available() )
  {
    comBuffer[comBufferIndex] = settingsFile->read();
    if ( comBuffer[comBufferIndex] == ',' || comBuffer[comBufferIndex] == ';' )
    {
      inputs[inputsIndex].outputPinNumberMin = interpretOutput( comBufferIndex );
      
      #ifdef DEBUG
        lcdBufClear();
        int2str(inputs[inputsIndex].outputPinNumberMin, lcdBuffer, 80);
        lcdPrint(lcdBuffer);
        delay(1000);
      #endif

      break;
    }
    comBufferIndex++;
  }
}


/*
   inputSetupOutputMax reads data from the settings file and sets up the output
   which will be turned on when the value of the sensor is too high.
*/
void inputSetupOutputMax(File *settingsFile, int inputsIndex)
{
  bufClear();
  comBufferIndex = 0;
  while ( settingsFile->available() )
  {
    comBuffer[comBufferIndex] = settingsFile->read();
    if ( comBuffer[comBufferIndex] == ',' || comBuffer[comBufferIndex] == ';' )
    {
      inputs[inputsIndex].outputPinNumberMax = interpretOutput( comBufferIndex );
      
      #ifdef DEBUG
        lcdBufClear();
        int2str(inputs[inputsIndex].outputPinNumberMax, lcdBuffer, 80);
        lcdPrint(lcdBuffer);
        delay(1000);
      #endif
      
      break;
    }
    comBufferIndex++;
  }
}

/*
   i2cSetup sets up an i2c input
*/
void i2cSetup(File *settingsFile,  int i)
{
  inputs[i].type = 1;

  i2cSetupAddress( settingsFile, i );
  inputSetupMin( settingsFile, i );
  inputSetupMax( settingsFile, i );
  inputSetupOutputMin( settingsFile, i );
  inputSetupOutputMax( settingsFile, i );
}

/*
   oneWireSetup sets up a 1-Wire input
*/
void oneWireSetup( File *settingsFile, int i )
{
  inputs[i].type = 6;

  inputSetupMin( settingsFile, i );
  inputSetupMax( settingsFile, i );
  inputSetupOutputMin( settingsFile, i );
  inputSetupOutputMax( settingsFile, i );
}

/*
   analogSetup sets up an analog input
*/
void analogSetup( File *settingsFile, int i )
{
  inputs[i].type = 1;

  inputSetupMin( settingsFile, i );
  inputSetupMax( settingsFile, i );
  inputSetupOutputMin( settingsFile, i );
  inputSetupOutputMax( settingsFile, i );
}

/*
   digitalSetup sets up a positive logic digital input
*/
void digitalSetup( File *settingsFile, int i )
{
  inputs[i].type = 2;

  inputs[i].minValue = 0;
  inputs[i].maxValue = 1;
  inputSetupOutputMin( settingsFile, i );
}

/*
   ndigitalSetup sets up a negative logic input
*/
void ndigitalSetup( File *settingsFile, int i )
{
  inputs[i].type = 3;

  inputs[i].minValue = 1;
  inputs[i].maxValue = 0;
  inputSetupOutputMin( settingsFile, i );
}

/*
   set up an output pin
*/
void outputSetup( File *file, int i )
{
  outputs[i].enable = 1;
  bufClear();
  comBufferIndex = 0;
  while ( file->available() )
  {
    comBuffer[comBufferIndex] = file->read();
    if ( comBuffer[comBufferIndex] == ',' || comBuffer[comBufferIndex] == ';' )
    {
      outputs[i].power = buf2num( comBufferIndex );

      #ifdef DEBUG
        lcdBufClear();
        int2str(outputs[i].power, lcdBuffer, 80);
        lcdPrint(lcdBuffer);
        delay(1000);
      #endif
      
      break;
    }
    comBufferIndex = comBufferIndex + 1;
  }
}

// ROTARY ENCODER LOGIC, TURN CLOCK WISE OR COUNTER CLOCKWISE CHECK
/*
 * encoder reads the state of the encoder and either increments, decrements, or leaves the counter
 * the same
 */
byte encoder(byte counter) {
  byte clk = 0;
  byte dt = 0;
  byte rotaryEncoderIsMoving = false;
  byte clockwise = false;
  clk = digitalRead(CLK);
  dt = digitalRead(DT);
  if ( clk && dt )
  {
    return counter;
  }
  else if ( !clk && dt ) // clockwise
  {
    rotaryEncoderIsMoving = true;
    clockwise = true;
  }
  else if ( clk && !dt ) //counterclockwise
  {
    rotaryEncoderIsMoving = true;
    clockwise = false;
  }
  if ( rotaryEncoderIsMoving )
  {
    if ( clockwise )
    {
      counter++;
    }
    else
    {
      counter--;
    }
    // debounce
    int debounce = 0;
    while ( debounce < 1000 )
    {
      if ( digitalRead(DT) && digitalRead(CLK) )
      {
        debounce++;
      }
    }
  }
  return counter;
}

/*
 * loop using the encoder to show current values of all inputs.
 */
void lcdShowReadings()
{
  int previousSelect = 0;
  int inputSelect = 0;
  while ( !digitalRead(SW) );
  while ( digitalRead(SW) )
  {
    inputSelect = encoder( inputSelect );
    if ( inputSelect > 50 ) inputSelect = 21;
    else if ( inputSelect > 21 ) inputSelect = 0;
    if ( inputSelect != previousSelect )
    {
      lcdBufClear();
      int2str(inputs[inputSelect].value, lcdBuffer, 20);
      for ( int i = 0; i < 20; i++ )
      {
        lcdBuffer[i+21] = lcdBuffer[i];
        lcdBuffer[i] = ' ';
      }
      lcdBuffer[20] = ' ';
      inputNames( inputSelect, lcdBuffer, 20 );
      lcdPrint(lcdBuffer);
    }
    previousSelect = inputSelect;
  }
}

/*
 * loop using encoder to show settings
 * Allows the user to change simple parameters without resetting the system
 * Does not save settings to SD card
 */
void lcdShowInputSettings()
{
  int inputSelect = 0;
  int settingsSelect = 0;
  uint16_t previous = 0;
  uint16_t value = 0;
  while ( !digitalRead(SW) );
  while ( digitalRead(SW) )
  {
    // TODO: does not show input on start
    inputSelect = encoder( inputSelect );
    if ( inputSelect > 50 ) inputSelect = 21;
    else if ( inputSelect > 21 ) inputSelect = 0;
    if ( inputSelect != previous )
    {
      lcdBufClear();
      inputNames(inputSelect, lcdBuffer, 80);
      lcdPrint(lcdBuffer);
    }
    previous = inputSelect;
  }
  switch (inputs[inputSelect].type)
  {
    case 6:
      previous = 0;
      settingsSelect = 0;
      while ( !digitalRead(SW) );
      while ( digitalRead(SW) )
      {
      settingsSelect = encoder( settingsSelect );
      if ( settingsSelect > 10 ) settingsSelect = 1;
      else if ( settingsSelect > 1 ) settingsSelect = 0;
        if ( settingsSelect != previous )
        {
          if ( settingsSelect == 0 )
          {
            lcdPrint("min value");
          }
          else if ( settingsSelect == 1 )
          {
            lcdPrint("max value");
          }
        }
        previous = settingsSelect;
      }
      value = 0;
      if ( settingsSelect == 0 )
      {
        value = inputs[inputSelect].minValue;
      }
      else if ( settingsSelect == 1 )
      {
        value = inputs[inputSelect].maxValue;
      }
      previous = value;
      while ( !digitalRead(SW) );
      while ( digitalRead(SW) )
      {
        value = encoder( value );
        if ( value != previous )
        {
          lcdBufClear();
          int2str( value, lcdBuffer, 80 );
          lcdPrint(lcdBuffer);
        }
        previous = value;
      }
      if ( settingsSelect == 0 )
      {
        inputs[inputSelect].minValue = value;
      }
      else if ( settingsSelect == 1 )
      {
        inputs[inputSelect].maxValue = value;
      }
      break;
    case 1:
    case 2:
    case 3:
    case 4:
      lcdPrint("no available settings");
      delay(1000);
      break;
    default:
      lcdPrint("err input type invalid");
      delay(1000);
      break;
  }
}

/*
   output to phone via bluetooth using remoteXY API
*/
void remoteXYoutput()
{
  // Convert temperature value, pH value and Oxygen value to text
  RemoteXY_Handler();
  int indeces[21] = {0};
  for ( int i = 0; i < 22; i++ )
  {
    if ( i < batteryInputIndex )
    {
      indeces[i] = i;
    }
    else if ( i == batteryInputIndex ) continue;
    else
    {
      indeces[i - 1] = i;
    }
  }
  dtostrf(inputs[indeces[0]].value, 0, 1, RemoteXY.Temp_1);
  dtostrf(inputs[indeces[1]].value, 0, 1, RemoteXY.Temp_2);
  dtostrf(inputs[indeces[2]].value, 0, 1, RemoteXY.Temp_3);
  dtostrf(inputs[indeces[3]].value, 0, 1, RemoteXY.Temp_4);
  dtostrf(inputs[indeces[4]].value, 0, 1, RemoteXY.Temp_5);
  dtostrf(inputs[indeces[5]].value, 0, 1, RemoteXY.Temp_6);
  dtostrf(inputs[indeces[6]].value, 0, 1, RemoteXY.Temp_7);
  dtostrf(inputs[indeces[7]].value, 0, 1, RemoteXY.pH_1);
  dtostrf(inputs[indeces[8]].value, 0, 1, RemoteXY.pH_2);
  dtostrf(inputs[indeces[9]].value, 0, 1, RemoteXY.pH_3);
  dtostrf(inputs[indeces[10]].value, 0, 1, RemoteXY.pH_4);
  dtostrf(inputs[indeces[11]].value, 0, 1, RemoteXY.pH_5);
  dtostrf(inputs[indeces[12]].value, 0, 1, RemoteXY.pH_6);
  dtostrf(inputs[indeces[13]].value, 0, 1, RemoteXY.pH_7);
  dtostrf(inputs[indeces[14]].value, 0, 1, RemoteXY.DO_1);
  dtostrf(inputs[indeces[15]].value, 0, 1, RemoteXY.DO_2);
  dtostrf(inputs[indeces[16]].value, 0, 1, RemoteXY.DO_3);
  dtostrf(inputs[indeces[17]].value, 0, 1, RemoteXY.DO_4);
  dtostrf(inputs[indeces[18]].value, 0, 1, RemoteXY.DO_5);
  dtostrf(inputs[indeces[19]].value, 0, 1, RemoteXY.DO_6);
  dtostrf(inputs[indeces[20]].value, 0, 1, RemoteXY.DO_7);
  dtostrf(inputs[batteryInputIndex].value, 0, 1, RemoteXY.Battery);
}

/*
   setup function, runs on boot
*/
void setup()
{

  //SERIAL SETUP
  Serial.begin(115200);
  Serial2.begin(9600);
  
  //D LATCH INTERFACE SETUP
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(G1, OUTPUT);
  pinMode(G2, OUTPUT);
  pinMode(CLR, OUTPUT);
  pinMode(DD, OUTPUT);
  digitalWrite(G1, HIGH);
  digitalWrite(G2, HIGH);
  digitalWrite(CLR, HIGH);
  daclr();
  
  //ROTARY ENCODER SETUP
  pinMode(CLK, INPUT); // PINMODE FOR ROTARY ENCODER
  pinMode(DT, INPUT); // PINMODE FOR ROTARY ENCODER
  pinMode(SW, INPUT); // PINMODE FOR ROTARY ENCODER

  //LCD SETUP
  lcd.begin(); // LCD BEGIN
  lcd.backlight();

  //RTC SETUP
  if ( !rtc.begin() ) {  // RTC BLOCK SETUP BEGIN
    lcdPrint("RTC failed to start");
    while (1);          // RTC BLOCK SETUP END
  }

  //SD SETUP
  pinMode( SD_CS, OUTPUT );  // PINMODE FOR SD CARD PIN
  if ( !SD.begin(SD_CS) )      // SD BLOCK BEGIN
  {
    lcdPrint("SD card failed to start");
    while ( 1 );
  }
  isSdCheck();    // CHECK IF THE SD IS MOUNTED
  // open settings.txt
  File settingsFile = SD.open("/settings.txt", FILE_READ);
  if ( !settingsFile )
  {
    lcdPrint("settings.txt not found");
    while( 1 );
  }

  //BLUETOOTH SETUP
  RemoteXY_Init();

  // READ SETTINGS FILE SETUP
  int batteryCounter = 0;
  for ( int i = 0; i < 37; i++ )
  {
    bufClear();
    comBufferIndex = 0;
    while ( settingsFile.available() ) // looking for first word
    {
      comBuffer[comBufferIndex] = settingsFile.read();
      // first word found
      if ( comBuffer[comBufferIndex] == ',' || comBuffer[comBufferIndex] == ';' )
      {
        int val = interpret( comBufferIndex );

        //off
        if ( val == 0 )
        {
          while ( settingsFile.available() && settingsFile.read() != 13 );
          break;
        }
        else if ( (val == 5) && (i < 6) )
        {
          
          #ifdef DEBUG
            lcdPrint("found i2c input");
            delay(1000);
          #endif
          
          i2cSetup( &settingsFile, i );
          while ( settingsFile.available() && settingsFile.read() != 13 );
          break;
        }
        else if ( (val == 1) && (i > 5) && (i < 18) )
        {
          
          #ifdef DEBUG
            lcdPrint("found analog input");
            delay(1000);
          #endif
          
          analogSetup( &settingsFile, i );
          while ( settingsFile.available() && settingsFile.read() != 13 );
          break;
        }
        else if ( (val == 2) && (i > 5) && (i < 22) )
        {

          #ifdef DEBUG
            lcdPrint("found digital input");
            delay(1000);
          #endif
          
          digitalSetup( &settingsFile, i );
          while ( settingsFile.available() && settingsFile.read() != 13 );
          break;
        }
        else if ( (val == 3) && (i > 5) && (i < 22) )
        {

          #ifdef DEBUG
            lcdPrint("found ndigital input");
            delay(1000);
          #endif
          
          ndigitalSetup( &settingsFile, i );
          while ( settingsFile.available() && settingsFile.read() != 13 );
          break;
        }
        else if ( (val == 6) && (i > 5) && (i < 22) )
        {
          #ifdef DEBUG
            lcdPrint("found oneWire input");
            delay(1000);
          #endif
          
          oneWireSetup( &settingsFile, i );
          while ( settingsFile.available() && settingsFile.read() != 13 );
          break;
        }
        else if ( (val == 4) && (i > 5) && (i < 22) )
        {

          #ifdef DEBUG
            lcdPrint("found battery input");
            delay(1000);
          #endif
          
          batteryCounter = batteryCounter + 1;
          if ( batteryCounter != 1 )
          {
            lcdPrint("Err too many battery inputs");
            while (1);
          }
          inputs[i].type = 4;
          batteryInputIndex = i;

          while ( settingsFile.available() && settingsFile.read() != 13 );
          break;
        }
        else if ( (val == 5) && (i > 21) )
        {
          
          #ifdef DEBUG
            lcdPrint("found output");
            delay(1000);
          #endif
          
          outputSetup( &settingsFile, i - 22 );
          while ( settingsFile.available() && settingsFile.read() != 13 );
          break;
        }
        else
        {
          lcdBufClear();
          lcdBuffer[0] = 48+i/10;
          lcdBuffer[1] = 48+i%10;
          while (1)
          {
            lcdPrint("Err pin type not recognized or invalid. Pin #:");
            delay(1000);
            lcdPrint(lcdBuffer);
            delay(1000);
          }
        }
      } // if first word found
      comBufferIndex = comBufferIndex + 1;
    } // while looking for first word
  }
  if ( batteryCounter == 0 )
  {
    lcdPrint("Err no battery input");
    while (1);
  }
  // after reading all i/o settings, read power limit
  comBufferIndex = 0;
  while ( settingsFile.available() )
  {
    comBuffer[comBufferIndex] = settingsFile.read();
    if ( comBuffer[comBufferIndex] == ',' || comBuffer[comBufferIndex] == ';' )
    {
      powerLimit = buf2num( comBufferIndex );
      break;
    }
    comBufferIndex = comBufferIndex + 1;
  }
  settingsFile.close();
  #ifdef DEBUG
    lcdPrint("initial read");
    delay(1000);
  #endif
  readInputs();
  #ifdef DEBUG
    lcdPrint("done reading");
    delay(1000);
    lcdPrint("logging to SD card...");
    delay(1000);
  #endif
  logSDCard();
  #ifdef DEBUG
    lcdPrint("done logging");
    delay(1000);
    lcdPrint("setting outputs...");
    delay(1000);
  #endif
  turnOnOutputs();
  #ifdef DEBUG
    lcdPrint("outputs done");
    delay(1000);
  
    lcdPrint("setup complete");
    delay(1000);
  #endif
  lcd.noBacklight();
} // setup

/*
   loop function, runs continuously after setup()
*/
void loop()
{
  // menu opened
  if ( !digitalRead(SW) )
  {
    lcd.backlight();
    while  ( !digitalRead(SW) );
    int previousSelect = 0;
    int inputSelect = 0;
    lcdPrint("show readings");
    while ( digitalRead(SW) )
    {
      inputSelect = encoder( inputSelect );
      if ( inputSelect > 10 ) inputSelect = 3;
      else if ( inputSelect > 3 ) inputSelect = 0;
      if ( inputSelect != previousSelect )
      {
        switch ( inputSelect )
        {
          case 0: 
            lcdPrint("show readings");
            break;
          case 1:
            lcdPrint("show settings");
            break;
          case 2:
            lcdPrint("bluetooth");
            break;
          case 3:
            lcdPrint("read now");
        }
      }
      previousSelect = inputSelect;
    }
    while ( !digitalRead(SW) );
    switch ( inputSelect )
    {
      case 0: 
        lcdPrint("turn knob to show readings");
        lcdShowReadings();
        break;
      case 1:
        lcdPrint("turn knob to show readings");
        lcdShowInputSettings();
        break;
      case 2:
        lcdPrint("press knob to turn off bluetooth");
        while ( digitalRead(SW) )
          remoteXYoutput();
        break;
      case 3:
        lcdPrint("reading...");
        readInputs();
        break;
    }
    lcd.noBacklight();
    while ( !digitalRead(SW) );
  }

  // after an amount of time, do standard functions
  if ( (unsigned long)(millis()-prevTime)>batteryTimeModify() )
  {
    prevTime = millis();
    readInputs();
    logSDCard();
    turnOnOutputs();
  }
}
