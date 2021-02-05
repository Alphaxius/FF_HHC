byte serial_event = false;
char buf[20] = {0};
int  bufLength = 0;

#define A1  33
#define A2  13
#define A3  15
#define G1  25
#define G2  26 
#define CLR 27
#define DD  32

void dclk1()
{
  digitalWrite(G1, LOW);
  digitalWrite(G1, HIGH);
}

void dclk2()
{
  digitalWrite(G2, LOW);
  digitalWrite(G2, HIGH);
}

void dclr()
{
  digitalWrite(CLR, LOW);
  digitalWrite(CLR, HIGH);
}

void daddress( int a )
{
  switch ( a%8 )
  {
    case 0:
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
      break;
    case 1:
      digitalWrite(A1, HIGH);
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
      break;
    case 2:
      digitalWrite(A1, LOW);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, LOW);
      break;
    case 3:
      digitalWrite(A1, HIGH);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, LOW);
      break;
    case 4:
      digitalWrite(A1, LOW);
      digitalWrite(A2, LOW);
      digitalWrite(A3, HIGH);
      break;
    case 5:
      digitalWrite(A1, HIGH);
      digitalWrite(A2, LOW);
      digitalWrite(A3, HIGH);
      break;
    case 6:
      digitalWrite(A1, LOW);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, HIGH);
      break;
    case 7:
      digitalWrite(A1, HIGH);
      digitalWrite(A2, HIGH);
      digitalWrite(A3, HIGH);
      break;
  }
}

void dd( int d )
{
  digitalWrite(DD, d);
}
void bufClear()
{
  for ( int i = 0; i < 20; i++ )
  {
    buf[i] = 0;
  }
  bufLength = 0;
}

void setup() {
  Serial2.begin(9600);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(G1, OUTPUT);
  pinMode(G2, OUTPUT);
  pinMode(CLR, OUTPUT);
  pinMode(DD, OUTPUT);

  digitalWrite(G1, HIGH);
  digitalWrite(G2, HIGH);
  digitalWrite(CLR, HIGH);
}

void loop() {
  while ( Serial2.available() )
  {
    char temp = Serial2.read();
    if ((temp != 254) && serial_event )
    {
      buf[bufLength++] = temp;
    }
    else if (temp==254)
    {
      serial_event = true;
    }
    else if (temp == 255) break;
  }
  if ( serial_event )
  {
    daddress(buf[0]);
    if ( buf[1] == 30 ) dd(HIGH);
    else if ( buf[1] == 31 ) dd(LOW);
    if ( buf[0] < 8 ) dclk1();
    else if ( buf[1] >= 8 ) dclk2();
    bufClear();
    serial_event = false;
  }
}
