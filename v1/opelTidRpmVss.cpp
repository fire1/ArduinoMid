/*
 * Sepp1593
 * http://forum.arduino.cc/index.php?topic=78634.90

 Pin 5 on arduino = scl ,pin 9
Pin 6 on arduino = mrq ,pin 10
Pin 7 on arduino = sda ,pin 11

Pin 8 (= speed pin) on arduino is connected to pin 12 of the tid connector.
Pin 9  (= rpm pin) on arduino is connected to pin 9 on the tachometer plug behind the rev counter (the 4th pin on the left lower side of the connector, i think it's a green wire).

Both pins 8 and 9 use 22 KOhm resistors.

The arduino gets the supply voltage from pin 8 on the tid connector (--> Radio can switch arduino on and off).

Ground to pin 4.

 */


#include <TID12.h>

TID MyTid(7,5,6);

const int sdv = 8;
const int rpm = 9;

int sdvvalue;
int rpmvalue;

float sdvfloat;
float rpmfloat;

int sdvstate = 0;
int rpmstate = 0;
int sdvstateold = 0;
int rpmstateold = 0;
int rpmstatestart = 0;

int rpmstart = 0;
int sdvstart = 0;

int sdvhcount = 0;
float sdvhmax = 0;
int sdvhimax = 0;
int rpmhcount = 0;

int sdvbreak = 0;
int rpmbreak = 0;

int rpm2s = 0;
int rpm3s = 0;

int rpmh = 0;
int rpmh2 = 0;

int sdv2s = 0;

int sdvh = 0;

unsigned long time;
unsigned long timeold;
unsigned long timediff;
unsigned long timediff2;

char rpmdisp[4];
char sdvdisp[3];

void setup()
{
  MyTid.space1_init(0,3);
  MyTid.space2_init(5,7);
  pinMode(sdv, INPUT);
  pinMode(rpm, INPUT);
}

void loop()

{
  timeold = micros();
  while (rpmstart == LOW)
    {
      time = micros();                                      //Engines RPM
      timediff2 = time - timeold;
      if (timediff2 > 40000)
        rpmstart = 1;
      rpmstateold = rpmstate;
      rpmstate = digitalRead(rpm);
      delayMicroseconds(150);
      if (rpmstate == HIGH && rpmstateold == LOW)            //wait for rising edge
        rpmstart = 1;
    }



  timeold = micros();
  while(rpmhcount < 5)
    {
      rpmstateold = rpmstate;
      rpmstate = digitalRead(rpm);
      delayMicroseconds(150);
      if (rpmstate == HIGH && rpmstateold == LOW)        //wait for rising edge of signal
        rpmhcount++;
      time = micros();                                    //caching old time value...
      timediff2 = time - timeold;                        //
      if (timediff2 > 187500)                            //--> RPM < 800/min
        {
          rpmbreak = 1;
          break;
        }
    }
  time = micros();

  rpmstart = 0;

  rpmhcount = 0;

  timediff = time - timeold;

  rpmfloat = float(timediff);

  rpmfloat = (1 / (rpmfloat / 1000000)) * 150;          //calculate

  rpmvalue = int(rpmfloat);

  rpmh = rpmvalue / 1000;

  rpmh2 = rpmvalue / 100;

  rpm2s = rpmh2 - (rpmh * 10);

  rpm3s = (rpmvalue / 10) - (rpmh2 * 10);

  rpmdisp[0] = '0' + rpmh;
  rpmdisp[1] = '0' + rpm2s;
  rpmdisp[2] = '0' + rpm3s;
  rpmdisp[3] = '0' + (rpmvalue % 10);
  rpmdisp[4] = '\0';

  if (rpmbreak == HIGH)                                //display 0000 if rpm < 800
    {
      rpmdisp[0] = '0';
      rpmdisp[1] = '0';
      rpmdisp[2] = '0';
      rpmdisp[3] = '0';
    }

  rpmbreak = 0;

  MyTid.display_message(rpmdisp,1,1);                 //send to display...


  sdvhmax = (7 * sdvfloat) / 10;                        //calculate speed value based on previous measurement for increased accuaracy

  sdvhimax = int(sdvhmax);                              //round...

  sdvhmax = float(sdvhimax);

  if (sdvhmax < 5)                                      //minimum speed is about 3 km/h
    sdvhmax = 5;



  timeold = micros();                                  //start 
  while (sdvstart == LOW)
    {
      time = micros();
      timediff2 = time - timeold;
      if (timediff2 > 26000)
        sdvstart = 1;
      sdvstateold = sdvstate;
      sdvstate = digitalRead(rpm);
      delayMicroseconds(150);
      if (sdvstate == HIGH && sdvstateold == LOW)        //wait for rising edge
        sdvstart = 1;
    }


  timeold = micros();
  while(sdvhcount < sdvhmax)
    {
      sdvstateold = sdvstate;
      sdvstate = digitalRead(sdv);
      delayMicroseconds(150);
      if (sdvstate == HIGH && sdvstateold == LOW)
        sdvhcount++;
      //ab hier
      time = micros();                                  //cache old time
      timediff2 = time - timeold;
      if (timediff2 > 247500)                          //if car too slow --> 000
        {
          sdvbreak = 1;
          break;
        }

    }
  time = micros();

  sdvhcount = 0;

  sdvstart = 0;

  timediff = time - timeold;

  sdvfloat = float(timediff);

  sdvfloat = ((1 / (sdvfloat / 1000000)) * 1.96875) * (sdvhmax/10) * 1.05;      //(1.750 / 32) * 10 = 0.546875  ,    3.6*0.546875  ,  (* 1.05 to increase accuaracy)  , accuarate for 165/70 R13 tyre dimensions

  sdvvalue = int(sdvfloat);

  sdvh = sdvvalue / 100;

  sdv2s = (sdvvalue / 10) - sdvh * 10;

  sdvdisp[0] = '0' + sdvh;
  sdvdisp[1] = '0' + sdv2s;
  sdvdisp[2] = '0' + (sdvvalue % 10);
  sdvdisp[3] = '\0';

  if (sdvbreak == HIGH)                              //display 000 if car is too slow
    {
      sdvdisp[0] = '0';
      sdvdisp[1] = '0';
      sdvdisp[2] = '0';
    }

  sdvbreak = 0;

  MyTid.display_message(sdvdisp,1,2);                //send to display...
}