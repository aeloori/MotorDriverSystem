 //#include <TM1637.h>
#include <TM1637Display.h>
#include <EEPROM.h>


#define encodera 2
#define encoderb 3
#define encoderSw 4
boolean encoderFlag=false;

//motorPins
#define motorEn 7   //motor on off pin
#define motorD 8    //motor direction pin
#define motorSp 9   //motor speed pin

#define startBtn 10

int displayCLK=5;
int displayDIO=6;

int counter=50;
int timeCount=0;
int currentState;
int lastState;
String temp="";

int btnLastState=0;
boolean btnSw=false;

TM1637Display screen(displayCLK, displayDIO);
uint8_t data[] = { 0xff, 0xff, 0xff, 0xff };
uint8_t blank[] = { 0x00, 0x00, 0x00, 0x00 };
int segRPM[]={0,0,0,0};

int segTime[]={0,0,0,0};




void setup()
{
  pinMode(encodera,INPUT);
  pinMode(encoderb,INPUT);
  pinMode(encoderSw,INPUT_PULLUP);

  pinMode(motorEn,OUTPUT);
  pinMode(motorSp,OUTPUT);
  pinMode(motorD,OUTPUT);

  pinMode(startBtn,INPUT_PULLUP);
  Serial.begin(9600);                                                                                               
  screen.setBrightness(4);

  lastState=digitalRead(encodera);
  btnLastState=digitalRead(encoderSw);
  digitalWrite(motorD,HIGH);// always in same direction
//  checkPrev();
}

void loop()
{
  beginOP();
  
}

void checkPrev()
{
  if(EEPROM.read(0)!=0 && EEPROM.read(0)!=255 && EEPROM.read(1)!=0 && EEPROM.read(1)!=255)
  {
    counter=EEPROM.read(0);
    timeCount=readTime();
    if(digitalRead(startBtn)==HIGH)
    {
      runOp();
    }
  }
}

void timeWrite()
{
  int temp=timeCount;
  int i=1;
  while(temp>0)
  {
    if(temp>255)
    {
      EEPROM.write(i,255);
    }
    else
    {
      EEPROM.write(i,temp);
    }
    temp-=255;
    i++;
  }
}

int readTime()
{
  int time=0;
  for(int i=1;i<=40;i++)
  {
    time+=EEPROM.read(i);
  }
  return time;
}

void beginOP()
{
  currentState=digitalRead(encodera);
  while(digitalRead(startBtn)==HIGH)
  {
    if(digitalRead(encoderSw)!=btnLastState)
  {
     Serial.println();
     Serial.println("btnPressed");
     if(encoderFlag==false)
     {
      encoderFlag=true;
      Serial.println(encoderFlag);
     }
     else
     {
      encoderFlag=false;
      Serial.println(encoderFlag);
     }
     delay(500);
  }

  if(currentState!=lastState && currentState==1)
  {
    if(encoderFlag==false)
    {
      if(digitalRead(encoderb)!=currentState && counter>=50 && counter<300)
      {
        counter+=10;
        Serial.print("RPM set is : ");
        Serial.println(counter);
        show();
      }
      else if(counter>50)
      {
        counter-=10;
        Serial.print("RPM set is : ");
        Serial.println(counter);
        show();
      }
    }
    else
    {
      if(digitalRead(encoderb)!=currentState && counter>=0 && counter<=9999)
      {
        timeCount++;
        Serial.print("time set is : ");
        Serial.println(timeCount);
        show();
      }
      else if(counter>0)
      {
        timeCount--;
        Serial.print("time set is : ");
        Serial.println(timeCount);
        show();
      }
    }
  }
  lastState=currentState;
  delay(1);
  }
  runOp();
}

void show()
{
  if(counter>0 && counter<10)
  {
    sep(true);
    data[0] = screen.encodeDigit(0);
    data[1] = screen.encodeDigit(0);
    data[2] = screen.encodeDigit(0);
    data[3] = screen.encodeDigit(segRPM[3]);
    screen.setSegments(data);
  }
  else if(counter>9 && counter<100)
  {
    sep(true);
    data[0] = screen.encodeDigit(0);
    data[1] = screen.encodeDigit(0);
    data[2] = screen.encodeDigit(segRPM[2]);
    data[3] = screen.encodeDigit(segRPM[3]);
    screen.setSegments(data);
  }

  else if(counter>99 && counter<1000)
  {
    sep(true);
    data[0] = screen.encodeDigit(0);
    data[1] = screen.encodeDigit(segRPM[1]);
    data[2] = screen.encodeDigit(segRPM[2]);
    data[3] = screen.encodeDigit(segRPM[3]);
    screen.setSegments(data);
  }

  else if(counter>999 && counter<1000)
  {
    sep(true);
    data[0] = screen.encodeDigit(segRPM[0]);
    data[1] = screen.encodeDigit(segRPM[1]);
    data[2] = screen.encodeDigit(segRPM[2]);
    data[3] = screen.encodeDigit(segRPM[3]);
    screen.setSegments(data);
  }
  else
  {
    screen.setSegments(blank);
  }
}

void sep(boolean flag)
  {
     if(flag==true)
     {
        int temp=counter;
        int i=3;
        if(counter>9)
        {
          while(temp>0)
          {
            segRPM[i]=temp%10;
            temp/=10;
            i--;
          }
        } 
        else if(counter>=0 && counter<10)
        {
          segRPM[3]=counter;
        }
     }
//     else
//     {
//        int temp=counter;
//        int i=3;
//        if(counter>9)
//        {
//          while(temp>0)
//          {
//            segTime[i]=temp%10;
//            temp/=10;
//            i--;
//          }
//        } 
//        else if(counter>=0 && counter<10)
//        {
//          segTime[3]=counter;
//        }
//     }
  }

void runOp()
{ 
//  counter=speedValue();
  digitalWrite(motorEn,HIGH);
  digitalWrite(motorSp,counter);
  EEPROM.write(0,counter);
  long timeSec=timeCount*60;
  while(timeSec>0)
  {
    delay(1000);
    timeSec--;
    if(timeSec%60==0)
    {
      timeCount--;
      timeWrite();
    }
    timeSec--;
  }
  digitalWrite(motorEn,LOW);
  digitalWrite(motorSp,0);
  Serial.println("Process has ended");
}

int speedValue()
{
    float speedDec=counter*.85;
    Serial.print("before Convertion : ");
    Serial.println(speedDec);
    Serial.println();
    Serial.print("After Converstion : ");
    Serial.println(static_cast<int>(floor(speedDec)));
    return static_cast<int>(floor(speedDec));
}
