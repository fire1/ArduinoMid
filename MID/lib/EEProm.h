//#include <USBAPI.h>

//
// Created by Angel Zaprianov on 28.6.2016 г..
//
// Since EEPROM have live time will use external-sourcecode ...
// http://www.hobbytronics.co.uk/arduino-external-eeprom

// http://playground.arduino.cc/Main/LibraryForI2CEEPROM
#include <Wire.h>

#include "MainFunc.h"


#ifndef ARDUINOMID_EepRom_H
#define ARDUINOMID_EepRom_H

#define EEP_ROM_ADDRESS 0x50    //Address of 24LC256 eeprom chip
#define DEBUG_EEP_ROM
//#define EEP__INJ_SER true

//
//  Mega2560
// 	20 (SDA), 21 (SCL)
//
// Saves Addresses in EEP Rom
const int EEP_ADR_FTK = 0; // Fuel tank Astra G -  52 liter 14 gallons

const int EEP_ADR_CL1 = 1; // Consumption Float A LPG
const int EEP_ADR_Cl2 = 2; // Consumption Float B LPG

const int EEP_ADR_CB1 = 8; // Consumption Float A BNZ
const int EEP_ADR_CB2 = 9; // Consumption Float B BNZ

const int EEP_ADR_TT1 = 3; // Total Travel distance
const int EEP_ADR_TT2 = 4; // Total Travel distance

const int EEP_ADR_WD1 = 5; // Trip distance
const int EEP_ADR_WD2 = 6; // Trip distance
const int EEP_ADR_TRD = 7; // Trip distance

const int EEP_ADR_TRS = 5; // Tires size
const int EEP_ADR_RMS = 6; // Rims Size

// AT24C256
// We’re using a 256kbit eeprom which is actually 32kbytes of space.
//      262,144 bits / 8 bits in a byte = 32,768 bytes.
//      That’s 62 times the Arduino’s built-in storage!

//
//
class EepRom {

    CarSens *_car;

public:
    EepRom(CarSens *carSens);

    void setup() {
        Wire.begin();
    };

    //
    // Called when zeroed current data
    void saveZeroingData();

    void saveCurrentData();

    void loadCurrentData();


private:

    float loadLpgCons();

    void saveLpgCons(float value = 0);

    float loadBnzCons();

    void saveBnzCons(float value = 0);

    float loadWorkDistance();

    void saveWorkingDistance(float value = 0);

    float loadTravelDistance();

    void saveTravelDistance(float value = 0);

    void WireEepRomWriteByte(uint16_t theMemoryAddress, int u8Byte) {

        Wire.beginTransmission(EEP_ROM_ADDRESS);
        Wire.write((theMemoryAddress >> 8) & 0xFF);
        Wire.write((theMemoryAddress >> 0) & 0xFF);
        Wire.write((uint8_t) u8Byte);
        Wire.endTransmission();
        delay(10);
    }


    int WireEepRomRead(uint16_t theMemoryAddress) {

        int u8retVal = 0;
        Wire.beginTransmission(EEP_ROM_ADDRESS);
        Wire.write((theMemoryAddress >> 8) & 0xFF);
        Wire.write((theMemoryAddress >> 0) & 0xFF);
        Wire.endTransmission();
        delay(10);
        Wire.requestFrom(EEP_ROM_ADDRESS, 1);
        if (Wire.available()) u8retVal = Wire.read();
        return u8retVal;
    }

};

EepRom::EepRom(CarSens *carSens) {
    _car = carSens;
}


/**
 *  Loads travel consumption
 */
float EepRom::loadLpgCons() {
    int va1 = WireEepRomRead(EEP_ADR_CL1);
    int va2 = WireEepRomRead(EEP_ADR_Cl2);
    return restoreFloat(va1, va2);
}

/**
 *  Saves travel consumption
 */
void EepRom::saveLpgCons(float value = 0) {
    int val[2];
    separateFloat(value, val);
    WireEepRomWriteByte(EEP_ADR_CL1, val[0]);
    WireEepRomWriteByte(EEP_ADR_Cl2, val[1]);
}

/**
 *  Loads travel consumption
 */
float EepRom::loadBnzCons() {
    int va1 = WireEepRomRead(EEP_ADR_CB1);
    int va2 = WireEepRomRead(EEP_ADR_CB2);
    return restoreFloat(va1, va2);
}

/**
 *  Saves travel consumption
 */
void EepRom::saveBnzCons(float value = 0) {
    int val[2];
    separateFloat(value, val);
    WireEepRomWriteByte(EEP_ADR_CB1, val[0]);
    WireEepRomWriteByte(EEP_ADR_CB2, val[1]);
}

/**
 *  LOAD Travel distance
 */
float EepRom::loadTravelDistance() {
    int va1 = WireEepRomRead(EEP_ADR_TT1);
    int va2 = WireEepRomRead(EEP_ADR_TT2);
    return restoreFloat(va1, va2);
}

/**
 *  SAVE Travel distance
 */
void EepRom::saveTravelDistance(float value = 0) {
    int val[2];
    separateFloat(value, val);
    WireEepRomWriteByte(EEP_ADR_TT1, val[0]);
    WireEepRomWriteByte(EEP_ADR_TT2, val[1]);
}

/**
 *  LOAD Work distance
 */
float EepRom::loadWorkDistance() {
    int va1 = WireEepRomRead(EEP_ADR_WD1);
    int va2 = WireEepRomRead(EEP_ADR_WD2);
    return restoreFloat(va1, va2);
}

/**
 *  Saves Work distance
 */
void EepRom::saveWorkingDistance(float value = 0) {
    int val[2];
    separateFloat(value, val);
    WireEepRomWriteByte(EEP_ADR_WD1, val[0]);
    WireEepRomWriteByte(EEP_ADR_WD2, val[1]);
}

/*************************************************
 *  public
 *              Combined methods
 *
 */
/**
 * Saves data to EepRom
 */
void EepRom::saveCurrentData() {

    TTL_LPG = TTL_LPG + CRT_LPG;
    saveLpgCons(TTL_LPG);

    TTL_BNZ = TTL_BNZ + CRT_BNZ;
    saveBnzCons(TTL_BNZ);

    TTL_TTD = TTL_TTD + _car->getDst();
    saveTravelDistance(TTL_TTD);
}

/**
 * Load data from chip
 */
void EepRom::loadCurrentData() {
    TTL_LPG = loadLpgCons();
    TTL_BNZ = loadBnzCons();

    TTL_TTD = loadTravelDistance();
    TTL_WRD = loadWorkDistance();
}

/**
 * Saves final data
 */
void EepRom::saveZeroingData() {
    int assumedTravel = int(TTL_TTD);
    TTL_LPG = 0;
    TTL_BNZ = 0;

    TTL_TTD = 0;
    TTL_WRD = TTL_WRD + (assumedTravel / 1000);

    saveLpgCons(TTL_LPG);
    saveTravelDistance(TTL_TTD);
    saveWorkingDistance(TTL_WRD);
}


#endif