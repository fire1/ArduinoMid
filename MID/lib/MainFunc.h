//
// Created by Angel Zaprianov on 27.6.2016 г..
//
#include <Arduino.h>

#ifndef MID_MID_H
#define MID_MID_H

#include <LiquidCrystal.h>

//
// http://skodaclub.bg/forum/index.php?/topic/45846-%D1%80%D0%B0%D0%B7%D1%85%D0%BE%D0%B4-%D0%BD%D0%B0-%D0%B3%D0%BE%D1%80%D0%B8%D0%B2%D0%BE-%D0%BD%D0%B0-%D0%BF%D1%80%D0%B0%D0%B7%D0%B5%D0%BD-%D1%85%D0%BE%D0%B4-%D0%BB%D0%B8%D1%82%D1%80%D0%B8-%D0%BD%D0%B0-%D1%87%D0%B0%D1%81/
//
int calConsumption(int engineRpm) {
    int oneEngineTurnLiter;
    int oneEngineTurnBurns;
    int consumptionForHour;
    int consumptionCc;
    int consumptionCubicKg;

    oneEngineTurnBurns = (CON_ENG_CC / 2) / 1000 /* for coverts to liters */;
    consumptionForHour = (engineRpm * oneEngineTurnBurns) /* result for 1 min */  * 60 /* to one hour */;

    if (THROTTLE_UP)
        consumptionCc = consumptionForHour / FLW_MTR_FR;  /* in case throttle is not positive  */
    else
        consumptionCc = consumptionForHour;

    consumptionCubicKg = ((consumptionCc / 1000) * 1.084) / BRN_MAS_FW; // Convert to kg per hour

    return consumptionCubicKg;
}

//
// Engine read values
unsigned int thcSnsCount = 0;
unsigned int spdSnsCount = 0;
unsigned int ecuSnsCount = 0;



//
// Define public method
static void setupMid();

static void initMenu();

static void runMenu();

bool isSensorReadAllow(long int interval = 0);

int getSensorAmplitudeRead(int PinTarget, int TotalContainerState);

//
// Sensors timing containers
long int SNS_LAST_RUN_TIME = 0; // Global sensor method timing
long int SNS_LAST_ECU_READ = 0; // ECU last time read

//
// Sensor timing handler
bool isSensorReadAllow(long int interval) {
    int intervalValue;

    if (interval == 0) {
        intervalValue = SNS_INTERVAL_TIME;
    }
    else {
        intervalValue = interval;
    }

    if (millis() >= SNS_LAST_RUN_TIME + intervalValue) {
        SNS_LAST_RUN_TIME = millis();
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
    //
    // Engine pin mode as input
    pinMode(TCH_SNS_PIN, INPUT);
    pinMode(SPD_SNS_PIN, INPUT);
    pinMode(ECU_SGN_PIN, INPUT);

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
    if (isSensorReadAllow()) {
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




