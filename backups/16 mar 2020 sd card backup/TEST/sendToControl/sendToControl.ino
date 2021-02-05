void setup() {
  // put your setup code here, to run once:
  Serial2.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for ( int i = 0; i < 16; i++ )
  {
    Serial2.write(254);
    Serial2.write(i);
    Serial2.write(31);
    Serial2.write(255);
    delay(100);
  }
  for ( int i = 0; i < 16; i++ )
  {
    Serial2.write(254);
    Serial2.write(i);
    Serial2.write(30);
    Serial2.write(255);
    delay(100);
  }

  delay(1000);
}
