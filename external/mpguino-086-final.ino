#include <Wire.h>
#include "LiquidCrystal.h"

LiquidCrystal lcd (4, 5, 7, 8, 12, 13);

int analogInput = 2;
int prev = LOW;
int refresh = 1000;
// float vout = 0.0;
float vin = 0.0;
//float R1 = 51000.0;    // !! resistance of R1 !!
// float R2 = 5500.0;     // !! resistance of R2 !!
// #define VoltageDivider(V);// 6153L // (50k1hm+4k4hm) / 4kOhm * 5V / 1023 * 100000opel
int sensorPin = 1;    // select the input pin for the potentiometer
int sensorValue = 0;         // the sensor value
int sensorMin = 0;        // full tank
int sensorMax = 1023;  //empty tank
int DRL = 10;
int value = 0;

byte pompa1[8] = {B00000, B00011, B00010, B00010, B00011, B00011, B00011, B00111};
byte pompa2[8] = {B00000, B11000, B01100, B01010, B11010, B11100, B11000, B11100};
byte diametru[8] = {B00000, B01101, B10010, B10101, B01001, B10110, B00000, B00000};
byte spate[8] = {B00000, B00000, B00001, B00010, B01111, B01111, B01101, B00010};
byte mijloc[8] = {B00000, B11111, B00000, B00000, B11111, B11111, B11111, B00000};
byte fata[8] = {B00000, B00000, B10000, B01000, B11110, B11111, B10111, B01000};

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
typedef uint8_t boolean;
typedef uint8_t byte;
#define RISING 3
#define FALLING 2
#define loopsPerSecond 2 // how many times will we try and loop in a second
//use with 20mhz
#define cyclesperhour 4500
#define dispadj 800
#define dispadj2 1250
#define looptime 1250000ul/loopsPerSecond //1/2 second
#define myubbr (20000000/16/9600-1)
#define injhold (parms[injectorSettleTimeIdx]*5)/4
#define outhi(port, pin) PORT##port |= ( 1 << P##port##pin )
#define outlo(port, pin) PORT##port &= ~( 1 << P##port##pin )
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

void enableLButton ();
void enableMButton ();
void enableRButton ();
void addEvent (byte eventID, unsigned int ms);
unsigned long microSeconds (void);
unsigned long elapsedMicroseconds (unsigned long startMicroSeconds,
				   unsigned long currentMicroseconds);
unsigned long elapsedMicroseconds (unsigned long startMicroSeconds);
void processInjOpen (void);
void processInjClosed (void);
void enableVSS ();
void setup (void);
void mainloop (void);
char *format (unsigned long num);
char *getStr (prog_char *str);
void doDisplayCustom ();
void doDisplayBigInstant ();
void doDisplayBigCurrent ();
void doTime ();
void doDisplayTotal ();
void doDisplayBigSpeed ();
void doDisplayBigTank ();
void doDisplayBigDistance ();
void doDisplayBigVolt ();
void doDisplaySystemInfo (void);
void displayTripCombo (char *lu1, char *lm1, unsigned long v1, char *lu2, char *lm2, unsigned long v2,
		       char *lu3, char *lm3, unsigned long v3, char *lu4, char *lm4, unsigned long v4);
void tDisplay (void *r);
int memoryTest ();
unsigned long instantmph ();
unsigned long instantmpg ();
unsigned long instantgph ();
//void bigNum(unsigned long t, char * txt1, const char * txt2);
void init64 (unsigned long an[], unsigned long bigPart,
	     unsigned long littlePart);
void shl64 (unsigned long an[]);
void shr64 (unsigned long an[]);
void add64 (unsigned long an[], unsigned long ann[]);
void sub64 (unsigned long an[], unsigned long ann[]);
boolean eq64 (unsigned long an[], unsigned long ann[]);
boolean lt64 (unsigned long an[], unsigned long ann[]);
void div64 (unsigned long num[], unsigned long den[]);
void mul64 (unsigned long an[], unsigned long ann[]);
void save ();
byte load ();
char *uformat (unsigned long val);
unsigned long rformat (char *val);
void editParm (byte parmIdx);
void initGuino ();
unsigned long millis2 ();
void delay2 (unsigned long ms);
void delayMicroseconds2 (unsigned int us);

void simpletx (char *string);

unsigned long parms[] = {95ul, 17923ul, 3669000ul, 3ul, 21ul, 1240ul, 1ul, 0ul, 50ul, 80000000, 4200000, 1ul};//opel



char *parmLabels[] = {"Contrast", "VSS Pulses/Km", "MicroSec/Litre", "Pulses/2 revs", "Injector DelayuS",
		      "Voltage * 1000  ", "VSS Delay ms", "InjTrg 0-Dn 1-Up", "    Tank L    ", "TankKm*100.000 ",
		      "Tank L*100.000", "DRL 1=ON 0=OFF"};
//"0-USA 1-METRIC","Tank Km * 1000","Tank Litres*1000"};


byte brightness[] = {0, 15, 128, 255}; //middle button cycles through these brightness settings
#define brightnessLength (sizeof(brightness)/sizeof(byte)) //array size
byte brightnessIdx = 3;
#define contrastIdx 0  //do contrast first to get display dialed in
#define vssPulsesPerMileIdx 1
volatile unsigned long distancefactor;
#define microSecondsPerGallonIdx 2
volatile unsigned long fuelfactor;
#define injPulsesPer2Revolutions 3
#define injectorSettleTimeIdx 4
#define VoltageIdx 5
volatile unsigned long Coef;
#define vsspauseIdx 6
#define injEdgeIdx 7
#define tankSizeIdx 8
volatile unsigned long L;
#define calibrationDistanceIdx 9
volatile unsigned long tmptankdistance;
#define calibrationFuelIdx 10
volatile unsigned long tmptankfuel;
#define metricIdx=1;
#define drlIdx 11
#define parmsLength (sizeof(parms)/sizeof(unsigned long)) /*  //array size      */

unsigned long TimeoutBrightness = 36000000;
//30 sec
unsigned long TimeoutTrip = 72000000 * 30;
//min * x
unsigned long injectorSettleTime;
unsigned long tmpDRL = parms[drlIdx];
unsigned long V = (parms[VoltageIdx] * 7982L) / parms[VoltageIdx];
#define nil 3999999999ul
#define guinosigold 0b10100101
#define guinosig 0b11100111
#define vssBit ( 1 << 0 )
#define lbuttonBit ( 1 << 3 )
#define mbuttonBit ( 1 << 4 )
#define rbuttonBit ( 1 << 5 )

typedef void (*pFunc) (void);//type for display function pointers

volatile unsigned long timer2_overflow_count;

/*** Set up the Events ***
 We have our own ISR for timer2 which gets called about once a millisecond.
 So we define certain event functions that we can schedule by calling addEvent
 with the event ID and the number of milliseconds to wait before calling the event.
 The milliseconds is approximate.

 Keep the event functions SMALL!!!  This is an interrupt!

 */
//event functions

void enableLButton () {
  PCMSK1 |= (1 << PCINT11);
}
void enableMButton () {
  PCMSK1 |= (1 << PCINT12);
}
void enableRButton () {
  PCMSK1 |= (1 << PCINT13);
}
//array of the event functions
pFunc eventFuncs[] = {enableVSS, enableLButton, enableMButton, enableRButton};
#define eventFuncSize (sizeof(eventFuncs)/sizeof(pFunc))
//define the event IDs
#define enableVSSID 0
#define enableLButtonID 1
#define enableMButtonID 2
#define enableRButtonID 3
//ms counters



unsigned int eventFuncCounts[eventFuncSize];

//schedule an event to occur ms milliseconds from now
void addEvent (byte eventID, unsigned int ms) {
  if (ms == 0)
    eventFuncs[eventID] ();
  else
    eventFuncCounts[eventID] = ms;
}

/* this ISR gets called every 1.024 milliseconds, we will call that a millisecond for our purposes
 go through all the event counts,
 if any are non zero subtract 1 and call the associated function if it just turned zero.  */
