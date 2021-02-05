#include <dummy.h>

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

void dtest()
{
  dd(LOW);
  for ( int i = 0; i < 8; i++ )
  {
    daddress(i);
    dclk1();
    delay(100);
  }
  
  dd(LOW);
  for ( int i = 0; i < 8; i++ )
  {
    daddress(i);
    dclk2();
    delay(100);
  }
  dd(HIGH);
  for ( int i = 0; i < 8; i++ )
  {
    daddress(i);
    dclk1();
    delay(100);
  }
  
  dd(HIGH);
  for ( int i = 0; i < 8; i++ )
  {
    daddress(i);
    dclk2();
    delay(100);
  }
}

void setup() {
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
  dtest();

//  daddress(0);
//  while (1)
//  {
//    dd(HIGH);
//    dclk1();
//    delay(500);
//    dd(LOW);
//    dclk1();
//    delay(500);
//  }
  
}
