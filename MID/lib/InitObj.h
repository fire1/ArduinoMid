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
// Debugging Command class
#include "CmdSerial.h"
//
//
#include "Melody.h"
//
// External LPG listener
#ifdef LPG_INSTALLATION

#include "LpgSerial.h"

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
//EepRom eepRom(carSens);
//
// Combined EepRom / CmdSerial
CmdSerial eepRom(carSens, whlSens);
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
LpgSerial lpgCom(ampInt, carSens);
#endif

#ifdef LPG_SWTC_STT
LpgSwitch lpgCom(ampInt);
#endif

//
// DRL
#ifndef DRL_PIN
#define DRL_PIN 7
#endif

//boolean flashDrl = false;
uint8_t fadeIDrl = 0;

void listenerDRL(AmpTime *amp, CarSens *car) {


    if (millis() < 3000) {
        analogWrite(10, 255);
        if (amp->isBig() && fadeIDrl < 255) {
            analogWrite(DRL_PIN, fadeIDrl);
            fadeIDrl++;
        } else if(fadeIDrl > 254){
            analogWrite(10, 0);
        }
    }

    if (amp->isSecond()) {
        if (car->isRunEng()) {
            if (car->isDimOn()) {
                analogWrite(10, 0);
                analogWrite(DRL_PIN, 0);
            } else {
                analogWrite(10, 255);
                analogWrite(DRL_PIN, 255);
            }
        } else {
            analogWrite(10, 255);
            if (amp->is5Seconds()) {
                analogWrite(DRL_PIN, 255);
            } else if (amp->isBig())
                analogWrite(DRL_PIN, 0);
        }
    }
}


#endif //ARDUINOMID_INITIALIZATIONS_H
