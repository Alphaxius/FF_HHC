#include <Wire.h>

const int i2cCount = 3; // This is the number of i2c sensors you want to test
char i2cAddresses[i2cCount] = {97, 86, 51}; // these are the addresses to each i2c sensor
  // make sure that the number of addresses in the array matches i2cCount


///////////////////////////////////////////////////////////////////////////////
/////// Atlas Scientific Dissolved Oxygen variables ///////////////////////////
//used to hold the I2C response code.
byte code = 0;
//we make a 20 byte character array to hold incoming data from the D.O. circuit
char do_data[20] = {0};
//used as a 1 byte buffer to store inbound bytes from the D.O. Circuit.
byte in_char = 0;
byte i = 0; //counter used for DO_data array.
char *DO; //char pointer used in string parsing.
//float var used to hold the float value of the D.O.
double DO_float;

char printBuffer[20] = {0};

/*
  DO_read combines functions from the available DO_I2C arduino sketch given
  by Atlas Scientific. This reads from the ezo Dissolved Oxygen sensor.
*/
void DO_read(char address)
{
  Wire.beginTransmission(address); //call the circuit by its ID number.
  Wire.write('r'); //transmit the command that was sent through the serial port.
  Wire.endTransmission();  //end the I2C data transmission.
  delay(1000);
  Wire.requestFrom(address, 20, 1);//call the circuit and request 20 bytes (this is more than we need)
  code = Wire.read();
  while (Wire.available()) { //are there bytes to receive.
    in_char = Wire.read(); //receive a byte.
    do_data[i] = in_char; //load this byte into our array.
    i += 1; //incur the counter for the array element.
    if (in_char == 0 || i == 20) { //if we see that we have been sent a null command.
      i = 0; //reset the counter i to 0.
      Wire.endTransmission(); //end the I2C data transmission.
      break; //exit the while loop.
    }
  }
  DO = strtok(do_data, ","); //let's pars the string at each comma.
  if ( sizeof(DO)/sizeof(char) < 2 )
  {
    DO_float = -1;
    return;
  }
  DO_float = atof(DO);
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  for ( int j = 0; j < i2cCount; j++ )
  {
    for ( int i = 0; i < 6; i++ )
    {
      int timesRead = 0;
      do {
        DO_read(i2cAddresses[j]);
        timesRead += 1;
      } while ( DO_float == -1 && timesRead < 10 );
      if ( DO_float == -1 )
      {
        Serial.println("Failed to read I2C");
        return;
      }
      result += DO_float;
    }
    result /= 6;
    Serial.println(result);
    delay(100);
  }
  delay(500);
}
