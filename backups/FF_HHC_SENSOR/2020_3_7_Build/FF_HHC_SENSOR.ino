
#include <Wire.h> // Version 2.3.5
#include <DallasTemperature.h> // Version 3.80

// battery read variables
uint32_t duration, period;
int batt_vol;
int batt;

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

//other variables
const int pinNumbers[16] = {36, 39, 34, 35, 32, 33, 25, 26, 27, 14, 12, 13, 23, 19, 18, 5};
int oneWireIndeces[16] = {0}; // for oneWire connections

char inputBuffer[20] = {0};
byte serial_event = 0;
int analogReadBuffer[10] = {0};

/*
  bufClear sets all the values of
*/
void bufClear()
{
  for ( int i = 0; i < 20; i++ )
  {
    inputBuffer[i] = 0;
  }
}

/*
   returns the length of the string located in inputBuffer
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
  for ( int i = 0; i < strLength; i++ )
  {
    divisor /= 10;
    inputBuffer[i] = 30 + n / divisor;
    n -= divisor * (n / divisor);
  }
  return strLength;
}

/*

*/
int battery(int pin) {
  uint32_t reading, accumulator;
  uint16_t count = 0;
  uint8_t shift = 2;
  float result;
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
    return batt;
  }
  int batt_vol = period;
  if (batt_vol < 772) result = 3; // positive
  if ((batt_vol <= 784) && (batt_vol >= 772)) result = 2; // negative
  if (batt_vol > 784) result = 1;
  if (batt_vol == 0) result = 0;
  return result;
}

/*
  DO_read combines functions from the available DO_I2C arduino sketch given
  by Atlas Scientific. This reads from the ezo Dissolved Oxygen sensor.
*/
float DO_read(int address)
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
  DO_float = atof(DO);
  return DO_float;
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
*/
int inputRead(int inputID, int readType)
{
  int result = 0;

  // i2c analog read
  if ( (inputID < 6) && (readType == 1) )
  {
    int n = 6;
    for ( int i = 0; i < n; i++ )
    {
      result += round(DO_read(inputID) * 1000);
    }
    result /= n;
  }  // i2c analog read

  // standard analog read
  if ( (inputID > 5) && (inputID < 18) && (readType == 1) )
  {
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
  }  // standard analog read

  // standard digital read
  if ( (inputID > 5) && ((readType == 2) || (readType == 3)) )
  {
    for ( int i = 0; i < 10; i++ )
    {
      result += readInput();
    }
    result /= 10;
  }

  // battery read
  if ( (inputID > 5) && (readType == 4) )
  {
    result = battery(pinNumbers[inputID - 6]);
  }
  
  //1Wire read
  if ( (inputID > 5) && (readType == 5) )
  {
    OneWire oneWire(oneWireIndeces[inputID]);          // Setup a oneWire )
    DallasTemperature sensors(&oneWire);    // Pass our oneWire reference to Dallas Temperature.
    sensors.requestTemperatures();                    // Send command to get temperatures
    result = round(1000 * sensors.getTempFByIndex(oneWireIndeces[inputID]));
  }
  return result;
}

void serialEvent()
{
  Serial.readBytesUntil(13, inputBuffer, 20);
  serial_event = true;
}

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  // set up inputs
}

void loop()
{
  if ( serial_event == true )
  {
    if ( inputBuffer[0] == 0 )
    {
      int readResult = inputRead((int)inputBuffer[1], (int)inputBuffer[2]);
      int bufferIndex = int2str(readResult);
      inputBuffer[bufferIndex] = 13;
      Serial.write(inputBuffer);
    }
    else if ( inputBuffer[0] == 1 )
    {
      i2cAddresses[(int)inputBuffer[1]] = inputBuffer[2];
    }
    bufClear();
    serial_event = false;
  }
}
