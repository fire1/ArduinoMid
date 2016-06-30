//
// Created by Angel Zaprianov on 27.6.2016 г..
//
#include <Arduino.h>
#ifndef MID_MID_H
#define MID_MID_H
#include <LiquidCrystal.h>

//
// Time information
#define MILLIS_PER_HR    3600000L // Hour
#define MILLIS_PER_MN    60000L   // Minute
#define MILLIS_PER_SC    1000L    // Second

//
// Engine read values
unsigned int thcSnsCount = 0;
unsigned int spdSnsCount = 0;
unsigned int ecuSnsCount = 0;

//
// Global interval
static const int SNS_INTERVAL_TIME = 2000;
//
// MID plug pins definition over Arduino
//
// Define button pins for steering controller
const int BTN_PIN_UP = 8;
const int BTN_PIN_DW = 9;
//
// Engine pins
const int TCH_SNS_PIN = 22; // the crankshaft speed sensor
const int SPD_SNS_PIN = 22; // Speed sensor hub
const int ECU_SGN_PIN = 24; // ECU signal

//
// Define public method
static void setupMid ();
static void initMenu ();
static void runMenu ();

bool isSensorReadAllow (long int interval = 0);

//
// Return reads from ECU
int getEcuSignalAmplitude ();

int getSensorAmplitudeRead (int PinTarget, int TotalContainerState);

//
// Sensors timing containers
long int SNS_LAST_RUN_TIME = 0; // Global sensor method timing
long int SNS_LAST_ECU_READ = 0; // ECU last time read

//
// Sensor timing handler
bool isSensorReadAllow (long int interval)
{
  int intervalValue;

  if (interval == 0)
    {
      intervalValue = SNS_INTERVAL_TIME;
    }
  else
    {
      intervalValue = interval;
    }

  if (millis () >= SNS_LAST_RUN_TIME + intervalValue)
    {
      SNS_LAST_RUN_TIME = millis ();
      return true;
    }
  return false;
}

//
// Setup the mid
void setupMain ()
{

  //
  // Pin button mode
  pinMode (BTN_PIN_UP, INPUT);
  pinMode (BTN_PIN_DW, INPUT);
  //
  // Engine pin mode as input
  pinMode (TCH_SNS_PIN, INPUT);
  pinMode (SPD_SNS_PIN, INPUT);
  pinMode (ECU_SGN_PIN, INPUT);


}

int getEcuSignalAmplitude ()
{
  return getSensorAmplitudeRead (ECU_SGN_PIN, ecuSnsCount);
}

//
// Read ECU signal amplitude
int getSensorAmplitudeRead (int PinTarget, int TotalContainerState)
{
  int ReadingState = LOW;
  //
  // Get reading from pin
  ReadingState = digitalRead (PinTarget);
  //
  //  Creating time loop amplitude
  if (isSensorReadAllow ())
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

#endif //ARDUINOMID_UTILS_H




