#include "defines.h"

// change the number in the next line to the pin that the sensor is plugged into, default is I0
int input = I0;

float result = 0;
int analogReadBuffer[10]={0};
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
}

void loop()
{
  // Get 10 sample value from the sensor for smooth the value
  for (int i = 0; i < 10; i++)
  {
    analogReadBuffer[i] = analogRead(input);
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
  result = 0;
  for ( int i = 2; i < 8; i++ )
  {
    result += analogReadBuffer[i];
  }
  result /= 6;
  Serial.println(result);
  delay(500);
}