ISR(TIMER2_OVF_vect) {
  timer2_overflow_count++;
  for (byte eventID = 0; eventID < eventFuncSize; eventID++)
    {
      if (eventFuncCounts[eventID] != 0)
	{
	  eventFuncCounts[eventID]--;
	  if (eventFuncCounts[eventID] == 0)
	    eventFuncs[eventID] ();
	}
    }
}

unsigned long maxLoopLength = 0; //see if we are overutilizing the CPU


#define buttonsUp   lbuttonBit + mbuttonBit + rbuttonBit  // start with the buttons in the right state
byte buttonState = buttonsUp;

//overflow counter used by millis2()
unsigned long lastMicroSeconds = millis2 () * 1000;
unsigned long microSeconds (void) {
  unsigned long tmp_timer2_overflow_count;
  unsigned long tmp;
  byte tmp_tcnt2;
  cli();
  //disable interrupts
  tmp_timer2_overflow_count = timer2_overflow_count;
  tmp_tcnt2 = TCNT2;
  sei();
  // enable interrupts
  tmp = ((tmp_timer2_overflow_count << 8) + tmp_tcnt2) * 4;
  if ((tmp <= lastMicroSeconds) && (lastMicroSeconds < 4290560000ul))
    return microSeconds ();
  lastMicroSeconds = tmp;
  return tmp;
}

unsigned long elapsedMicroseconds (unsigned long startMicroSeconds,
				   unsigned long currentMicroseconds) {
  if (currentMicroseconds >= startMicroSeconds)
    return currentMicroseconds - startMicroSeconds;
  return 4294967295 - (startMicroSeconds - currentMicroseconds);
}

unsigned long elapsedMicroseconds (unsigned long startMicroSeconds) {
  return elapsedMicroseconds (startMicroSeconds, microSeconds ());
}

//Trip prototype
class Trip {
 public:
  unsigned long loopCount; //how long has this trip been running
  unsigned long injPulses; //rpm
  unsigned long injHiSec;
  // seconds the injector has been open
  unsigned long injHius;
  // microseconds, fractional part of the injectors open
  unsigned long injIdleHiSec;
  // seconds the injector has been open
  unsigned long injIdleHius;
  // microseconds, fractional part of the injectors open
  unsigned long vssPulses;
  //from the speedo
  unsigned long vssEOCPulses;
  //from the speedo
  unsigned long vssPulseLength; // only used by instant
  //these functions actually return in thousandths,
  unsigned long distance ();
  unsigned long fuel ();
  unsigned long lkm ();
  unsigned long mpg ();
  unsigned long mph ();
  unsigned long time (); //mmm.ss
  unsigned long timeS ();
  unsigned long timeM ();
  unsigned long timeH ();
  unsigned m;
  unsigned m1;
  unsigned h;
  unsigned sec;
  unsigned long fuelperdistanceunit (unsigned long multiplier);
  unsigned long Range ();
  unsigned long eocMiles (); //how many "free" miles?
  unsigned long idleGallons (); //how many gallons spent at 0 mph?
  void update (Trip t);
  void reset ();
  Trip ();
};

//LCD prototype
namespace LCD
{
    void gotoXY (byte x, byte y);
    void print (char *string);
    void init ();
    void print (byte value);
    void tickleEnable ();
    void cmdWriteSet ();
    void LcdCommandWrite (byte value);
    void LcdDataWrite (byte value);
    byte pushNibble (byte value);
};

//main objects we will be working with:
unsigned long injHiStart; //for timing injector pulses
Trip tmpTrip;
Trip instant;
Trip current;
Trip tank;

unsigned volatile long instInjStart = nil;
unsigned volatile long tmpInstInjStart = nil;
unsigned volatile long instInjEnd;
unsigned volatile long tmpInstInjEnd;
unsigned volatile long instInjTot;
unsigned volatile long tmpInstInjTot;
unsigned volatile long instInjCount;
unsigned volatile long tmpInstInjCount;

volatile static pFunc int0Func;
ISR(INT0_vect) { //processInjOpen by default
  int0Func ();
}

volatile static pFunc int1Func;
ISR(INT1_vect) {//processInjClosed
  int1Func ();
}

void processInjOpen (void) {
  injHiStart = microSeconds ();
}

void processInjClosed (void) {
  long t = microSeconds ();
  long x = elapsedMicroseconds (injHiStart, t) - injectorSettleTime;
  if (x > 0)
    tmpTrip.injHius += x;
  tmpTrip.injPulses++;

  if (tmpInstInjStart != nil)
    {
      if (x > 0)
	tmpInstInjTot += x;
      tmpInstInjCount++;
    }
  else
    {
      tmpInstInjStart = t;
    }

  tmpInstInjEnd = t;
}

volatile boolean vssFlop = 0;

void enableVSS () {
  //    tmpTrip.vssPulses++;
  vssFlop = !vssFlop;
}

unsigned volatile long lastVSS1;
unsigned volatile long lastVSSTime;
unsigned volatile long lastVSS2;

volatile boolean lastVssFlop = vssFlop;

//attach the vss/buttons interrupt
ISR(PCINT1_vect) {
  static byte vsspinstate = 0;
  byte p = PINC;//bypassing digitalRead for interrupt performance
  if ((p & vssBit) != (vsspinstate & vssBit))
    {
      addEvent (enableVSSID, parms[vsspauseIdx]); //check back in a couple milli
    }
  if (lastVssFlop != vssFlop)
    {
      lastVSS1 = lastVSS2;
      unsigned long t = microSeconds ();
      lastVSS2 = elapsedMicroseconds (lastVSSTime, t);
      lastVSSTime = t;
      tmpTrip.vssPulses++;
      tmpTrip.vssPulseLength += lastVSS2;
      lastVssFlop = vssFlop;
    }
  vsspinstate = p;
  buttonState &= p;
}

pFunc displayFuncs[] = {doDisplayCustom, doDisplayInstantHour, doDisplayConsumTrip, doDisplayCurDistance,
			doDisplayTripConsum,
			doDisplayTripTime, doDisplayDRL, doDisplaySpeed, doDisplayConsumTank, doDisplayTankDistance,
			doDisplayTankConsum, doDisplayTotalTime, doDisplayRange,};

