#include "defines.h"

int readInput()
{
// change the number in the next line to the pin that the sensor is plugged into, default is I0
  return digitalRead( I0 );
}

int result = 0;
void setup()
{
  Serial.begin(9600);
  pinMode(I0, INPUT);
  pinMode(I1, INPUT);
  pinMode(I2, INPUT);
  pinMode(I3, INPUT);
  pinMode(I4, INPUT);
  pinMode(I5, INPUT);
  pinMode(I6, INPUT);
  pinMode(I7, INPUT);
  pinMode(I8, INPUT);
  pinMode(I9, INPUT);
  pinMode(IA, INPUT);
  pinMode(IB, INPUT);
  pinMode(IC, INPUT);
  pinMode(ID, INPUT);
  pinMode(IE, INPUT);
  pinMode(IF, INPUT);
}

void loop()
{
  result = 0;
  for ( int i = 0; i < 10; i++ )
  {
    result += readInput();
  }
  if ( result < 5 ) result = 0;
  else result = 1;
  Serial.println(result);
}
