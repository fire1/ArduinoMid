//
// Created by Angel Zaprianov on 16.9.2016 г..
//

#ifndef ARDUINOMID_SENSINIT_H
#define ARDUINOMID_SENSINIT_H


//
// Read Tachometer
#include "sens/RpmSens.h"
//
// Read SpeedHub
#include "sens/VssSens.h"
//
//
#include "sens/EcuSens.h"
//
// Screen back-light sensor
#include "sens/DimSens.h"
//
// Temperature
#include "sens/TmpSens.h"

//
// Read inside temperature
#include "DispInfo.h"
void handleBackLight(){

}

void sensorsInit() {
    cli();
    //
    // Sense of car sensors ;)
    sensRpm();
    sensVss();
    sensEcu();
    sensAvr();
    sensTmp();
    sensCon();

    sensDim(); // Dim display
    sei();
}

#endif //ARDUINOMID_SENSINIT_H