#define displayFuncSize (sizeof(displayFuncs)/sizeof(pFunc)) //array size
prog_char *displayFuncNames[displayFuncSize];
byte newRun = 0;
void setup (void) {
  
  pinMode (DRL, OUTPUT);
  newRun = load ();//load the default parameters
  byte x = 0;
  LCD::LcdCommandWrite (0b00000001); // clear display, set cursor position to zero
  displayFuncNames[x++] = PSTR("                ");
  displayFuncNames[x++] = PSTR("                ");
  displayFuncNames[x++] = PSTR("                ");
  displayFuncNames[x++] = PSTR("                ");
  displayFuncNames[x++] = PSTR("                ");
  displayFuncNames[x++] = PSTR("                ");
  displayFuncNames[x++] = PSTR("                ");
  displayFuncNames[x++] = PSTR("                ");
  displayFuncNames[x++] = PSTR("                ");
  displayFuncNames[x++] = PSTR("                ");
  displayFuncNames[x++] = PSTR("                ");
  displayFuncNames[x++] = PSTR("                ");
  displayFuncNames[x++] = PSTR("                ");


  //  analogWrite(BrightnessPin,brightness[brightnessIdx]);
  sbi(TCCR1A, COM1A1); //brightness pwm enable
  tmpDRL = parms[drlIdx];

  OCR1A = brightness[brightnessIdx];

  DDRB = (1 << DDB5) | (1 << DDB4) | (1 << DDB1) | (1 << DDB0);
  DDRD = (1 << DDD7) | (1 << DDD6) | (1 << DDD5) | (1 << DDD4);

  delay2 (500);
  V = (parms[VoltageIdx] * 7982L) / parms[VoltageIdx];

  //  analogWrite(ContrastPin,parms[contrastIdx]);
  sbi(TCCR0A, COM0A1);//contrast pwm enable
  OCR0A = parms[contrastIdx];

  LCD::init ();

  LCD::LcdCommandWrite (0b00000001); // clear display, set cursor position to zero
  LCD::LcdCommandWrite (0b10000); // set dram to zero
  LCD::gotoXY (0, 0);
  LCD::print (getStr (PSTR("  Opel Astra F  ")));
  LCD::gotoXY (0, 1);
  LCD::print (getStr (PSTR("  MPGuino  v0.86")));

  injectorSettleTime = injhold;
  int0Func = processInjOpen;
  int1Func = processInjClosed;

  //set up the external interrupts
  EICRA = (EICRA & ~((1 << ISC00) | (1 << ISC01)))
	  | ((parms[injEdgeIdx] == 1 ? RISING : FALLING) << ISC00);
  EIMSK |= (1 << INT0);
  EICRA = (EICRA & ~((1 << ISC10) | (1 << ISC11)))
	  | ((parms[injEdgeIdx] == 1 ? FALLING : RISING) << ISC10);
  EIMSK |= (1 << INT1);

  PORTC |= (1 << 5) | (1 << 4) | (1 << 3); //button pullup resistors

  unsigned long Volt = voltage ();

  V = (parms[VoltageIdx] * 7982L) / parms[VoltageIdx];
  distancefactor = parms[vssPulsesPerMileIdx];
  fuelfactor = parms[microSecondsPerGallonIdx];
  tmpDRL = parms[drlIdx];
  L = parms[tankSizeIdx];                //distancefactor = (distancefactor * 1.609344);
  fuelfactor = (fuelfactor * 3.78541178);


  //low level interrupt enable stuff
  PCMSK1 |= (1 << PCINT8);
  enableLButton ();
  enableMButton ();
  enableRButton ();
  PCICR |= (1 << PCIE1);

  delay2 (1500);
}

byte screen = 0;
byte holdDisplay = 0;

void mainloop (void) {
  if (newRun != 1)
    initGuino ();//go through the initialization screen
  unsigned long lastActivity = microSeconds ();
  unsigned long tripHold;
  unsigned long tankHold; //state at point of last activity
  while (true)
    {
      unsigned long loopStart = microSeconds ();
      instant.reset (); //clear instant
      cli();
      instant.update (tmpTrip); //"copy" of tmpTrip in instant now
      tmpTrip.reset (); //reset tmpTrip first so we don't lose too many interrupts
      instInjStart = tmpInstInjStart;
      instInjEnd = tmpInstInjEnd;
      instInjTot = tmpInstInjTot;
      instInjCount = tmpInstInjCount;

      tmpInstInjStart = nil;
      tmpInstInjEnd = nil;
      tmpInstInjTot = 0;
      tmpInstInjCount = 0;

      sei();

      //send out instantmpg * 1000, instantmph * 1000, the injector/vss raw data
      simpletx (format (instantmpg ()));
      simpletx (",");
      simpletx (format (instantmph ()));
      simpletx (",");
      simpletx (format (instant.injHius * 1000));
      simpletx (",");
      simpletx (format (instant.injPulses * 1000));
      simpletx (",");
      simpletx (format (instant.vssPulses * 1000));
      simpletx ("\n");

      current.update (instant); //use instant to update current
      tank.update (instant); //use instant to update tank


      if (instant.vssPulses == 0 && instant.injPulses == 0 && holdDisplay
							      == 0)
	{
	  if (elapsedMicroseconds (lastActivity)
	      > TimeoutBrightness && lastActivity
				     != nil)
	    {
	      OCR1A = brightness[0];
	      tank.loopCount = tankHold;
	      current.loopCount = tripHold;
	      lcd.noDisplay ();
	    }
	}
      else
	{
	  if (lastActivity != nil)
	    {//wake up!!!
	      OCR1A = brightness[brightnessIdx];
	      tankHold = tank.loopCount;
	      tripHold = current.loopCount;
	      lcd.display ();
	    }
	}

      if (instant.vssPulses == 0 && instant.injPulses == 0 && holdDisplay
							      == 0)
	{
	  if (elapsedMicroseconds (lastActivity)
	      > TimeoutTrip && lastActivity
			       != nil)
	    {
	      lastActivity = nil;
	      //  lcd.noDisplay();
	    }
	}
      else
	{
	  if (lastActivity == nil)
	    {//wake up!!!
	      lcd.display ();
	      OCR1A = brightness[brightnessIdx];
	      lastActivity = loopStart;
	      current.reset ();
	      tank.loopCount = tankHold;
	      current.update (instant);
	      tank.update (instant);
	    }
	  else
	    {
	      lastActivity = loopStart;
	      tankHold = tank.loopCount;

	    }
	}

      if (holdDisplay == 0)
	{
	  displayFuncs[screen] (); //call the appropriate display routine
	  LCD::gotoXY (0, 0);

	  //see if any buttons were pressed, display a brief message if so
	  if (!(buttonState & lbuttonBit) && !(buttonState & rbuttonBit))
	    {// left and right = initialize
	      LCD::LcdCommandWrite (0b00000001); // clear display, set cursor position to zero
	      LCD::print (getStr (PSTR("Setup ")));
	      delay (100000);
	      // set values for math-less calibration method - which is used when refilling the tank
	      unsigned long tmptankdistance = tank.distance ();   // take a snapshot, in case update occurs during the updates
	      unsigned long tmptankfuel = tank.fuel ();
	      int tmpdirty = 0;

	      unsigned long Volt = voltage ();
	      parms[calibrationDistanceIdx] = tmptankdistance;   // distance x 1000, for updating parms[vssPulsesPerMileIdx]
	      parms[calibrationFuelIdx] = tmptankfuel;           // fuel x 1000, for updating parms[microSecondsPerGallonIdx]
	      parms[drlIdx] = tmpDRL;
	      initGuino ();
	      //}else if(!(buttonState&lbuttonBit) && !(buttonState&rbuttonBit)){// left and right = run lcd init = tank reset
	      //    LCD::print(getStr(PSTR("Init LCD ")));
	      //    LCD::init();



	      L = parms[tankSizeIdx];

	      V = (parms[VoltageIdx] * 7982L) / 1200L;
	      if (parms[calibrationDistanceIdx] != tmptankdistance)
		{
		  parms[vssPulsesPerMileIdx] = recalculate (parms[vssPulsesPerMileIdx], tmptankdistance, parms[calibrationDistanceIdx]);
		  tmpdirty = 1;
		}

	      if (parms[calibrationFuelIdx] != tmptankfuel)
		{
		  parms[microSecondsPerGallonIdx] = recalculate (parms[microSecondsPerGallonIdx], tmptankfuel, parms[calibrationFuelIdx]);
		  tmpdirty = 1;
		}

	      if (tmpdirty)
		save ();    // save recalculated settings
	    }
	  else if (!(buttonState & lbuttonBit) && !(buttonState
						    & mbuttonBit))
	    {// left and middle = tank reset
	      tank.reset ();
	      LCD::LcdCommandWrite (0b00000001); // clear display, set cursor position to zero
	      LCD::print (getStr (PSTR("Tank Reset ")));
	      delay (100000);
	    }
	  else if (!(buttonState & mbuttonBit) && !(buttonState
						    & rbuttonBit))
	    {// right and middle = current reset
	      current.reset ();
	      LCD::LcdCommandWrite (0b00000001); // clear display, set cursor position to zero
	      LCD::print (getStr (PSTR("Current Reset ")));
	      delay (100000);
	    }
	  else if (!(buttonState & lbuttonBit))
	    { //left is rotate through screeens to the left
	      if (screen != 0)
		screen = (screen - 1);
	      else
		screen = displayFuncSize - 1;
	      LCD::print (getStr (displayFuncNames[screen]));
	    }
	  else if (!(buttonState & mbuttonBit))
	    { //middle is cycle through brightness settings
	      brightnessIdx = (brightnessIdx + 1) % brightnessLength;
	      OCR1A = brightness[brightnessIdx];
	      //        analogWrite(BrightnessPin,brightness[brightnessIdx]);
	      LCD::LcdCommandWrite (0b00000001); // clear display, set cursor position to zero
	      LCD::print (getStr (PSTR("Brightness ")));
	      LCD::LcdDataWrite ('0' + brightnessIdx);
	      LCD::print (" ");
	    }
	  else if (!(buttonState & rbuttonBit))
	    {//right is rotate through screeens to the left
	      screen = (screen + 1) % displayFuncSize;
	      LCD::print (getStr (displayFuncNames[screen]));
	    }
	  if (buttonState != buttonsUp)
	    holdDisplay = 1;
	}
      else
	{
	  holdDisplay = 0;
	}
      buttonState = buttonsUp;//reset the buttons

      //keep track of how long the loops take before we go int waiting.
      unsigned long loopX = elapsedMicroseconds (loopStart);
      if (loopX > maxLoopLength)
	maxLoopLength = loopX;

      while (elapsedMicroseconds (loopStart) < (looptime));//wait for the end of a second to arrive
    }

  lcd.begin (16, 2);
}

