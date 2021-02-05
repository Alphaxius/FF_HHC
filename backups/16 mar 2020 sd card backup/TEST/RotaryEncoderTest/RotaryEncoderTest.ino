
#define CLK 14
#define DT 4
#define SW 35

byte count;
byte clk;
byte dt;
byte rotaryEncoderIsMoving;
byte clockwise;

void setup()
{
  Serial.begin(9600);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT);

  count = 0;
  rotaryEncoderIsMoving = false;
  clk = 0;
  dt = 0;
  clockwise = false;
}


void loop()
{
  while ( 1 )
  {
    clk = digitalRead(CLK);
    dt = digitalRead(DT);
    if ( clk && dt )
    {
      continue;
    }
    else if ( !clk && dt ) // clockwise
    {
      rotaryEncoderIsMoving = true;
      clockwise = true;
      break;
    }
    else if ( clk && !dt ) //counterclockwise
    {
      rotaryEncoderIsMoving = true;
      clockwise = false;
      break;
    }
  }
  if ( rotaryEncoderIsMoving )
  {
    if ( clockwise )
    {
      count++;
    }
    else
    {
      count--;
    }
    Serial.println(count);
    // debounce
    rotaryEncoderIsMoving = 1;
    while ( rotaryEncoderIsMoving > 0 ) // wait for rollover
    {
      if ( digitalRead(DT) && digitalRead(CLK) )
      {
        rotaryEncoderIsMoving++;
      }
    }
    rotaryEncoderIsMoving = false; 
  }
}
