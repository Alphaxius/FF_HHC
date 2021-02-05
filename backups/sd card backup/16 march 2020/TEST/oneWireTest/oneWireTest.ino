#include <DallasTemperature.h>
#include "defines.h"

// change the number in the next line to the pin that the sensor is plugged into, default is I0
int one_wire = I0;


OneWire oneWire;
DallasTemperature sensors;
float result = 0;
void setup()
{
  Serial.begin(9600);
}

void loop()
{
  oneWire = OneWire(one_wire);
  sensors = DallasTemperature(&oneWire);
  sensors.begin();
  sensors.requestTemperatures();
  result = 0;
  for ( int i = 0; i < 6; i++ )
  {
    result += sensors.getTempFByIndex(0);
  }
  result /= 6;
  Serial.println(result);
  delay(500);
}
