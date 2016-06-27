//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#include <Arduino.h>
#include "MID.h"
#include "chr.h"
#include <LiquidCrystal.h>

//
// Time of last run time
long int SNS_LAST_RUN_TIME = 0;

//
// Sensor timing handler
bool MID::isSensorReadAllow ()
{

  if (millis () >= SNS_LAST_RUN_TIME + SNS_INTERVAL_TIME)
    {
      SNS_LAST_RUN_TIME = millis ();
      return true;
    }
  return false;
}
//
// Setup the mid
void MID::setup ()
{
  MID::setupCustomCharacters ();

}

//
// LCD Character handler
void MID::setupCustomCharacters ()
{
  lcd.createChar (0, CHR_SML);
  lcd.createChar (1, CHR_CLS);
  lcd.createChar (2, CHR_MNC1);
  lcd.createChar (3, CHR_MNC2);
}

