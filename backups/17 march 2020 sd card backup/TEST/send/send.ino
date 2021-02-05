void setup() {
  // put your setup code here, to run once:
  Serial2.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial2.write('h');
  Serial2.write('e');
  Serial2.write('l');
  Serial2.write('l');
  Serial2.write('o');
  Serial2.write(255);

  delay(1000);
}
