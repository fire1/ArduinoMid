//
// Created by Angel Zaprianov on 3.4.2017 г..
//

#ifndef ARDUINOMID_INITIALIZATIONS_H
#define ARDUINOMID_INITIALIZATIONS_H
//
// I2C detector driver
//#include "LpgSens.h"
//
// Interval / Amplitude
#include "IntAmp.h"
//
// Adding Alphine emulator
#include "WhlSens.h"
//
// Main Sensor handler
#include "MainFunc.h"
//
// Engine sensors
#include "CarSens.h"
//
// Car state
#include "CarState.h"
//
// Menu button handler
#include "MenuBtn.h"
//
//
#include "EepRom.h"
//
// Add library
#include "ShutDw.h"
//
//
#include "MenuBase.h"

//
// Amplitude interval
//    ampInt(min,low,mid,sec, big, max);
IntAmp ampInt;
//
// Constructing the class
CarSens carSens(ampInt);
//
// Constructing car state
CarState carStat(ampInt);
//
// Constructing the class
WhlSens whlSens(ampInt);
//
// Data storage
EepRom eepRom(carSens);
//
// Buttons driver
MenuBtn btnMenu(ampInt, carSens, eepRom, whlSens, carStat);
//
// Additional libraries
// ----
// Shutdown constructor
ShutDw shutDown(eepRom, ampInt, carSens, whlSens);


#endif //ARDUINOMID_INITIALIZATIONS_H
