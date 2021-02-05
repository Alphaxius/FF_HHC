void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600);
}

char in[20] = {0};
char temp = 0;
int comlength = 0;
byte serial_event = false;

void loop() {
  // put your main code here, to run repeatedly:
  while ( Serial2.available() )
  {
//    Serial.write(Serial2.read());
    temp = Serial2.read();
    if ( temp != 254 && temp != 255 && serial_event ) 
    {
      in[comlength++] = temp;
    }
    else if ( temp == 254 )
    {
      serial_event = true;
      continue;
    }
    else if ( temp == 255 )
    {
      break;
    }
  }
  if ( serial_event )
  {
    for ( int i = 0; i < comlength; i++ )
    {
      Serial.print((int)in[i]);
      Serial.print(' ');
      in[i] = 0;
    }
    Serial.println();
    serial_event = false;
    comlength = 0;
  }
}