char fBuff[7];//used by format

void dispv (char *usl, char *ml, unsigned long num) {
  //LCD::print(parms[metricIdx]==1?ml:usl);
  LCD::print (format (num));
}
void dispv_raw (char *usl, char *ml, unsigned long num) {
  //LCD::print(parms[metricIdx]==1?ml:usl);
  LCD::print (num);
}

char *format (unsigned long num) {
  byte dp = 3;

  while (num > 999999)
    {
      num /= 10;
      dp++;
      if (dp == 5)
	break; // We'll lose the top numbers like an odometer
    }
  if (dp == 5)
    dp = 99; // We don't need a decimal point here.

  // Round off the non-printed value.
  if ((num % 10) > 4)
    num += 10;
  num /= 10;
  byte x = 6;
  while (x > 0)
    {
      x--;
      if (x == dp)
	{ //time to poke in the decimal point?{
	  fBuff[x] = '.';
	}
      else
	{
	  fBuff[x] = '0' + (num % 10);//poke the ascii character for the digit.
	  num /= 10;
	}
    }
  //fBuff[3] = 0;
  fBuff[6] = 0;
  return fBuff;
}
//get a string from flash
char mBuff[17];
//used by getStr
char *getStr (prog_char *str) {
  strcpy_P (mBuff, str);
  return mBuff;
}

char *formatt (unsigned long num) {
  byte dp = 1;

  while (num > 999999)
    {
      num /= 10;
      dp++;
      if (dp == 5)
	break; // We'll lose the top numbers like an odometer
    }
  if (dp == 5)
    dp = 99; // We don't need a decimal point here.

  // Round off the non-printed value.
  if ((num % 10) > 1)
    num += 10;
  num /= 10;
  byte x = 6;
  while (x > 0)
    {
      x--;
      if (x == dp)
	{ //time to poke in the decimal point?{
	  fBuff[x] = '.';
	}
      else
	{
	  fBuff[x] = '0' + (num % 10);//poke the ascii character for the digit.
	  num /= 10;
	}
    }
  //fBuff[3] = 0;
  fBuff[4] = 0;
  return fBuff;
}

void doDisplayCustom () {
  char buffer[50];
  LCD::print ("R ");
  LCD::print (ltoa (instantrpm (), buffer, 10));
  LCD::print ("  SPD ");
  LCD::print (ltoa (instantmph (), buffer, 10));
  LCD::print ("       ");
  LCD::gotoXY (0, 1);
  LCD::print ("Volt ");
  LCD::print (formatt (voltage ()));
  LCD::print ("       ");
  if (instant.vssPulses > 10 && voltage () > 1380000 && tmpDRL == 1) digitalWrite (DRL, HIGH);//set volt to open DRL
  if (instant.injPulses == 0 && instant.vssPulses == 0) digitalWrite (DRL, LOW); //set volt to close DRL
}

void doDisplayInstantHour () {
  if (instant.vssPulses > 10 && voltage () > 1380000 && tmpDRL == 1) digitalWrite (DRL, HIGH);//set volt to open DRL
  if (instant.injPulses == 0 && instant.vssPulses == 0) digitalWrite (DRL, LOW); //set volt to close DRL
  if (instant.vssPulses < 5)
    LCD::gotoXY (0, 0);
  if (instant.vssPulses < 5)
    LCD::print ("  Instant Hour  ");
  if (instant.vssPulses < 5)
    LCD::gotoXY (0, 1);
  if (instant.vssPulses < 5)
    LCD::print ("     ");
  if (instant.vssPulses < 5)
    LCD::print (formatt (instantgph ()));
  if (instant.vssPulses < 5)
    LCD::print (" L/h      ");
  if (instant.vssPulses > 5)
    LCD::gotoXY (0, 0);
  if (instant.vssPulses > 5)
    LCD::print ("     Instant    ");
  if (instant.vssPulses > 5)
    LCD::gotoXY (0, 1);
  if (instant.vssPulses > 5)
    LCD::print ("     ");
  if (instant.vssPulses > 5)
    LCD::print (formatt (instantfuelperhour ()));
  if (instant.vssPulses > 5)
    LCD::print (" L%   ");

}

void doDisplayTripConsum () {
  if (instant.vssPulses > 10 && voltage () > 1380000 && tmpDRL == 1) digitalWrite (DRL, HIGH);//set volt to open DRL
  if (instant.injPulses == 0 && instant.vssPulses == 0) digitalWrite (DRL, LOW); //set volt to close DRL
  //char buffer[50];
  LCD::gotoXY (0, 0);
  LCD::print ("  Trip Consum   ");
  LCD::gotoXY (0, 1);
  LCD::print ("     ");
  LCD::print (formatt (current.fuel ()));
  LCD::print (" L    ");
}

void doDisplayTankConsum () {
  if (instant.vssPulses > 10 && voltage () > 1380000 && tmpDRL == 1) digitalWrite (DRL, HIGH);//set volt to open DRL
  if (instant.injPulses == 0 && instant.vssPulses == 0) digitalWrite (DRL, LOW); //set volt to close DRL
  lcd.createChar (2, pompa1);
  lcd.createChar (3, pompa2);
  lcd.createChar (4, spate);
  lcd.createChar (5, mijloc);
  lcd.createChar (6, fata);
  char buffer[50];
  LCD::gotoXY (0, 0);
  lcd.write (2);
  lcd.write (3);
  LCD::print ("....");
  lcd.write (4);
  lcd.write (5);
  lcd.write (6);
  LCD::print ("       ");

  LCD::gotoXY (0, 1);
  LCD::print ("     ");
  LCD::print (formatt (tank.fuel ()));
  LCD::print (" L    ");
}

