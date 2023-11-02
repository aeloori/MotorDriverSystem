
//importing libraries into code

#include <EEPROM.h>
#include <TM1637.h>


// motor breakout

#define motorPin 6; // Enable pin
#define directionPin 7; // Z/f for direction of motor spin
#define speedPin 8; // VR for speed of the motor

/* note : no need to connect signal pin to arduino as we are not using feedback from the motor to perfom the operation */

// Rotary encoder Breakout
#define rpmE1 9;
#define rpmE2 10;
#define rpmBtn 11;

//start/Stop button
#define opBtn 7;


//Variables
int rpmCounter=0;
int rpmState;
int rpmLastState;

//4 digits 7 segment display breakout
TM1637 rpmDisplay(2,3);
TM1637 timeDisplay(4,5);

void setup()
{
  //setting up display modules
  
  rpmDisplay.begin(); //starting the display for rpm
  rpmDisplay.setBrightness(4); //setting the display brightness to level 4
  
  timeDisplay.begin(); //starting the display for time
  timeDisplay.setBrightness(4); //setting the display brightness to level 4

  pinMode(rpmE1,INPUT);
  pinMode(rpmE2,INPUT);
  check(); // checking previous power cut
}

void loop()
{
  getRPMData();
}

// getting data from rotary encoder

void getRPMData(){
  while(rpmBtn!=HIGH)
  {
    rpmState=digitalRead(rpmE1);
    if(rpmState!=rpmLastState){
      if(digalRead(rpmE2!=rpmState)
      {
        rpmCounter+=10;
        rpmDisplay.print(rpmCounter);
      }
      else
      {
        rpmCounter-=10;
        rpmDisplay.print(rpmCounter);
      }
    }
    rpmLastState=rpmState;
  }
}

//running the motor for desired rpm and time

void processStart(float rpm,float duration)
{
  //ADD Speed using rpm
  rpmDisplay.print(rpm);
  timeDisplay.print(duration);
  float timeCounter=0;

  digitalWrite(motorPin,HIGH);

  //ADD start button or stop
  while(timeCounter<=duration*60)
  {
    delay(1000);
    timeCounter++;
    EEPROM.update(1,timeCounter/60); //storing time value into eeprom for power loss resque
    EEPROM.update(0,rpm/10); // storing rpm value into eeprom for power loss resque
  }
  
}

//  --check the rpm and time saved in eeprom memory when the system gets shut down

void check()
{
  float savedRPM=EEPROM.read(0); // rpm memory location
  float savedTime=EEPROM.read(1); //time memory location
  if(savedRPM!= 255  && savedTime!=255 ||  savedRPM!=0 &&  savedTime!=0)
  {
      processStart(savedRPM,savedTime); // sending rpm and time parameters to begin the process
  }
  else()
  {
    EEPROM.write(0,0);
    EEPROM.write(1,0);
  }
}
