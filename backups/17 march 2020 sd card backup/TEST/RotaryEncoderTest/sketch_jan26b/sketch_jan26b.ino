
// Rotary Encoder Inputs
#include <LiquidCrystal_I2C.h>
#define CLK 14
#define DT 4
#define SW 5
int temp_counter = 0;
int buttonCounter = 0;
int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;

LiquidCrystal_I2C lcd( 0x27, 16 ,2 );
void setup() {
  
  // pins as inputs
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);

  lcd.begin();
  lcd.backlight();
  // Setup Serial Monitor
  Serial.begin(9600);
  lastStateCLK = digitalRead(CLK);             // initial state of CLK


  lcd.clear();
  lcd.print("Set Temp");
  lcd.setCursor(10,0);
  lcd.print(counter);
  lcd.setCursor(0,1);
  lcd.print("Temp");
  
}

void loop() {
  int btnState = digitalRead(SW);
  if (btnState == LOW) {
  if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed!");
      buttonCounter++;
      Serial.println(buttonCounter);
     
      if(buttonCounter == 1){
        lcd.clear();
        lcd.print("Adjust Temp  ");
        
   } else if (buttonCounter == 2) {
        lcd.clear();
        lcd.print("Set Temp");
        lcd.setCursor(10,0);
        lcd.print(counter);
        lcd.setCursor(0,1);
        lcd.print("Temp");
        lcd.setCursor(10,1);
        lcd.print("30");
        buttonCounter = 0;
   }   
    }
    lastButtonPress = millis();           // Remember last button press event
      
  }

  while (buttonCounter == 1){
  encoder();
  lcd.setCursor(12,0);
  temp_counter = counter;
  lcd.print(temp_counter);
  break;
  }

  while (buttonCounter == 2){
  lcd.clear();
  lcd.print("Set Temp");
  lcd.setCursor(10,0);
  lcd.print(temp_counter);
  lcd.setCursor(0,1);
  lcd.print("Temp");
  lcd.setCursor(10,1);
  lcd.print("30"); /////////////////data from 1st mcu display here
  break;
  }

  //lcd.setCursor(10,0);
  //lcd.print(temp_counter);
  
  delay(1); 
  
}


void encoder(){
  currentStateCLK = digitalRead(CLK);                                             // current state of CLK
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){
    if (digitalRead(DT) != currentStateCLK) {
      counter ++;
      currentDir ="CW";
    } else {
      counter --;
      currentDir ="CCW";
    }
    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
  }
  lastStateCLK = currentStateCLK;                                                   // Remember last CLK state
}