void doDisplayRange () {
  if (instant.vssPulses > 10 && voltage () > 1380000 && tmpDRL == 1) digitalWrite (DRL, HIGH);//set volt to open DRL
  if (instant.injPulses == 0 && instant.vssPulses == 0) digitalWrite (DRL, LOW); //set volt to close DRL
  char buffer[50];
  lcd.createChar (2, pompa1);
  lcd.createChar (3, pompa2);
  lcd.createChar (4, spate);
  lcd.createChar (5, mijloc);
  lcd.createChar (6, fata);
  LCD::gotoXY (0, 0);
  LCD::print (" ");
  lcd.write (2);
  lcd.write (3);
  LCD::print ("    ");
  lcd.write (4);
  lcd.write (5);
  lcd.write (6);
  LCD::print ("....");
  lcd.write (2);
  lcd.write (3);
  LCD::print ("       ");
  LCD::gotoXY (0, 1);
  LCD::print (" ");
  LCD::print (ltoa (sensorValue, buffer, 10));
  LCD::print (" L     ");
  LCD::print (ltoa (current.Range (), buffer, 10));
  LCD::print (" Km     ");
}
void doDisplaySpeed () {
  if (instant.vssPulses > 10 && voltage () > 1380000 && tmpDRL == 1) digitalWrite (DRL, HIGH);//set volt to open DRL
  if (instant.injPulses == 0 && instant.vssPulses == 0) digitalWrite (DRL, LOW); //set volt to close DRL
  lcd.createChar (1, diametru);
  char buffer[50];
  LCD::gotoXY (0, 0);
  LCD::print ("Trip  Km/h  Tank ");
  LCD::gotoXY (0, 1);
  lcd.write (1);
  LCD::print (" ");
  LCD::print (ltoa (current.mph (), buffer, 10));
  LCD::print ("      ");
  lcd.write (1);
  LCD::print (" ");
  LCD::print (ltoa (tank.mph (), buffer, 10));
  LCD::print ("    ");

}
void doDisplayConsumTrip () {
  if (instant.vssPulses > 10 && voltage () > 1380000 && tmpDRL == 1) digitalWrite (DRL, HIGH);//set volt to open DRL
  if (instant.injPulses == 0 && instant.vssPulses == 0) digitalWrite (DRL, LOW); //set volt to close DRL
  lcd.createChar (1, diametru);
  LCD::gotoXY (0, 0);
  LCD::print ("      Trip     ");
  LCD::gotoXY (0, 1);
  LCD::print ("    ");
  lcd.write (1);
  LCD::print (" ");
  LCD::print (formatt (current.fuelperdistanceunit (100)));
  LCD::print (" L%    ");
}
void doDisplayTripTime () {
  if (instant.vssPulses > 10 && voltage () > 1380000 && tmpDRL == 1) digitalWrite (DRL, HIGH);//set volt to open DRL
  if (instant.injPulses == 0 && instant.vssPulses == 0) digitalWrite (DRL, LOW); //set volt to close DRL
  char buffer[50];
  LCD::gotoXY (0, 0);
  LCD::print ("      Trip      ");
  LCD::gotoXY (0, 1);
  LCD::print ("    ");
  if (current.timeH () < 10)
    LCD::print ("0");
  LCD::print (ltoa (current.timeH (), buffer, 10));
  LCD::print (":");
  if (current.timeM () < 10)
    LCD::print ("0");
  LCD::print (ltoa (current.timeM (), buffer, 10));
  LCD::print (":");
  if (current.timeS () < 10)
    LCD::print ("0");
  LCD::print (ltoa (current.timeS (), buffer, 10));
  LCD::print ("     ");
}

void doDisplayTotalTime () {
  if (instant.vssPulses > 10 && voltage () > 1380000 && tmpDRL == 1) digitalWrite (DRL, HIGH);//set volt to open DRL
  if (instant.injPulses == 0 && instant.vssPulses == 0) digitalWrite (DRL, LOW); //set volt to close DRL
  lcd.createChar (2, pompa1);
  lcd.createChar (3, pompa2);
  lcd.createChar (4, spate);
  lcd.createChar (5, mijloc);
  lcd.createChar (6, fata);
  char buffer[50];
  LCD::gotoXY (0, 0);
  lcd.write (2);
  lcd.write (3);
  LCD::print ("....");
  lcd.write (4);
  lcd.write (5);
  lcd.write (6);
  LCD::print ("       ");
  LCD::gotoXY (0, 1);
  LCD::print ("    ");
  if (tank.timeH () < 10)
    LCD::print ("0");
  LCD::print (ltoa (tank.timeH (), buffer, 10));
  LCD::print (":");
  if (tank.timeM () < 10)
    LCD::print ("0");
  LCD::print (ltoa (tank.timeM (), buffer, 10));
  LCD::print (":");
  if (tank.timeS () < 10)
    LCD::print ("0");
  LCD::print (ltoa (tank.timeS (), buffer, 10));
  LCD::print ("     ");
}

void doDisplayConsumTank () {
  if (instant.vssPulses > 10 && voltage () > 1380000 && tmpDRL == 1) digitalWrite (DRL, HIGH);//set volt to open DRL
  if (instant.injPulses == 0 && instant.vssPulses == 0) digitalWrite (DRL, LOW); //set volt to close DRL
  lcd.createChar (1, diametru);
  lcd.createChar (2, pompa1);
  lcd.createChar (3, pompa2);
  lcd.createChar (4, spate);
  lcd.createChar (5, mijloc);
  lcd.createChar (6, fata);
  LCD::gotoXY (0, 0);
  lcd.write (2);
  lcd.write (3);
  LCD::print ("....");
  lcd.write (4);
  lcd.write (5);
  lcd.write (6);
  LCD::print ("       ");
  LCD::gotoXY (0, 1);
  LCD::print ("   ");
  lcd.write (1);
  LCD::print (" ");
  LCD::print (formatt (tank.fuelperdistanceunit (100)));
  LCD::print (" L%     ");

}
void doDisplayCurDistance () {
  if (instant.vssPulses > 10 && voltage () > 1380000 && tmpDRL == 1) digitalWrite (DRL, HIGH);//set volt to open DRL
  if (instant.injPulses == 0 && instant.vssPulses == 0) digitalWrite (DRL, LOW); //set volt to close DRL
  char buffer[50];
  LCD::gotoXY (0, 0);
  LCD::print ("      Trip      ");
  LCD::gotoXY (0, 1);
  LCD::print ("     ");
  if (current.distance () < 9999900)
    LCD::print (formatt (current.distance ()));
  if (current.distance () > 9999900)
    LCD::print (ltoa (current.distance () / 100000, buffer, 10));
  LCD::print ("Km   ");

}

void doDisplayTankDistance () {
  if (instant.vssPulses > 10 && voltage () > 1380000 && tmpDRL == 1) digitalWrite (DRL, HIGH);//set volt to open DRL
  if (instant.injPulses == 0 && instant.vssPulses == 0) digitalWrite (DRL, LOW); //set volt to close DRL
  lcd.createChar (2, pompa1);
  lcd.createChar (3, pompa2);
  lcd.createChar (4, spate);
  lcd.createChar (5, mijloc);
  lcd.createChar (6, fata);
  char buffer[50];
  LCD::gotoXY (0, 0);
  lcd.write (2);
  lcd.write (3);
  LCD::print ("....");
  lcd.write (4);
  lcd.write (5);
  lcd.write (6);
  LCD::print ("       ");
  LCD::gotoXY (0, 1);
  LCD::print ("     ");
  if (tank.distance () < 9999900)
    LCD::print (formatt (tank.distance ()));
  if (tank.distance () > 9999900)
    LCD::print (ltoa (tank.distance () / 100000, buffer, 10));
  LCD::print ("Km   ");

}
void doDisplayDRL () {
  digitalWrite (DRL, LOW); //set volt to close DRL
  LCD::gotoXY (0, 0);
  LCD::print ("   LIGHTS OFF   ");
  LCD::gotoXY (0, 1);
  LCD::print ("   Volt ");
  LCD::print (formatt (voltage ()));
  LCD::print ("    ");
}

//x=0..16, y= 0..1
void LCD::gotoXY (byte x, byte y) {
  byte dr = x + 0x80;
  if (y == 1)
    dr += 0x40;
  if (y == 2)
    dr += 0x14;
  if (y == 3)
    dr += 0x54;
  LCD::LcdCommandWrite (dr);
}

void LCD::print (char *string) {
  byte x = 0;
  char c = string[x];
  while (c != 0)
    {
      LCD::LcdDataWrite (c);
      x++;
      c = string[x];
    }
}

