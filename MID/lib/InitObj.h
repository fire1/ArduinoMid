//
// Created by Angel Zaprianov on 3.4.2017 г..
//

#ifndef ARDUINO_MID_INITIALIZATIONS_H
#define ARDUINO_MID_INITIALIZATIONS_H
//
// Interval / Amplitude
#include "AmpTime.h"
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
//
#include "CarCollect.h"
//
//
#ifdef LPG_INSTALLATION

#include "LpgSerial.h"

#endif
//
// Amplitude interval
//    ampInt(min,low,mid,sec, big, max);
AmpTime ampInt;
//
// Constructing the class
CarSens carSens(ampInt);
//
// Constructing car state
CarState carStat(ampInt, carSens);
//
// Constructing the class
WhlSens whlSens(ampInt);
//
// Data storage
EepRom eepRom(carSens, whlSens);
//
// Buttons driver
MenuBtn btnMenu(ampInt, carSens, eepRom, whlSens, carStat);
//
// Additional libraries
// ----
// Shutdown constructor
ShutDw shutDown(eepRom, ampInt, carSens, whlSens);

//CarCollect carCollect(carSens, ampInt);

#ifdef ADT_FUEL_SYSTEM_SERIAL
LpgSerial lpgCom;
#endif

#endif //ARDUINOMID_INITIALIZATIONS_H
