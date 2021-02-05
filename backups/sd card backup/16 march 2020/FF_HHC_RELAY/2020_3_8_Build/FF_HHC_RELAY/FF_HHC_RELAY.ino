
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
#define CLK 14                               //ROTARY ENCODER PIN
#define DT 4                                 //ROTARY ENCODER PIN
#define SW 5                                 //ROTARY ENCODER PIN

// SD CARD PINS
#define SD_CS 5                              //SD PIN

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

struct InputPin
{
	//type: determines how the value should be interpretted
		// 0 = off
		// 1 = analog
		// 2 = digital
		// 3 = ndigital
		// 4 = battery
	int type = 0;
	
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
char inputBuffer[20] = {0}; 
// place to store current location in the buffer
int inputBufferIndex = 0;
// input and output arrays, makes keeping track of all the input and output information easier
InputPin inputs[22] = {InputPin()};
OutputPin outputs[16] = {OutputPin()};
// index for inputs array that has the battery value
int batteryInputIndex = 0;
// integer in Watts (could be another unit of power, just be consistent)
// represents how much power the battery system can handle
int powerLimit = 0;
// another string used to send data to the other mcu
char sensorComBuffer[4] = {0};
// one more string used to buffer writing to sd files
char writeToBuffer[500] = {0};

// LCD setup
LiquidCrystal_I2C lcd( 0x27, 16 , 2 ); //LCD OBJECT

// real time clock variables
RTC_DS3231 rtc; // RTC OBJECT
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Rotary encoder variables
int counter = 0;                   // ROTARY ENCODER
int currentStateCLK;               // ROTARY ENCODER
int lastStateCLK;                  // ROTARY ENCODER

// timing variables
unsigned int prevTime = 0;
unsigned int currTime = 0;

/*
 * dclk1 clocks the first d latch
 */
void dclk1()
{
  digitalWrite(G1, LOW);
  digitalWrite(G1, HIGH);
}

/*
 * dclk2 clocks the second d latch
 */
void dclk2()
{
  digitalWrite(G2, LOW);
  digitalWrite(G2, HIGH);
}

/*
 * dclr clears all d latches
 */
void dclr()
{
  digitalWrite(CLR, LOW);
  digitalWrite(CLR, HIGH);
}

/*
 * daddress sets which address is being accessed
 * parameter a is the address as int modulo between 0 and 7
 * both d latches share the address bus
 */
void daddress( int a )
{
  switch ( a%8 )
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
 * dd sets the single bit data bus state
 */
void dd( int d )
{
  digitalWrite(DD, d);
}

/*
 * dwrite combines set address, set data, and clock into one function
 * d is HIGH or LOW
 * a is the address 0 through 15
 */
void dwrite( int d, int a )
{
	daddress(a);
	dd(d);
	if(a<8) dclk1();
	else dclk2();
}

/*
 * changeOutputPinState turns on or off an output if enable conditions are met.
 */
void changeOutputPinState( int address, int state )
{
	if ( outputs[address].enable == 0 ) return;
	else if ( state == 0 )
	{
		outputs[address].outputOn = 0;
		dwrite(LOW, address);
		return;
	}
	else
	{
		outputs[address].outputOn = 1;
		dwrite(HIGH, address);
		return;
	}
} //changeOutputPinState

/*
 * getCurrentNominalPowerUsage returns the total amount of nominal power being used.
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
	return powerSum;
}

/*
 * checkPower determines whether turning on an output would be within the power limit.
 * parameter i is the index to the outputs vector which contains the output we want to turn on.
 */
byte checkPower( int i )
{
  if ( getCurrentNominalPowerUsage() + outputs[i].power <= (3/(6-inputs[batteryInputIndex].value))*powerLimit ) return 1;
  else return 0;
}

/*
 * updates the inputs array with all current values
 */
void readInputs()
{
	for ( int i = 0; i < 22; i++ )
	{
		// skip if off
		if ( inputs[i].type == 0 ) continue;
		bufClear();
    sensorComBuffer[0] = 0;
    sensorComBuffer[1] = i;
    sensorComBuffer[2] = inputs[i].type;
    sensorComBuffer[3] = 13;
		Serial2.write(sensorComBuffer);
		inputBufferIndex = Serial2.readBytesUntil(13, inputBuffer, 20);
		inputs[i].value = buf2num( inputBufferIndex );
	}
} //readInputs

/*
scoreInput returns a value between 0 and 1 or greater.
A value greater than 1 may indicate a problem.
This value tells how important it is for action to be taken.
If an input is connected to an output, another function may turn on said
output based on the number that this function returns.
For a digital pin, this returns 1 when active and 0 when inactive
*/
float scoreInput(int pinNumber)
{
	if ( inputs[pinNumber].type == 1 )
	{
		float score = 0.5 - (inputs[pinNumber].value - inputs[pinNumber].minValue)/(inputs[pinNumber].maxValue - inputs[pinNumber].minValue);
		score = 4*score*score;
		return score;
	}
	if ( inputs[pinNumber].type == 2 )
	{
		return inputs[pinNumber].value;
	}
	if ( inputs[pinNumber].type == 3 )
	{
		return 1-inputs[pinNumber].value;
	}
	return 0;
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

byte output(int pinNumber)
{
  // case that input is not connected to any outputs
  if ( inputs[pinNumber].outputPinNumberMin == -1 && inputs[pinNumber].outputPinNumberMax == -1 )
  {
    return 0;
  }
  
  // type is analog
  if ( inputs[pinNumber].type == 1 )
  {
    // case that input is low enough to turn on its corresponding output
    // as long as the corresponding output exists
    float threshold = 0.625*inputs[pinNumber].minValue+0.375*inputs[pinNumber].maxValue;
    if ( inputs[pinNumber].value < threshold && inputs[pinNumber].outputPinNumberMin != -1 && checkPower(inputs[pinNumber].outputPinNumberMin) )
    {
      changeOutputPinState(inputs[pinNumber].outputPinNumberMin, 1);
      return 1;
    }
  
    // case that input is high enough to turn on its corresponding output
    // as long as the corresponding output exists
    threshold = 0.375*inputs[pinNumber].minValue+0.675*inputs[pinNumber].maxValue;
    if ( inputs[pinNumber].value > threshold && inputs[pinNumber].outputPinNumberMax != -1 && checkPower(inputs[pinNumber].outputPinNumberMax) )
    {
      changeOutputPinState(inputs[pinNumber].outputPinNumberMax, 1);
      return 1;
    }
    
    // case that direction of input does not match direction of output
    return 0;
  }
  
  if ( inputs[pinNumber].type == 2 || inputs[pinNumber].type == 3 )
  {
    if ( inputs[pinNumber].outputPinNumberMin != -1 && checkPower(inputs[pinNumber].outputPinNumberMin))
    {
      changeOutputPinState(inputs[pinNumber].outputPinNumberMin, 1);
      return 1;
    }
    return 0;
  }
  
  return 0;
} // output

/*
turnOnOutputs is the logic which decides which outputs should be turned on
based on scores returned by scoreInput
*/
void turnOnOutputs()
{
	int triedIndeces[22] = {0};
	int triedIndecesIndex = 0;
	while ( triedIndecesIndex < 22 ) // first output
	{
		triedIndeces[triedIndecesIndex] = 0;
		// get a maximum score
		for ( int i = 1; i < 22; i++ ) // skip testing inputs[0] since it is
										// assumed to be the highest before
										// going into the loop
		{
			// is score of inputs[i] greater than previous largest one?
			if ( scoreInput(triedIndeces[triedIndecesIndex]) < scoreInput(i) )
			{
				// if so, check whether we have tried it before
				int tried = 0;
				for ( int j = 0; j < triedIndecesIndex; j++ )
				{
					// previously used?
					if ( i == triedIndeces[triedIndecesIndex] )
					{
						tried = 1;
						break;
					}
				}
				if ( tried == 1 )
				{
					continue;
				}
				triedIndeces[triedIndecesIndex] = i;
			}
		}
		
		// temporarily store the maximum score
		float score = scoreInput(triedIndeces[triedIndecesIndex]);
		
		// if the maximum is too low, all done
		// all inputs are regular and no action needs to be taken
		if ( score < 0.08 )
		{
			break;
		}
		
		// if the maximum is too high, problem indicated!
		if ( score > 1 )
		{
			/*what to do if problem is indicated?*/
		}
		// otherwise score is not problematically high, but needs action taken.
		
		if ( output( triedIndeces[triedIndecesIndex] ) == 1 )
		{
			// output was successful
			triedIndecesIndex = triedIndecesIndex + 1;
			continue;
		}
	}
} //turnOnOutputs

/*
interpret uses the inputBuffer to determine what type an input is.
This is a helper function.
Parameter length tells this function how long the word in the inputBuffer is.
*/
int interpret( int length )
{
	if ( length < 2 ) return -1;
	int sum = inputBuffer[0] + inputBuffer[1];
	switch ( sum )
	{
		case 131: return 4; // BATTERY
		case 141: return 2; // DIGITAL
		case 143: return 1; // ANALOG
		case 146: return 3; // NDIGITAL
		case 149: return 0; // OFF
		case 157: return 5; // ON
		default: return -1; // INCORRECT ENTRY
	}
}

/*
buf2num uses the inputBuffer and assumes that it holds a number.
This function returns that number as an integer.
Parameter length is how long the number is.
*/
int buf2num( int length )
{
	int num = 0;
	int coef = 1;
	for ( int i = 0; i < length; i++ )
	{
		num = num + coef * (inputBuffer[length - 1 - i] - 30);
		coef = coef * 10;
	}
	return num;
}

/*
bufClear sets all the values of the inputBuffer to 0
*/
void bufClear()
{
	for ( int i = 0; i < 20; i++ )
	{
		inputBuffer[i] = 0;
	}
}

/*
 * interpretOutput assumes that the buffer contains the following string: "OUTxx"
 * xx is a two digit number (ascii encoding) that should be between 01 and 16
 * the function returns the digit as an integer between 0 and 15 corresponding to an
 * index of the outputs array
 * parameter length is the amount of data to be checked in the buffer. This should be no more
 * and no less than 5. 
 * And the number of the counting shall be five.
 */
int interpretOutput( int length )
{
	if ( length == 5 )
	{
		inputBuffer[0] = inputBuffer[3];
		inputBuffer[1] = inputBuffer[4];
		return buf2num( 2 ) - 1;
	}
	
	return -1;
}

/*
 * i2cSetupAddress reads the data from the settings file and sends the i2c address to the other mcu
 * for the other mcu to store and use when reading the corresponding sensor.
 */
void i2cSetupAddress(File *settingsFile, int inputsIndex)
{
	bufClear();
	inputBufferIndex = 0;
	while ( settingsFile->available() )
	{
		inputBuffer[inputBufferIndex] = settingsFile->read();
		if ( inputBuffer[inputBufferIndex] == ',' || inputBuffer[inputBufferIndex] == ';'  )
		{
			int address = buf2num( inputBufferIndex );
			
			// check if address is too big
			if ( address > 127 )
			{
				lcd.print("invalid i2c address");
				while(1);
			}
			else
			{
        sensorComBuffer[0] = 1;
        sensorComBuffer[1] = inputsIndex;
        sensorComBuffer[2] = address;
        sensorComBuffer[3] = 13;
        Serial2.write(sensorComBuffer);
			}
			
			break;
		}
		inputBufferIndex = inputBufferIndex + 1;
	}
}

/*
 * inputSetupMin reads data from the settings file and sets up the minimum value 
 * of that input
 */
void inputSetupMin(File *settingsFile, int inputsIndex)
{
	bufClear();
	inputBufferIndex = 0;
	while ( settingsFile->available() )
	{
		inputBuffer[inputBufferIndex] = settingsFile->read();
		if ( inputBuffer[inputBufferIndex] == ',' || inputBuffer[inputBufferIndex] == ';' )
		{
			inputs[inputsIndex].minValue = buf2num( inputBufferIndex );
			break;
		}
		inputBufferIndex = inputBufferIndex + 1;
	}
}

/*
 * inputSetupMax reads data from the settings file and sets up the maximum value 
 * of that input
 */
void inputSetupMax(File *settingsFile, int inputsIndex)
{
	bufClear();
	inputBufferIndex = 0;
	while ( settingsFile->available() )
	{
		inputBuffer[inputBufferIndex] = settingsFile->read();
		if ( inputBuffer[inputBufferIndex] == ',' || inputBuffer[inputBufferIndex] == ';' )
		{
			inputs[inputsIndex].maxValue = buf2num( inputBufferIndex );
			break;
		}
		inputBufferIndex = inputBufferIndex + 1;
	}
}

/*
 * inputSetupOutputMin reads data from the settings file and sets up the output
 * which will be turned on when the value of the sensor is too low.
 */
void inputSetupOutputMin(File *settingsFile, int inputsIndex)
{
	bufClear();
	inputBufferIndex = 0;
	while ( settingsFile->available() )
	{
		inputBuffer[inputBufferIndex] = settingsFile->read();
		if ( inputBuffer[inputBufferIndex] == ',' || inputBuffer[inputBufferIndex] == ';' )
		{
			inputs[inputsIndex].outputPinNumberMin = interpretOutput( inputBufferIndex );
		}
	}
}


/*
 * inputSetupOutputMax reads data from the settings file and sets up the output
 * which will be turned on when the value of the sensor is too high.
 */
void inputSetupOutputMax(File *settingsFile, int inputsIndex)
{
	bufClear();
	inputBufferIndex = 0;
	while ( settingsFile->available() )
	{
		inputBuffer[inputBufferIndex] = settingsFile->read();
		if ( inputBuffer[inputBufferIndex] == ',' || inputBuffer[inputBufferIndex] == ';' )
		{
			interpretOutput( inputBufferIndex );
		}
	}
}

/*
 * oneWireSetupAddress reads data from the settings file and sends the relevant information
 * to the other mcu
 */
void oneWireSetupAddress(File *settingsFile,  int i )
{
  
}

/*
 * i2cSetup sets up an i2c input
 */
void i2cSetup(File *settingsFile,  int i)
{
	inputs[i].type = 1;
	
	i2cSetupAddress( settingsFile, i );
	inputSetupMin( settingsFile, i );
	inputSetupMax( settingsFile, i );
	inputSetupOutputMin( settingsFile, i );
	inputSetupOutputMax( settingsFile, i );
	
	while ( settingsFile->available() && settingsFile->read() != 13 );
}

/*
 * oneWireSetup sets up a 1-Wire input
 */
void oneWireSetup( File *settingsFile, int i )
{
	inputs[i].type = 1;
	
	oneWireSetupAddress( settingsFile, i );
	inputSetupMin( settingsFile, i );
	inputSetupMax( settingsFile, i );
	inputSetupOutputMin( settingsFile, i );
	inputSetupOutputMax( settingsFile, i );
	
	while ( settingsFile->available() && settingsFile->read() != 13 );
}

/*
 * analogSetup sets up an analog input
 */
void analogSetup( File *settingsFile, int i )
{
	inputs[i].type = 1;
	
	inputSetupMin( settingsFile, i );
	inputSetupMax( settingsFile, i );
	inputSetupOutputMin( settingsFile, i );
	inputSetupOutputMax( settingsFile, i );
	
	while ( settingsFile->available() && settingsFile->read() != 13 );
}

/*
 * digitalSetup sets up a positive logic digital input
 */
void digitalSetup( File *settingsFile, int i )
{
	inputs[i].type = 2;
	
	inputs[i].minValue = 0;
	inputs[i].maxValue = 1;
	inputSetupOutputMin( settingsFile, i );
	
	while ( settingsFile->available() && settingsFile->read() != 13 );
}

/*
 * ndigitalSetup sets up a negative logic input
 */
void ndigitalSetup( File *settingsFile, int i )
{
	inputs[i].type = 3;
	
	inputs[i].minValue = 1;
	inputs[i].maxValue = 0;
	inputSetupOutputMin( settingsFile, i );
	
	while ( settingsFile->available() && settingsFile->read() != 13 );
}

/*
 * set up an output pin
 */
void outputSetup( File *file, int i )
{
  outputs[i].enable = 1;
  bufClear();
  inputBufferIndex = 0;
  while ( file->available() )
  {
    inputBuffer[inputBufferIndex] = file->read();
    if ( inputBuffer[inputBufferIndex] == ',' || inputBuffer[inputBufferIndex] == ';' )
    {
      outputs[i].power = buf2num( inputBufferIndex );
      break;
    }
    inputBufferIndex = inputBufferIndex + 1;
  }
  
  while ( file->available() && file->read() != 13 );
}

String inputNames( int i )
{
  if ( i < 6 )
  {
    return "I2C"+String(i);
  }
  else if ( i < 16 )
  {
    return "IN"+String(i-6);
  }
  else if ( i < 22 )
  {
    switch(i)
    {
      case 16: return "INA";
      case 17: return "INB";
      case 18: return "INC";
      case 19: return "IND";
      case 20: return "INE";
      case 21: return "INF";
    }
  }
  return "ERR_INVLD_INPUT";
}

// CHECK IF FILE IS CREATED, IF NOT, CREATE FILE
void isFileCreate() {
  File file = SD.open("/data.txt");
  if (!file) {
    // Change here for value and set up of Displaying data in SD card
    writeFile("/data.txt", "Temp, Date \r\n");
  }
  file.close();
}

// WRITE TO SD CARD
void writeFile(const char * path, const char * message) {
  File file = SD.open(path, FILE_WRITE);
  if (!file)
  {
    isFileCreate();
    file = SD.open(path, FILE_APPEND);
    if (!file)
    {
      lcd.print("error writing to file");
      return;
    }
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
    isFileCreate();
    file = SD.open(path, FILE_APPEND);
    if (!file)
    {
      lcd.print("error appending to file");
      return;
    }
  }
  file.println(message);
  file.close();
}

/*
 * logSDCard appends all readings to the data.txt file
 */
void logSDCard() {
  DateTime now = rtc.now();
  // READ "RTCLIB" LIBARY TO SEE DIFFERENT DATE, TIME TOSTRING() FORMAT
  char buf2[] = "YY/MM/DD-hh:mm:ss";
  now.toString(buf2);
  int wtbi = 0;
  for (; wtbi < sizeof(buf2)/sizeof(char); wtbi++ )
  {
    writeToBuffer[wtbi] = buf2[wtbi];
  }
  writeToBuffer[wtbi++] = '\n';
  for ( int i = 0; i < 22; i++ )
  {
    if ( inputs[i].type == 0 || inputs[i].type == 4 ) continue;
    bufClear();
    inputNames(i).toCharArray(inputBuffer,20);
    int j = 0;
    while( inputBuffer[j] != 0 )
    {
      writeToBuffer[wtbi++] = inputBuffer[j++];
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
      writeToBuffer[wtbi++] = 30 + n / divisor;
      n -= divisor * (n / divisor);
    }
    writeToBuffer[wtbi++]='\n';
  }
  writeToBuffer[wtbi++]='\n';
  appendFile("/data.txt",writeToBuffer);
}

// CHECK IS SD IS MOUNT OR NOT
void isSdCheck() {
  SD.begin(SD_CS);
  if (!SD.begin(SD_CS)) {
    lcd.print("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    lcd.print("No SD card attached");
    return;
  }
  lcd.print("Initializing SD card...");
  if (!SD.begin(SD_CS)) {
    lcd.print("ERROR - SD card initialization failed!");
    return; // init failed
  }
}

// ROTARY ENCODER LOGIC, TURN CLOCK WISE OR COUNTER CLOCKWISE CHECK
byte encoder(byte counter) {
  currentStateCLK = digitalRead(CLK);// CURRENT STATE OF CLK
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1)
  {
    if (digitalRead(DT) != currentStateCLK) counter++;
    else counter--;
  }
  lastStateCLK = currentStateCLK;// REMEMBER LAST STATE OF CLK
  return counter;
}

void lcdShowReadings()
{
  int inputSelect = 0;
  while ( digitalRead(SW) != LOW )
  {
    inputSelect = encoder( inputSelect )%22;
    int rollover = 0;
    while ( inputs[inputSelect].type == 0 && inputs[inputSelect].type == 4 && rollover < 22 )
    {
      inputSelect++;
      rollover++;
    }
    if ( rollover == 22 )
    {
      lcd.setCursor(0,1);
      lcd.print("ERR_NO_INPUTS");
      return;
    }
    lcd.setCursor(0,1);
    lcd.print(inputNames( inputSelect ));
    lcd.print(inputs[inputSelect].value);
  }
}

void lcdShowInputSettings()
{
  int inputSelect = 0;
  int settingsSelect = 0;
  int previous = 0;
  int value = 0;
  while ( digitalRead(SW) != LOW )
  {
    inputSelect = encoder( inputSelect )%22;
    if ( inputSelect != previous )
    {
      int rollover = 0;
      while ( inputs[inputSelect].type == 0 && inputs[inputSelect].type == 4 && rollover < 22 )
      {
        inputSelect++;
        rollover++;
      }
      if ( rollover == 22 )
      {
        lcd.setCursor(0,1);
        lcd.print("ERR_NO_INPUTS");
        return;
      }
      lcd.setCursor(0,1);
      lcd.print(inputNames( inputSelect ));
    }
  }
  lcd.clear();
  lcd.setCursor(0,1);
  switch(inputs[inputSelect].type)
  {
    case 1:
      previous = 0;
      settingsSelect = 0;
      while ( digitalRead(SW) != LOW )
      {
        settingsSelect = encoder( settingsSelect )%2;
        if ( settingsSelect != previous )
        {
          if ( settingsSelect == 0 ) 
          {
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("min value");
          }
          else if ( settingsSelect == 1 ) 
          {
            lcd.clear();
            lcd.setCursor(0,1);
            lcd.print("max value");
          }
        }
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
      while ( digitalRead(SW) != LOW )
      {
        value = encoder( value );
        if ( value != previous )
        {
           lcd.clear();
           lcd.setCursor(0,1);
           lcd.print( value );
        }
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
    case 2:
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("no available settings");
      delay(1000);
      break;
    case 3:
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("no available settings");
      delay(1000);
      break;
    default:
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print("ERR_INPUT_TYPE");
      delay(1000);
      break;
  }
}

/*
 * output to phone via bluetooth using remoteXY API
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
 * setup function, runs on boot
 */
void setup()
{
	//ROTARY ENCODER SETUP
  pinMode(CLK, INPUT); // PINMODE FOR ROTARY ENCODER
  pinMode(DT, INPUT); // PINMODE FOR ROTARY ENCODER
  pinMode(SW, INPUT_PULLUP); // PINMODE FOR ROTARY ENCODER
  lastStateCLK = digitalRead(CLK);

  //LCD SETUP
  lcd.begin(); // LCD BEGIN
  lcd.backlight();

	//RTC SETUP
  rtc.begin();          // RTC OBJECT BEGIN
  if (! rtc.begin()) {  // RTC BLOCK SETUP BEGIN
    while (1);          // RTC BLOCK SETUP END
  }  
  
  // IF TIME IS WRONG, ENABLE THIS CODE, BURN THE CODE TO THE ESP32 TO GET RIGHT TIME AND DATE, THEN DISABLE IT.
  //rtc.adjust(DateTime(__DATE__, __TIME__)); 
  
	//SD SETUP
  pinMode( SD_CS, OUTPUT);                      // PINMODE FOR SD CARD PIN
  SD.begin(SD_CS);                              // SD BLOCK BEGIN
  isSdCheck();                                  // CHECK IF THE SD IS MOUNTED
  isFileCreate();                               // CHECK IF SD HAS FILE NAME DATA, IF NOT, CREATE NEW FILE CALL DATA.TEXT
  File settingsFile = SD.open("/settings.txt");
  if ( !settingsFile )
  {
    lcd.print("settings.txt not found");
  }
  
	//SERIAL SETUP
  Serial.begin(115200);
	Serial2.begin(9600);

  //BLUETOOTH SETUP
  RemoteXY_Init();
	
	//D LATCH INTERFACE SETUP
	pinMode(A1, OUTPUT);
	pinMode(A2, OUTPUT);
	pinMode(A3, OUTPUT);
	pinMode(G1, OUTPUT);
	pinMode(G2, OUTPUT);
	pinMode(CLR, OUTPUT);
	pinMode(DD, OUTPUT);
  
	digitalWrite(G2, HIGH);
	digitalWrite(CLR, HIGH);

  prevTime = 0;
  
	// READ SETTINGS FILE SETUP
	int batteryCounter = 0;
	for ( int i = 0; i < 38; i++ )
	{
		bufClear();
		inputBufferIndex = 0;
		while ( settingsFile.available() ) // looking for first word
		{
			inputBuffer[inputBufferIndex] = settingsFile.read();
			// first word found
			if ( inputBuffer[inputBufferIndex] == ',' || inputBuffer[inputBufferIndex] == ';' )
			{
				int val = interpret( inputBufferIndex );
				
				//off
				if ( val == 0 )
				{
					while ( settingsFile.available() && settingsFile.read() != 13 );
					break;
				}
				else if ( (val == 5) && (i < 6) )
				{
					i2cSetup( &settingsFile, i );
					break;
				}
				else if ( (val == 1) && (i > 5) && (i < 18) )
				{
					analogSetup( &settingsFile, i );
					break;
				}
				else if ( (val == 2) && (i > 5) && (i < 22) )
				{
					digitalSetup( &settingsFile, i );
					break;
				}
				else if ( (val == 3) && (i > 5) && (i < 22) )
				{
					ndigitalSetup( &settingsFile, i );
					break;
				}
				else if ( (val == 4) && (i > 5) && (i < 22) )
				{
					batteryCounter = batteryCounter + 1;
					if ( batteryCounter != 1 )
					{
						lcd.print("too many battery inputs");
						while(1);
					}
					batteryInputIndex = i;
					
					while ( settingsFile.available() && settingsFile.read() != 13 );
					break;
				}
				else if ( (val == 5) && (i > 21) )
				{
					outputSetup( &settingsFile, i - 22 );
					break;
				}
				else if ( (val == 6) && (i > 18) )
				{
					oneWireSetup( &settingsFile, i - 22 );
					break;
				}
				else
				{
					lcd.print("pin type error");
					while(1);
				}
			} // if first word found
			inputBufferIndex = inputBufferIndex + 1;
		} // while looking for first word
	}
	if ( batteryCounter == 0 )
	{
		lcd.print("no battery input");
		while(1);
	}
	while ( settingsFile.available() )
	{
		inputBuffer[inputBufferIndex] = settingsFile.read();
		if ( inputBuffer[inputBufferIndex] == ',' || inputBuffer[inputBufferIndex] == ';' )
		{
			powerLimit = buf2num( inputBufferIndex );
			break;
		}
		inputBufferIndex = inputBufferIndex + 1;
	}
	settingsFile.close();
  lcd.noBacklight();
} // setup

/*
 * loop function, runs continuously after setup()
 */
void loop()
{
  if ( digitalRead(SW) == LOW )
  {
    unsigned int t0 = millis()%65536;
    while ( digitalRead(SW) == LOW );
    if ( millis()%65536 - t0 < 1024 )
    {
      lcd.backlight();
      lcd.clear();
      lcd.setCursor(1,0);
      lcd.print("Reading...");
      readInputs();
      lcdShowReadings();
      lcd.noBacklight();
    }
    else
    {
      lcdShowInputSettings();
    }
  }
  currTime = millis()%(2*tenMinutes);
  if ( (currTime>prevTime+tenMinutes)||((currTime<prevTime)&&(currTime>prevTime+tenMinutes)) )
  {
  	readInputs();
    logSDCard();
    remoteXYoutput();
  	turnOnOutputs();
  }
}