void LCD::init () {
  delay2 (16); // wait for more than 15 msec
  pushNibble (0b00110000); // send (B0011) to DB7-4
  cmdWriteSet ();
  tickleEnable ();
  delay2 (5); // wait for more than 4.1 msec
  pushNibble (0b00110000); // send (B0011) to DB7-4
  cmdWriteSet ();
  tickleEnable ();
  delay2 (1); // wait for more than 100 usec
  pushNibble (0b00110000); // send (B0011) to DB7-4
  cmdWriteSet ();
  tickleEnable ();
  delay2 (1); // wait for more than 100 usec
  pushNibble (0b00100000); // send (B0010) to DB7-4 for 4bit
  cmdWriteSet ();
  tickleEnable ();
  delay2 (1); // wait for more than 100 usec
  // ready to use normal LcdCommandWrite() function now!
  LcdCommandWrite (0b00101000); // 4-bit interface, 2 display lines, 5x8 font
  LcdCommandWrite (0b00001100); // display control:
  LcdCommandWrite (0b00000110); // entry mode set: increment automatically, no display shift

  //creating the custom fonts:
  LcdCommandWrite (0b01001000); // set cgram
  static byte chars[] PROGMEM = {
      B11111, B00000, B11111, B11111, B00000, B11111, B00111, B11100,
      B11111, B00000, B11111, B11111, B00000, B11111, B01111, B11110,
      B00000, B00000, B00000, B11111, B00000, B11111, B11111, B11111,
      B00000, B00000, B00000, B11111, B00000, B11111, B11111, B11111,
      B00000, B00000, B00000, B11111, B00000, B11111, B11111, B11111,
      B00000, B00000, B00000, B11111, B01110, B11111, B11111, B11111,
      B00000, B11111, B11111, B01111, B01110, B11110, B11111, B11111,
      B00000, B11111, B11111, B00111, B01110, B11100, B11111, B11111};

  /* write the character data to the character generator ram */
  for (byte x = 0; x < 8; x++)
    {
      for (byte y = 0; y < 8; y++)
	{
	  LcdDataWrite (pgm_read_byte(&chars[y * 8 + x]));
	}
    }
  LcdCommandWrite (B00000001);  // clear display, set cursor position to zero
  LcdCommandWrite (B10000000);  // set dram to zero

}

void LCD::tickleEnable () {
  // send a pulse to enable  PD5
  PORTD |= (1 << 5);
  delayMicroseconds2 (1); // pause 1 ms according to datasheet
  PORTD &= ~(1 << 5);
  delayMicroseconds2 (1); // pause 1 ms according to datasheet
}

void LCD::cmdWriteSet () { //set enable (PD5) low and DI(PD4) low
  PORTD &= ~(1 << 5);
  delayMicroseconds2 (1); // pause 1 ms according to datasheet
  PORTD &= ~(1 << 4);
}

byte LCD::pushNibble (byte value) { //db7=PB5, db6=PB4, db5 = PB0, db4  = PD7
  value & 128 ? PORTB |= (1 << 5) : PORTB &= ~(1 << 5);
  value <<= 1;
  value & 128 ? PORTB |= (1 << 4) : PORTB &= ~(1 << 4);
  value <<= 1;
  value & 128 ? PORTB |= (1 << 0) : PORTB &= ~(1 << 0);
  value <<= 1;
  value & 128 ? PORTD |= (1 << 7) : PORTD &= ~(1 << 7);
  value <<= 1;
  return value;
}

void LCD::LcdCommandWrite (byte value) {
  value = pushNibble (value);
  cmdWriteSet ();
  tickleEnable ();
  value = pushNibble (value);
  cmdWriteSet ();
  tickleEnable ();
  delay2 (5);
}

void LCD::LcdDataWrite (byte value) {
  PORTD |= (1 << 4); //di on pd4
  value = pushNibble (value);
  tickleEnable ();
  value = pushNibble (value);
  tickleEnable ();
  delay2 (5);
}

// this function will return the number of bytes currently free in RAM
extern int __bss_end;
extern int *__brkval;
int memoryTest () {
  int free_memory;
  if ((int) __brkval == 0)
    free_memory = ((int) &free_memory) - ((int) &__bss_end);
  else
    free_memory = ((int) &free_memory) - ((int) __brkval);
  return free_memory;
}

Trip::Trip () {
}

//for display computing
unsigned long tmp1[2];
unsigned long tmp2[2];
unsigned long tmp3[2];

unsigned long instantmph () {
  //unsigned long vssPulseTimeuS = (lastVSS1 + lastVSS2) / 2;
  unsigned long vssPulseTimeuS = instant.vssPulseLength / instant.vssPulses;

  init64 (tmp1, 0, 1000000000ul);
  init64 (tmp2, 0, distancefactor);
  div64 (tmp1, tmp2);
  init64 (tmp2, 0, cyclesperhour);
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, vssPulseTimeuS);
  div64 (tmp1, tmp2);
  return tmp1[1] / 1000;

}

double voltage () {

  value = analogRead (analogInput);
  vin = (value * V);
  //   return vin+3;
}
unsigned long instantmpg () {
  unsigned long imph = instantmph ();
  unsigned long igph = instantgph ();
  if (igph == 0)
    return 0;
  if (imph == 0)
    return 0;//999999000;
  init64 (tmp1, 0, 1000ul);
  init64 (tmp2, 0, igph);
  mul64 (tmp2, tmp1);
  init64 (tmp1, 0, imph);
  div64 (tmp2, tmp1);
  //return tmp2[1];
  return tmp2[1] / 10;//////////pt opel*10
}

unsigned long instantgph () {
  init64 (tmp1, 0, instInjTot);
  init64 (tmp2, 0, 3600000000ul);
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, 1000ul);
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, fuelfactor);
  div64 (tmp1, tmp2);
  init64 (tmp2, 0, instInjEnd - instInjStart);
  div64 (tmp1, tmp2);
  return tmp1[1] * 10;
}

unsigned long instantfuelperhour () {

  init64 (tmp1, 0, instInjTot);
  init64 (tmp2, 0, 12000000ul);
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, 1000000ul);
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, fuelfactor);
  div64 (tmp1, tmp2);
  init64 (tmp2, 0, instInjEnd - instInjStart);
  div64 (tmp1, tmp2);
  return tmp1[1] * 10;
}

unsigned long instantrpm () {
  init64 (tmp1, 0, instInjCount);
  init64 (tmp2, 0, 120000000ul);
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, 1000ul);
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, parms[injPulsesPer2Revolutions]);
  div64 (tmp1, tmp2);
  init64 (tmp2, 0, instInjEnd - instInjStart);
  div64 (tmp1, tmp2);
  return tmp1[1] / 1000;
}
unsigned long recalculate (unsigned long calibrationvalue, unsigned long currentvalue, unsigned long correctvalue) {
  simpletx ("recalc ");
  simpletx (uformat (currentvalue));     // 0000022211
  simpletx (",");
  simpletx (uformat (correctvalue));     // 0000022299
  simpletx (",");
  simpletx (uformat (calibrationvalue)); // 0000009926
  simpletx (";");

  init64 (tmp1, 0, currentvalue);
  init64 (tmp2, 0, 1000000ul);              // multiply to obtain six decimal points of accuracy
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, correctvalue);
  div64 (tmp1, tmp2);                   // current / correct, resulting fraction in tmp1
  // (22211 * 1000000)/22299=996.035 = 996 (the decimal digits are lost)
  
  simpletx (uformat (tmp1[0]));
  simpletx (",");
  simpletx (uformat (tmp1[1]));
  simpletx (";");
  
  init64 (tmp2, 0, calibrationvalue);
  mul64 (tmp1, tmp2);                   // (correction-fraction * 1000000) * original calibration value, result (new calibration value * 1000000) in tmp1
  
  simpletx (uformat (tmp1[0]));
  simpletx (",");
  simpletx (uformat (tmp1[1]));
  simpletx (";");

  init64 (tmp2, 0, 1000000ul);
  div64 (tmp1, tmp2);                   // back to correct magnitude
  
  simpletx (uformat (tmp1[0]));
  simpletx (", return=");
  simpletx (uformat (tmp1[1]));
  simpletx ("\n");

  return tmp1[1];
}
unsigned long Trip::distance () {
  init64 (tmp1, 0, vssPulses);
  init64 (tmp2, 0, 1000);
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, distancefactor);
  div64 (tmp1, tmp2);
  return tmp1[1] * 100;
}

