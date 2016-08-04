#define enableVSSID 0
#define vsspValue 9;
#define vssBit ( 1 << 0 )

unsigned volatile long lastVSS1;
unsigned volatile long lastVSSTime;
unsigned volatile long lastVSS2;

volatile boolean lastVssFlop = vssFlop;

unsigned long elapsedMicroseconds (unsigned long startMicroSeconds, unsigned long currentMicroseconds)
{
  if (currentMicroseconds >= startMicroSeconds)
    return currentMicroseconds - startMicroSeconds;
  return 4294967295 - (startMicroSeconds - currentMicroseconds);
}

unsigned long elapsedMicroseconds (unsigned long startMicroSeconds)
{
  return elapsedMicroseconds (startMicroSeconds, microSeconds ());
}

//overflow counter used by millis2()
unsigned long lastMicroSeconds = millis2 () * 1000;
unsigned long microSeconds (void)
{
  unsigned long tmp_timer2_overflow_count;
  unsigned long tmp;
  byte tmp_tcnt2;
  cli (); //disable interrupts
  tmp_timer2_overflow_count = timer2_overflow_count;
  tmp_tcnt2 = TCNT2;
  sei (); // enable interrupts
  tmp = ((tmp_timer2_overflow_count << 8) + tmp_tcnt2) * 4;
  if ((tmp <= lastMicroSeconds) && (lastMicroSeconds < 4290560000ul))
    return microSeconds ();
  lastMicroSeconds = tmp;
  return tmp;
}

unsigned volatile long lastVSS1;
unsigned volatile long lastVSSTime;
unsigned volatile long lastVSS2;

volatile boolean lastVssFlop = vssFlop;

//attach the vss/buttons interrupt
void ISR (PCINT1_vect)
{
  static byte vsspinstate = 0;
  byte p = PINC;//bypassing digitalRead for interrupt performance
  if ((p & vssBit) != (vsspinstate & vssBit))
    {
      addEvent (enableVSSID, parms[vsspause]); //check back in a couple milli
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