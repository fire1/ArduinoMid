//
// Created by Angel Zaprianov on 30.6.2016 г..
//

#ifndef MID_TRIP_UTILS_H
#define MID_TRIP_UTILS_H

// Time information
#define MILLIS_PER_HOUR    3600000L
#define MILLIS_PER_MINUTE    60000L
#define MILLIS_PER_SECOND     1000L

void ISR( PCINT1_vect )
    {
        static byte vsspinstate = 0;
        byte p = PINC;//bypassing digitalRead for interrupt performance
        if ((p & vssBit) != (vsspinstate & vssBit)) {
          addEvent(enableVSSID, parms[vsspauseIdx]); //check back in a couple milli
        }
        if (lastVssFlop != vssFlop) {
          lastVSS1 = lastVSS2;
          unsigned long t = microSeconds();
          lastVSS2 = elapsedMicroseconds(lastVSSTime, t);
          lastVSSTime = t;
          tmpTrip.vssPulses++;
          tmpTrip.vssPulseLength += lastVSS2;
          lastVssFlop = vssFlop;
        }
        vsspinstate = p;
        buttonState &= p;
    }


// Calculate the time difference, and account for roll over too
unsigned long calcTimeDiff(unsigned long start, unsigned long end)
{
/*
if (start < end)
  {
    return end - start;
  }
  else // roll over
  {
    return ULONG_MAX - start + end;
  }
*/
  // inspired by http://www.arduino.cc/playground/Code/TimingRollover saves 200bytes
  return (long)(end - start);
}


/**
 *
 */
void getEngineInTime(char *retbuf)
{
  unsigned long run_time;
  int hours, minutes, seconds;  //to store the time

#ifdef useECUState
  if (ECUconnection) {//update with current time, if the car is running
#else
  if(has_rpm) {//update with current time, if the car is running
#endif
      run_time = calcTimeDiff(engine_on, millis());    //We now have the number of ms
    } else { //car is not running.  Display final time when stopped.
      run_time = calcTimeDiff(engine_on, engine_off);
    }
  //Lets lcdDisplay the running time
  //hh:mm:ss
  hours =   run_time / MILLIS_PER_HOUR;
  minutes = (run_time % MILLIS_PER_HOUR) / MILLIS_PER_MINUTE;
  seconds = (run_time % MILLIS_PER_MINUTE) / MILLIS_PER_SECOND;

  //Now we have our varriables parsed, lets lcdDisplay them
  sprintf_P(retbuf, PSTR("%d:%02d:%02d"), hours, minutes, seconds);
}


#endif //ARDUINOMID_TRIP_UTILS_H