unsigned long Trip::eocMiles () {
  init64 (tmp1, 0, vssEOCPulses);
  init64 (tmp2, 0, 1000);
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, distancefactor);
  div64 (tmp1, tmp2);
  return tmp1[1];
}

unsigned long Trip::mph () {
  if (loopCount == 0)
    return 0;
  init64 (tmp1, 0, loopsPerSecond);
  init64 (tmp2, 0, vssPulses);
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, 3600000ul);
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, distancefactor);
  div64 (tmp1, tmp2);
  init64 (tmp2, 0, loopCount);
  div64 (tmp1, tmp2);
  return tmp1[1] / 1000;
}

unsigned long Trip::fuel () {
  init64 (tmp1, 0, injHiSec);
  init64 (tmp2, 0, 1000000);
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, injHius);
  add64 (tmp1, tmp2);
  init64 (tmp2, 0, 1000);
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, fuelfactor);
  div64 (tmp1, tmp2);
  return (tmp1[1] * 1.609344) * 10;
}
unsigned long Trip::Range () {
  sensorValue = analogRead (sensorPin);// apply the calibration to the sensor reading
  sensorValue = map (sensorValue, sensorMax, sensorMin, 0, L);
  sensorValue = constrain (sensorValue, 0, L);
  unsigned long restfuel = ((sensorValue) * 10000000 / tank.fuelperdistanceunit (100));
  return restfuel;
}
unsigned long Trip::idleGallons () {
  init64 (tmp1, 0, injIdleHiSec);
  init64 (tmp2, 0, 1000000);
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, injIdleHius);
  add64 (tmp1, tmp2);
  init64 (tmp2, 0, dispadj);
  mul64 (tmp1, tmp2);
  init64 (tmp2, 0, fuelfactor);
  div64 (tmp1, tmp2);
  return tmp1[1];
}


//eocMiles
//idleGallons


unsigned long  Trip::fuelperdistanceunit (unsigned long multiplier) {
  init64 (tmp1, 0, injHiSec);
  init64 (tmp3, 0, 1000000);
  mul64 (tmp3, tmp1);
  init64 (tmp1, 0, injHius);
  add64 (tmp3, tmp1);
  init64 (tmp1, 0, distancefactor);
  mul64 (tmp3, tmp1);
  init64 (tmp2, 0, 1000);
  mul64 (tmp3, tmp2);
  init64 (tmp1, 0, fuelfactor);
  init64 (tmp2, 0, vssPulses);
  mul64 (tmp1, tmp2);
  div64 (tmp3, tmp1);
  init64 (tmp2, 0, multiplier);    // scale it up (eg: "1" for litres/km or "100" litres/100km
  mul64 (tmp3, tmp2);
  if (tmp3[1] > 150000) tmp3[1] = 150000;
  return (tmp3[1] * 1.609344) * 10;
}

//return the seconds as a time mmm.ss, eventually hhh:mm too
unsigned long Trip::timeS () {//return seconds;
  byte d = 60;
  unsigned long seconds = loopCount / loopsPerSecond;
  sec = (seconds % d);
  return sec;
}
unsigned long Trip::timeM () {//return MINUTES;
  byte d = 60;
  unsigned long minutes = (loopCount / loopsPerSecond) / 60;
  m = (minutes % d);
  return m;
}
unsigned long Trip::timeH () {//return hour;
  byte d = 999;
  byte h = 0;
  unsigned long hours = (loopCount / loopsPerSecond) / 3600;
  h = (hours % d);
  return h;
}

void Trip::reset () {
  loopCount = 0;
  injPulses = 0;
  injHius = 0;
  injHiSec = 0;
  vssPulses = 0;
  vssPulseLength = 0;
  injIdleHiSec = 0;
  injIdleHius = 0;
  vssEOCPulses = 0;
}

void Trip::update (Trip t) {
  loopCount++; //we call update once per loop
  vssPulses += t.vssPulses;
  vssPulseLength += t.vssPulseLength;
  if (t.injPulses == 0) //track distance traveled with engine off
    vssEOCPulses += t.vssPulses;

  if (t.injPulses > 2 && t.injHius < 500000)
    {//chasing ghosts
      injPulses += t.injPulses;
      injHius += t.injHius;
      if (injHius >= 1000000)
	{ //rollover into the injHiSec counter
	  injHiSec++;
	  injHius -= 1000000;
	}
      if (t.vssPulses == 0)
	{ //track gallons spent sitting still

	  injIdleHius += t.injHius;
	  if (injIdleHius >= 1000000)
	    { //r
	      injIdleHiSec++;
	      injIdleHius -= 1000000;
	    }
	}
    }
}

//the standard 64 bit math brings in  5000+ bytes
//these bring in 1214 bytes, and everything is pass by reference
unsigned long zero64[] = {0, 0};

void init64 (unsigned long an[], unsigned long bigPart, unsigned long littlePart) {
  an[0] = bigPart;
  an[1] = littlePart;
}

//left shift 64 bit "number"
void shl64 (unsigned long an[]) {
  an[0] <<= 1;
  if (an[1] & 0x80000000)
    an[0]++;
  an[1] <<= 1;
}

//right shift 64 bit "number"
void shr64 (unsigned long an[]) {
  an[1] >>= 1;
  if (an[0] & 0x1)
    an[1] += 0x80000000;
  an[0] >>= 1;
}

//add ann to an
void add64 (unsigned long an[], unsigned long ann[]) {
  an[0] += ann[0];
  if (an[1] + ann[1] < ann[1])
    an[0]++;
  an[1] += ann[1];
}

//subtract ann from an
void sub64 (unsigned long an[], unsigned long ann[]) {
  an[0] -= ann[0];
  if (an[1] < ann[1])
    {
      an[0]--;
    }
  an[1] -= ann[1];
}

//true if an == ann
boolean eq64 (unsigned long an[], unsigned long ann[]) {
  return (an[0] == ann[0]) && (an[1] == ann[1]);
}

//true if an < ann
boolean lt64 (unsigned long an[], unsigned long ann[]) {
  if (an[0] > ann[0])
    return false;
  return (an[0] < ann[0]) || (an[1] < ann[1]);
}

//divide num by den
void div64 (unsigned long num[], unsigned long den[]) {
  unsigned long quot[2];
  unsigned long qbit[2];
  unsigned long tmp[2];
  init64 (quot, 0, 0);
  init64 (qbit, 0, 1);

  if (eq64 (num, zero64))
    { //numerator 0, call it 0
      init64 (num, 0, 0);
      return;
    }

  if (eq64 (den, zero64))
    { //numerator not zero, denominator 0, infinity in my book.
      init64 (num, 0xffffffff, 0xffffffff);
      return;
    }

  init64 (tmp, 0x80000000, 0);
  while (lt64 (den, tmp))
    {
      shl64 (den);
      shl64 (qbit);
    }

  while (!eq64 (qbit, zero64))
    {
      if (lt64 (den, num) || eq64 (den, num))
	{
	  sub64 (num, den);
	  add64 (quot, qbit);
	}
      shr64 (den);
      shr64 (qbit);
    }

  //remainder now in num, but using it to return quotient for now
  init64 (num, quot[0], quot[1]);
}

//multiply num by den
void mul64 (unsigned long an[], unsigned long ann[]) {
  unsigned long p[2] = {0, 0};
  unsigned long y[2] = {ann[0], ann[1]};
  while (!eq64 (y, zero64))
    {
      if (y[1] & 1)
	add64 (p, an);
      shl64 (an);
      shr64 (y);
    }
  init64 (an, p[0], p[1]);
}

void save () {
  eeprom_write_byte ((unsigned char *) 0, guinosig);
  eeprom_write_byte ((unsigned char *) 1, parmsLength);
  byte p = 0;
  for (int x = 4; p < parmsLength; x += 4)
    {
      unsigned long v = parms[p];
      eeprom_write_byte ((unsigned char *) x, (v >> 24) & 255);
      eeprom_write_byte ((unsigned char *) x + 1, (v >> 16) & 255);
      eeprom_write_byte ((unsigned char *) x + 2, (v >> 8) & 255);
      eeprom_write_byte ((unsigned char *) x + 3, (v) & 255);
      p++;
    }
}

