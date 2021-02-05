
#include "defines.h"


// change the number in the next line to the pin that the sensor is plugged into, default is I0
int inputPin = I0;




/*

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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(inputPin,INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  int batteryValue = battery(inputPin);
  Serial.println("battery() returns");
  Serial.println(batteryValue);
  Serial.println();
  delay(1000);
}
