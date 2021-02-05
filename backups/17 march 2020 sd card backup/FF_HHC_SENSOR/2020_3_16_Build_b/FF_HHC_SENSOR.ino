/*********************************************************************
 * Jacob Barrett, Gregg Dawson, Sothearith Ly, Minh Tri-Truong
 * 
 * FF_HHC_SENSORS
 * Program for sensor side board's ESP32 DEVKIT V1
 * 
 * Last updated 16 March 2020
 *********************************************************************/

#include <Wire.h> // Version 2.3.5
#include <DallasTemperature.h> // Version 3.80

/////// Atlas Scientific Dissolved Oxygen variables ///////////////////////////
//holds i2c addresses (up to 6)
char i2cAddresses[6] = {0};
//used to hold the I2C response code.
byte code = 0;
//we make a 20 byte character array to hold incoming data from the D.O. circuit
char do_data[20] = {0};
//used as a 1 byte buffer to store inbound bytes from the D.O. Circuit.
byte in_char = 0;
byte i = 0; //counter used for DO_data array.
char *DO; //char pointer used in string parsing.
//float var used to hold the float value of the D.O.
float DO_float;

////// Dallas Temperature Variables
OneWire oneWire;
DallasTemperature sensors;

//other variables
const int pinNumbers[16] = {36, 39, 34, 35, 32, 33, 25, 26, 27, 14, 12, 13, 23, 19, 18, 5};

// input buffer controls
unsigned char comBuffer[20] = {0};
int  bufLength = 0;
byte serial_event = 0;
int analogReadBuffer[10] = {0};
unsigned long t0 = 0;

/*
  bufClear sets all the values of comBuffer to 0
*/
void bufClear()
{
  
  for ( int i = 0; i < 20; i++ )
  {
    comBuffer[i] = 0;
  }
  bufLength = 0;
}

/*
   returns the length of the string located in comBuffer
*/
int int2str(int n)
{
  int divisor = 10;
  int strLength = 1;
  while (n % divisor != n)
  {
    strLength += 1;
    divisor *= 10;
  }
  for ( int i = 1; i < strLength+1; i++ )
  {
    divisor /= 10;
    comBuffer[i] = 48 + n / divisor;
    n -= divisor * (n / divisor);
  }
  return strLength;
}

/*
  battery(int) reads the battery monitor and converts to the battery voltage
  accurate within 2v returns a value between 10 and 16 inclusive, or 0
  returns 0 if no reading can be made, likely because the battery monitor is
  disconnected
  parameter pin is the actual pin number that is being read
  
  Based on code by Luan Nguyen, Rowina Akim, Jade Do, and Quan Tong
  Modified for use with ESP32 and updated interface
*/
int battery(int pin) {
  uint32_t duration, period;
  uint32_t reading, accumulator;
  uint16_t count = 0;
  uint8_t shift = 2;
  int battery_voltage = 0;
  reading  = 0;
  accumulator = 0;
  do {
    reading =  pulseIn(pin, HIGH);
    accumulator = accumulator + reading;
    count = count + 1;
  } while (count != (0x01 << shift));
  duration = (uint32_t) (accumulator >> shift) & 0x00FFFFFF; // duration in uS
  period = (uint32_t)(duration / 250); // 2*high time / 1000, period in ms
  if (period == 0) {
    battery_voltage = 0;
  }
  else if ( period > 132 )
  {
    battery_voltage = 10;
  }
  else if ( period > 130 )
  {
    battery_voltage = 12;
  }
  else if ( period > 127 )
  {
    battery_voltage = 14;
  }
  else
  {
    battery_voltage = 16;
  }
  return battery_voltage;
}

/*
  DO_read combines functions from the available DO_I2C arduino sketch given
  by Atlas Scientific. This reads from the ezo Dissolved Oxygen sensor.
*/
void DO_read(int address)
{
  Wire.beginTransmission(i2cAddresses[address]); //call the circuit by its ID number.
  Wire.write('r'); //transmit the command that was sent through the serial port.
  Wire.endTransmission();  //end the I2C data transmission.
  delay(1000);
  Wire.requestFrom(i2cAddresses[address], 20, 1);//call the circuit and request 20 bytes (this is more than we need)
  code = Wire.read();
  while (Wire.available()) { //are there bytes to receive.
    in_char = Wire.read(); //receive a byte.
    do_data[i] = in_char; //load this byte into our array.
    i += 1; //incur the counter for the array element.
    if (in_char == 0) { //if we see that we have been sent a null command.
      i = 0; //reset the counter i to 0.
      Wire.endTransmission(); //end the I2C data transmission.
      break; //exit the while loop.
    }
  }
  DO = strtok(do_data, ","); //let's pars the string at each comma.
  if ( DO == NULL )
  {
    DO_float = 0.0;
    return;
  }
  else
  {
    DO_float = atof(DO);
    return;
  }
}

/*
 * 
 */