byte load () { //return 1 if loaded ok
#ifdef usedefaults
  return 1;
#endif
  byte b = eeprom_read_byte ((unsigned char *) 0);
  byte c = eeprom_read_byte ((unsigned char *) 1);
  if (b == guinosigold)
    c = 9; //before fancy parameter counter

  if (b == guinosig || b == guinosigold)
    {
      byte p = 0;

      for (int x = 4; p < c; x += 4)
	{
	  unsigned long v = eeprom_read_byte ((unsigned char *) x);
	  v = (v << 8) + eeprom_read_byte ((unsigned char *) x + 1);
	  v = (v << 8) + eeprom_read_byte ((unsigned char *) x + 2);
	  v = (v << 8) + eeprom_read_byte ((unsigned char *) x + 3);
	  parms[p] = v;
	  p++;
	}
      return 1;
    }
  return 0;
}

char *uformat (unsigned long val) {
  unsigned long d = 1000000000ul;
  for (byte p = 0; p < 10; p++)
    {
      mBuff[p] = '0' + (val / d);
      val = val - (val / d * d);
      d /= 10;
    }
  mBuff[10] = 0;
  return mBuff;
}

unsigned long rformat (char *val) {
  unsigned long d = 1000000000ul;
  unsigned long v = 0ul;
  for (byte p = 0; p < 10; p++)
    {
      v = v + (d * (val[p] - '0'));
      d /= 10;
    }
  return v;
}

void editParm (byte parmIdx) {
  unsigned long v = parms[parmIdx];
  byte p = 9; //right end of 10 digit number
  //display label on top line
  //set cursor visible
  //set pos = 0
  //display v

  LCD::gotoXY (8, 0);
  LCD::print ("        ");
  LCD::gotoXY (0, 0);
  LCD::print (parmLabels[parmIdx]);
  LCD::gotoXY (0, 1);
  char *fmtv = uformat (v);
  LCD::print (fmtv);
  LCD::print (" OK XX");
  LCD::LcdCommandWrite (0b00001110);

#ifdef cursor_first_nonzero_digit
  for (int x = 9; x >= 0; x--) { //do a nice thing and put the cursor at the first non zero number
	  if (fmtv[x] != '0')
		  p=x;
  }
#else
  // cursor on the OK
  p = 10;
#endif
  byte keyLock = 1;
  while (true)
    {

      if (p < 10)
	LCD::gotoXY (p, 1);
      if (p == 10)
	LCD::gotoXY (11, 1);
      if (p == 11)
	LCD::gotoXY (14, 1);

      if (keyLock == 0)
	{
	  if (!(buttonState & lbuttonBit) && !(buttonState & rbuttonBit))
	    {// left & right
	      LCD::LcdCommandWrite (0b00001100);
	      return;
	    }
	  else if (!(buttonState & lbuttonBit))
	    {// left
	      p = p - 1;
	      if (p == 255)
		p = 11;
	    }
	  else if (!(buttonState & rbuttonBit))
	    {// right
	      p = p + 1;
	      if (p == 12)
		p = 0;
	    }
	  else if (!(buttonState & mbuttonBit))
	    {// middle
	      if (p == 11)
		{ //cancel selected
		  LCD::LcdCommandWrite (0b00001100);
		  return;
		}
	      if (p == 10)
		{ //ok selected
		  LCD::LcdCommandWrite (0b00001100);
		  parms[parmIdx] = rformat (fmtv);
		  return;
		}

	      byte n = fmtv[p] - '0';
	      n++;
	      if (n > 9)
		n = 0;
	      if (p == 0 && n > 3)
		n = 0;
	      fmtv[p] = '0' + n;
	      LCD::gotoXY (0, 1);
	      LCD::print (fmtv);
	      LCD::gotoXY (p, 1);
	      if (parmIdx == contrastIdx)//adjust contrast dynamically
		OCR0A = rformat (fmtv);

	      //                 analogWrite(ContrastPin,rformat(fmtv));


	    }

	  if (buttonState != buttonsUp)
	    keyLock = 1;
	}
      else
	{
	  keyLock = 0;
	}
      buttonState = buttonsUp;
      delay2 (125);
    }

}

void initGuino () { //edit all the parameters
  for (int x = 0; x < parmsLength; x++)
    editParm (x);
  save ();
  injectorSettleTime = injhold;

  int0Func = processInjOpen;
  int1Func = processInjClosed;
  EIMSK &= ~(1 << INT0);
  EIMSK &= ~(1 << INT1);

  EICRA = (EICRA & ~((1 << ISC00) | (1 << ISC01)))
	  | ((parms[injEdgeIdx] == 1 ? RISING : FALLING) << ISC00);
  EIMSK |= (1 << INT0);
  EICRA = (EICRA & ~((1 << ISC10) | (1 << ISC11)))
	  | ((parms[injEdgeIdx] == 1 ? FALLING : RISING) << ISC10);
  EIMSK |= (1 << INT1);

  distancefactor = parms[vssPulsesPerMileIdx];
  fuelfactor = parms[microSecondsPerGallonIdx];
  fuelfactor = (fuelfactor * 3.78541178);
  tmpDRL = parms[drlIdx];

  holdDisplay = 1;
}

unsigned long millis2 () {
  return timer2_overflow_count * 64UL * 2 / (16000000UL / 128000UL);
}

void delay2 (unsigned long ms) {
  unsigned long start = millis2 ();
  while (millis2 () - start < ms);
}

/* Delay for the given number of microseconds.  Assumes a 16 MHz clock.
 * Disables interrupts, which will disrupt the millis2() function if used
 * too frequently. */
void delayMicroseconds2 (unsigned int us) {
  uint8_t oldSREG;
  if (--us == 0)
    return;
  us <<= 2;
  us -= 2;
  oldSREG = SREG;
  cli();
  // busy wait
  __asm__ __volatile__ (
  "1: sbiw %0,1" "\n\t" // 2 cycles
      "brne 1b" : "=w" (us) : "0" (us) // 2 cycles
  );
  // reenable interrupts.
  SREG = oldSREG;
}

void simpletx (char *string) {
  if (UCSR0B != (1 << TXEN0))
    { //do we need to init the uart?
      UBRR0H = (unsigned char) (myubbr >> 8);
      UBRR0L = (unsigned char) myubbr;
      UCSR0B = (1 << TXEN0);//Enable transmitter
      UCSR0C = (3 << UCSZ00);//N81
    }
  while (*string)
    {
      while (!(UCSR0A & (1 << UDRE0)));
      UDR0 = *string++; //send the data
    }
}

int main (void) {
  sei();
  sbi(TCCR0A, WGM01);
  sbi(TCCR0A, WGM00);
  sbi(TCCR0B, CS01);
  sbi(TCCR0B, CS00);
  sbi(TIMSK0, TOIE0);

  // set timer 1 prescale factor to 64
  sbi(TCCR1B, CS11);
  sbi(TCCR1B, CS10);
  // put timer 1 in 8-bit phase correct pwm mode
  sbi(TCCR1A, WGM10);
  // set timer 2 prescale factor to 64
  sbi(TCCR2B, CS22);
  // configure timer 2 for phase correct pwm (8-bit)
  sbi(TCCR2A, WGM20);

  // set a2d prescale factor to 128
  sbi(ADCSRA, ADPS2);
  sbi(ADCSRA, ADPS1);
  sbi(ADCSRA, ADPS0);

  // enable a2d conversions
  sbi(ADCSRA, ADEN);

  UCSR0B = 0;

  sei();

  timer2_overflow_count = 0;

  TCCR2A = 1 << WGM20 | 1 << WGM21;
  // set timer 2 prescale factor to 64
  TCCR2B = 1 << CS22;
  TIMSK2 |= 1 << TOIE2;
  TIMSK0 &= !(1 << TOIE0);

  setup ();
  mainloop ();

  return 0;
}

