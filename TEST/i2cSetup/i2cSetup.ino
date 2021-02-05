void setup() {
  Serial2.begin(9600);
  delay(1000);
  char command[] = {0x49, 0x32, 0x43, 0x2C, 0x35, 0x31, 0x0D};
  Serial2.write(command);
}


void loop() {}
