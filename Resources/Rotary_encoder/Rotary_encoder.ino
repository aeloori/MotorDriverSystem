
#include <TM1637.h>

#define encoder0PinA 2
#define encoder0PinB 3
#define encoder0Btn 4
int encoder0Pos = 0;

TM1637 tm(9, 8);
void setup() {
  tm.begin();
  tm.setBrightness(2);
  Serial.begin(9600);
  pinMode(encoder0PinA, INPUT_PULLUP);
  pinMode(encoder0PinB, INPUT_PULLUP);
  pinMode(encoder0Btn, INPUT_PULLUP);
  attachInterrupt(0, doEncoder, CHANGE);
}
int valRotary,lastValRotary;
void loop() {
  int btn = digitalRead(encoder0Btn);
  Serial.print(btn);
  Serial.print(" ");
  Serial.print(valRotary);
  if(valRotary>9)
  {
    tm.display("0by0");
  }
  else if(valRotary>99)
  {
    tm.display("0hi0");
  }
  else if(valRotary>999)
  {
    tm.display("byei");
  }
  else
  {
    tm.display("hell");
  }
  if(valRotary>lastValRotary)
  {
  Serial.print("  CW");
  }
  if(valRotary)  {

  Serial.print("  CCW");
  }
  lastValRotary = valRotary;
  Serial.println(" ");
  delay(250);
}
void doEncoder()
{
  if (digitalRead(encoder0PinA) == digitalRead(encoder0PinB))
  {
  encoder0Pos+=10;
  }
  else
  {
  encoder0Pos-=10;
  }
  valRotary = encoder0Pos/2.5;
}
