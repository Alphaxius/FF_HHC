
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd( 0x27, 16 , 2 );       //LCD OBJECT

/*
   print the contents of lcdBuffer to the 4x20 lcd
*/
void lcdPrint(const char *lcdBuffer)
{
  lcd.clear();
  int i = 0;
  while ( lcdBuffer[i] != '\0' )
  {
    lcd.setCursor(i-(i / 20)*20+(i/40)*20,(i/20)%2);
    lcd.write(lcdBuffer[i]);
    i++;
  }
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  lcd.noBacklight();
  delay(250);
  lcd.backlight();
  delay(250);
  lcd.noBacklight();
  delay(250);
  lcd.backlight();
  delay(250);
  lcdPrint("testing testing testing testing testing testing testing testing ");
  delay(1000);
}
