//
// Created by Angel Zaprianov on 27.6.2016 г..
//
#include <Arduino.h>

#ifndef MID_MID_H
#define MID_MID_H

#include <LiquidCrystal.h>



double getAirFlowRation(int engTemp){
    // CFM = (Input BTU x thermal efficiency) / (1.08 x DT)

    return 1.084;
}
//
// http://skodaclub.bg/forum/index.php?/topic/45846-%D1%80%D0%B0%D0%B7%D1%85%D0%BE%D0%B4-%D0%BD%D0%B0-%D0%B3%D0%BE%D1%80%D0%B8%D0%B2%D0%BE-%D0%BD%D0%B0-%D0%BF%D1%80%D0%B0%D0%B7%D0%B5%D0%BD-%D1%85%D0%BE%D0%B4-%D0%BB%D0%B8%D1%82%D1%80%D0%B8-%D0%BD%D0%B0-%D1%87%D0%B0%D1%81/
//
int calConsumption(int engineRpm) {
    int oneEngineTurnLiter;
    int oneEngineTurnBurns;
    int consumptionForHour;
    int consumptionCc;
    int consumptionCubicKg;
    int engineTemperature = 50;



    oneEngineTurnBurns = (CON_ENG_CC / 2) / 1000 /* for coverts to liters */;
    consumptionForHour = (engineRpm * oneEngineTurnBurns) /* result for 1 min */  * 60 /* to one hour */;

    if (THROTTLE_UP)
        consumptionCc = consumptionForHour / FLW_MTR_FR;  /* in case throttle is not positive  */
    else
        consumptionCc = consumptionForHour;


    consumptionCubicKg = ((consumptionCc / 1000) * getAirFlowRation(engineTemperature)) / AIR_FUL_RT; // Convert to kg per hour

    return consumptionCubicKg/81;
}




//
// Engine read values
unsigned int thcSnsCount = 0;
unsigned int spdSnsCount = 0;




//
// Define public method
static void setupMid();

static void initMenu();

static void runMenu();

bool isSensorReadAllow();

int getSensorAmplitudeRead(int PinTarget, int TotalContainerState);

//
// Sensors timing containers
long int SNS_LAST_RUN_TIME = 0; // Global sensor method timing
long int SNS_LAST_ECU_READ = 0; // ECU last time read

//
// Sensor timing handler
bool isSensorReadAllow() {
    if (millis() >= SNS_LAST_RUN_TIME + SNS_INTERVAL_TIME) {
        SNS_LAST_RUN_TIME = millis();
        return true;
    }
    return false;
}

int long SNS_MIN_RUN_TIME = 0;
//
// Sensor timing handler
bool isSensorReadMinute() {
    if (millis() >= SNS_LAST_RUN_TIME + MILLIS_PER_MN) {
        SNS_MIN_RUN_TIME = millis();
        return true;
    }
    return false;
}

//
// Setup the mid
void setupMain() {



    //
    // Pin button mode
    pinMode(BTN_PIN_UP, INPUT);
    pinMode(BTN_PIN_DW, INPUT);


}



//
// Read ECU signal amplitude
int getSensorAmplitudeRead(int PinTarget, int TotalContainerState) {
    int ReadingState = LOW;
    //
    // Get reading from pin
    ReadingState = digitalRead(PinTarget);
    //
    //  Creating time loop amplitude
    if (isSensorReadMinute()) {
        //
        // Reset time bounce to zero
        TotalContainerState = 0;
    }
    //
    // If is high count it
    if (ReadingState == HIGH) {
        TotalContainerState++;
    }
    //
    //
    return TotalContainerState;
}

/**
 *
 */
float calcFuelEfficiency_(int distanceKm, int liters) {
    float efficien; //efficiency variable
    efficien = distanceKm / liters;
    return (efficien);//returning value
}


char fBuff[7];//used by format

char *formatNumber (unsigned long num)
{
    byte dp = 3;

    while (num > 999999)
        {
        num /= 10;
        dp++;
        if (dp == 5) break; // We'll lose the top numbers like an odometer
        }
    if (dp == 5) dp = 99; // We don't need a decimal point here.

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
    fBuff[6] = 0;
    return fBuff;
}



/*
 * Convert seconds to Inches
 */
long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}
/*
 * Convert seconds to Centimeters
 */
long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}

/**
int calcFuelEfficiency (void)
{

  //sets values as either integars or real numbers
  double radius_of_tire, gasoline_consumed, fuel_economy;
  int num_of_rev, inches_in_mile;
  //equation converting constants to inches in a mile, needed for distance
  inches_in_mile = INCHES_IN_FOOT * FEET_IN_MILE;

  ////steps which prints/asks user for info needed to make calculation
  printf ("What is the radius of your tires, in inches?\n");
  scanf ("%lf", &radius_of_tire); //double

  printf ("How many revolutions did your car's tires make?\n");
  scanf ("%d", &num_of_rev); //int

  printf ("How many gallons of gas did your car use?\n");
  scanf ("%lf", &gasoline_consumed); //double

  //prints final value to the screen to two decimal places
  printf ("Your car averaged %0.2lf miles per gallon.\n",
//      /overall equation used for calculating fuel economy/includes
//      constants. This equation translates to distance divided by gallons
//      of gas used to drive that distance
          num_of_rev * (PI_OF_CIRCF * TWO_OF_CIRCF * radius_of_tire) / inches_in_mile
          / gasoline_consumed, fuel_economy);

  if (0 <= fuel_economy <= 20)
    {
      printf ("Your car gets poor gas mileage.\n");
    }
  else if (20 < fuel_economy <= 30)
    {
      printf ("Your car gets average gas mileage.\n");
    }
  else if (30 < fuel_economy <= 40)
    {
      printf ("Your car gets good gas mileage.\n");
    }
  else if (fuel_economy > 40)
    {
      printf ("Your car gets excellent gas mileage.\n");
    }

  return 0;
}
 */

#endif //ARDUINOMID_UTILS_H




