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
int getSensorAmplitudeRead (int PinTarget, int TotalContainerState) {
    int ReadingState = LOW;
    //
    // Get reading from pin
    ReadingState = digitalRead (PinTarget);
    //
    //  Creating time loop amplitude
    if (isSensorReadMinute ())
        {
        //
        // Reset time bounce to zero
        TotalContainerState = 0;
        }
    //
    // If is high count it
    if (ReadingState == HIGH)
        {
        TotalContainerState++;
        }
    //
    //
    return TotalContainerState;
}

/**

#endif //ARDUINOMID_UTILS_H




