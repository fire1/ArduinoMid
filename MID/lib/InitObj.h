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
// Data recorder
#include "EepRom.h"
//
// Add library
#include "ShutDw.h"
//
// Menu constructor
#include "MenuBase.h"
//
// Car data collector
#include "CarCollect.h"
//
// External LPG listener
#ifdef LPG_INSTALLATION

#include "LpgSerial.h"
#include "DbgSerial.h"
#include "Melody.h"

#endif
//
// Amplitude interval
//    ampInt(min,low,mid,sec, big, max);
AmpTime ampInt;
//
//
Melody melody(ampInt);
//
// Constructing the class
CarSens carSens(ampInt, melody);
//
// Constructing car state
CarState carStat(ampInt, carSens);
//
// Constructing the class
WhlSens whlSens(ampInt, carSens);
//
// Data storage
EepRom eepRom(carSens);

DbgSerial dbgSerial(carSens, eepRom, whlSens);
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
