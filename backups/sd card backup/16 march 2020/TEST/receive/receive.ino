byte serial_event = false;
char buf[20] = {0};
int  bufLength = 0;
//
//void serialEvent()
//{
//  bufLength = Serial2.readBytesUntil(255,buf,20);
//  serial_event = true;
//}

void bufClear()
{
  for ( int i = 0; i < 20; i++ )
  {
    buf[i] = 0;
  }
  bufLength = 0;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
//  if ( serial_event )
//  {
////    for ( int i = 0; i < bufLength; i++ )
////    {
////      Serial.write(buf[i]);
////    }
////    Serial.write('\n');
//    Serial.println(buf);
//    serial_event = false;
//  }
  while ( Serial2.available() )
  {
    char temp = Serial2.read();
    if (temp == 255 ) break;
    buf[bufLength++] = temp;
    serial_event = true;
  }
  if ( serial_event )
  {
    for ( int i = 0; i < bufLength; i++ )
    {
      Serial.write(buf[i]);
    }
    Serial.write('\n');
    bufClear();
    serial_event = false;
  }
}
