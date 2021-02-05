
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd( 0x27, 16 , 2 );       //LCD OBJECT

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
  for ( int i = 0; i < 40; i++ )
  {
    for ( int j = 0; j < 2; j++ )
    {
      lcd.setCursor(i,j);
      lcd.print('a');
      delay(100);
      lcd.print('b');
      delay(100);
    }
  }
  lcd.clear();
}
