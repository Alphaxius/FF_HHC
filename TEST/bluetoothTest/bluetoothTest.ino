
// Declare float value for each sensor and a battery

float temp_Value_1;
float temp_Value_2;
float temp_Value_3;
float temp_Value_4;
float temp_Value_5;
float temp_Value_6;
float temp_Value_7;
float pH_Value_1;
float pH_Value_2;
float pH_Value_3;
float pH_Value_4;
float pH_Value_5;
float pH_Value_6;
float pH_Value_7;
float DO_1;
float DO_2;
float DO_3;
float DO_4;
float DO_5;
float DO_6;
float DO_7;
float batt;
int batt_ble =0;


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
// DO NOT CHANGE HERE
// this is used by RemoteXY_API
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




void setup() {
  RemoteXY_Init ();
  Serial.begin(115200);
  
}

void loop() {
  RemoteXY_Handler ();   // Convert temperature value, pH value and Oxygen value to text 
  dtostrf(temp_Value_1, 0, 1, RemoteXY.Temp_1);
  dtostrf(temp_Value_2, 0, 1, RemoteXY.Temp_2);
  dtostrf(temp_Value_3, 0, 1, RemoteXY.Temp_3);
  dtostrf(temp_Value_4, 0, 1, RemoteXY.Temp_4);
  dtostrf(temp_Value_5, 0, 1, RemoteXY.Temp_5);
  dtostrf(temp_Value_6, 0, 1, RemoteXY.Temp_6);
  dtostrf(temp_Value_7, 0, 1, RemoteXY.Temp_7);

  
  dtostrf(pH_Value_1, 0, 1, RemoteXY.pH_1);
  dtostrf(pH_Value_2, 0, 1, RemoteXY.pH_2);
  dtostrf(pH_Value_3, 0, 1, RemoteXY.pH_3);
  dtostrf(pH_Value_4, 0, 1, RemoteXY.pH_4);
  dtostrf(pH_Value_5, 0, 1, RemoteXY.pH_5);
  dtostrf(pH_Value_6, 0, 1, RemoteXY.pH_6);
  dtostrf(pH_Value_7, 0, 1, RemoteXY.pH_7);

  
  dtostrf(DO_1, 0, 1, RemoteXY.DO_1);
  dtostrf(DO_2, 0, 1, RemoteXY.DO_2);
  dtostrf(DO_3, 0, 1, RemoteXY.DO_3);
  dtostrf(DO_4, 0, 1, RemoteXY.DO_4);
  dtostrf(DO_5, 0, 1, RemoteXY.DO_5);
  dtostrf(DO_6, 0, 1, RemoteXY.DO_6);
  dtostrf(DO_7, 0, 1, RemoteXY.DO_7);

  
  dtostrf(batt, 0, 1, RemoteXY.Battery);


  temp_Value_1 = random (101,200);// Temperature Value that recieve from sensor
  temp_Value_2 = random (201,300);
  temp_Value_3 = random (201,300);
  temp_Value_4 = random (201,300);
  temp_Value_5 = random (201,300);
  temp_Value_6 = random (201,300);
  temp_Value_7 = random(0,100); 


     
  pH_Value_1 = random(0,100);     // pH Value that recieve from sensor
  pH_Value_2 = random(101,200);
  pH_Value_3 = random(201,300);
  pH_Value_4 = random(201,300);
  pH_Value_5 = random(201,300);
  pH_Value_6 = random(201,300);
  pH_Value_7 = random(201,300);


  DO_1  = random(0,100);          // Oxygen Value that recieve from sensor
  DO_2 = random(101,200);
  DO_3 = random(201,300);
  DO_4 = random(201,300);
  DO_5 = random(201,300);
  DO_6 = random(201,300);
  DO_7 = random(201,300);


  
  batt = random(1,50);          // Battery Value that recieve from battery status

}