int readDallasTemp(int inputID)
{
  oneWire = OneWire(pinNumbers[inputID]);
  sensors = DallasTemperature(&oneWire);
  sensors.begin();
  sensors.requestTemperatures();
  int result = 0;
  for ( int i = 0; i < 6; i++ )
  {
    result += sensors.getTempFByIndex(0);
  }
  result /= 6;
  return result;
}

/*
 * 
 */
int standardDigitalRead(int inputID)
{
  int result = 0;
  for ( int i = 0; i < 10; i++ )
  {
    {
      result += digitalRead(pinNumbers[inputID]);
    }
    if ( result < 5 ) result = 0;
    else result = 1;
  }
  return result;
}

/*
 * 
 */
int standardAnalogRead(int inputID)
{
  int result = 0;
  // Get 10 sample value from the sensor for smooth the value
  for (int i = 0; i < 10; i++)
  {
    analogReadBuffer[i] = analogRead(pinNumbers[inputID]);
    delay(10);
  }
  // Sort the analog from small to large
  for (int i = 0; i < 9; i++)
  {
    for (int j = i + 1; j < 10; j++)
    {
      if (analogReadBuffer[i] > analogReadBuffer[j])
      {
        int temp = analogReadBuffer[i];
        analogReadBuffer[i] = analogReadBuffer[j];
        analogReadBuffer[j] = temp;
      }
    }
  }
  // Take the average value of 6 center samples
  for ( int i = 2; i < 8; i++ )
  {
    result += analogReadBuffer[i];
  }
  result /= 6;
  return result;
}

/*
 * 
 */
int standardI2Cread(int inputID)
{
  int result = 0;
  for ( int i = 0; i < 6; i++ )
  {
    int timesRead = 0;
    do {
      DO_read(inputID);
      timesRead += 1;
    } while ( DO_float == -1 && timesRead < 10 );
    if ( DO_float == -1 )
    {
      return -1;
    }
    result += DO_float;
  }
  result /= 6;
  return result;
}

/*
  inputRead reads the input based on the code sent from other MCU
  inputID is used to get pin number
  read type determines what function is used to read from that pin number
  inputID < 6 is an i2c input
  inputID >= 6 is a digital, analog, or battery input
  readType = 1 is analog read
		 = 2 or 3 is digital read
		 = 4 is battery read
     = 5 is oneWire/dallas temperature read
*/
int inputRead(int inputID, int readType)
{
  // i2c analog read
  if ( (inputID < 6) && (readType == 1) )
    return standardI2Cread(inputID);

  // standard analog read
  if ( (inputID > 5) && (inputID < 18) && (readType == 1) )
    return standardAnalogRead(inputID - 6);

  // standard digital read
  if ( (inputID > 5) && ((readType == 2) || (readType == 3)) )
    return standardDigitalRead(inputID - 6);

  // battery read
  if ( (inputID > 5) && (readType == 4) )
    return battery(pinNumbers[inputID - 6]);
  
  //1Wire read
  if ( (inputID > 5) && (readType == 6) )
    return readDallasTemp(inputID - 6);

  return 0;
}

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600);
  Wire.begin();
  // set up inputs
  for ( int i = 0; i < 16; i++ )
  {
    pinMode(pinNumbers[i],INPUT);
  }
}

void loop()
{
  if ( Serial2.available() )
  {
    t0 = millis();
    while ( (unsigned long)(millis()-t0)<60000 )
    {
      unsigned char temp = Serial2.read();
      if ( bufLength == 20 )
      {
        comBuffer[0] = 254;
        comBuffer[1] = 253;
        comBuffer[2] = 255;
        Serial2.write(comBuffer, 3);
        serial_event = false;
        break;
      }
      else if ((temp != 254) && (temp != 255) && serial_event )
        comBuffer[bufLength++] = temp;
      else if (temp == 254)
        serial_event = true;
      else if (temp == 255) break;
    }
  }
  if ( serial_event )
  {
    
//    // DEBUG
//    for ( int i = 0; i < bufLength; i++ )
//    {
//      Serial.print((int)comBuffer[i]);
//      Serial.print(' ');
//    }
//    Serial.println();
//    // END DEBUG
    
    if ( comBuffer[0] == 0 )
    {
      int readResult = inputRead((int)comBuffer[1], (int)comBuffer[2]);
      bufClear();
      comBuffer[0]=254;
      bufLength = int2str(readResult)+1;
      comBuffer[bufLength++]=255;
      
//      // DEBUG
//      for ( int i = 0; i < bufLength; i++ )
//      {
//        Serial.print((int)comBuffer[i]);
//        Serial.print(' ');
//      }
//      Serial.println();
//      // END DEBUG
      
      Serial2.write(comBuffer, bufLength);
    }
    else if ( comBuffer[0] == 1 )
    {
      i2cAddresses[(int)comBuffer[1]] = comBuffer[2];
    }
    bufClear();
    serial_event = false;
  }
}
