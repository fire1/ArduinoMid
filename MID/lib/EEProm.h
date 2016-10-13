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


//
//  Mega2560
// 	20 (SDA), 21 (SCL)
//
// Saves Addresses in EEP Rom
const int EEP_ADR_FTK = 0; // Fuel tank Astra G -  52 liter 14 gallons
const int EEP_ADR_TTD = 1; // Total Travel distance
const int EEP_ADR_TRD = 2; // Trip distance
const int EEP_ADR_TR2 = 3; // Trip distance
const int EEP_ADR_TTT = 4; // Total Trip Time
const int EEP_ADR_TER = 4; // Time Engine Run
const int EEP_ADR_TRS = 5; // Tires size
const int EEP_ADR_RMS = 6; // Rims Size
const int EEP_ADR_TC1 = 7; // Gas tank size
const int EEP_ADR_TC2 = 8; // Travel Consumption
//
// We’re using a 256kbit eeprom which is actually 32kbytes of space.
//      262,144 bits / 8 bits in a byte = 32,768 bytes.
//      That’s 62 times the Arduino’s built-in storage!




//
//
class EepRom {

public:
    void setup() { Wire.begin(); };

    void saveFuelTankLevel(unsigned int value = 0) {
        WireEepRomWriteByte(EEP_ADR_FTK, value);
    };

    int loadFuelTankLevel() {
        WireEepRomRead(EEP_ADR_FTK);
    };

    void saveTravelDistance(unsigned int value = 0) {
        WireEepRomWriteByte(EEP_ADR_TTD, value / 4);
    }

    int long loadTravelDistance() {
        return WireEepRomRead(EEP_ADR_TTD) * 4;
    }

    /**
     *  Saves travel consumption
     */
    void saveTravelConsumption(float value = 0) {
        int val[1];

        separateFloat(value, val);

#if defined(DEBUG_EEP_ROM)
        Serial.print("EEP ROM records value A: ");
        Serial.print(val[0]);
        Serial.print(" and  value B:");
        Serial.print(val[1]);
        Serial.print("\n\r");
#endif

        WireEepRomWriteByte(EEP_ADR_TC1, val[0]);
        WireEepRomWriteByte(EEP_ADR_TC2, val[1]);
    }

    /**
     *  Loads travel consumption
     */
    float loadTravelConsumption() {

        int va1 = WireEepRomRead(EEP_ADR_TC1);
        int va2 = WireEepRomRead(EEP_ADR_TC2);


#if defined(DEBUG_EEP_ROM)
        Serial.print("EEP ROM restore value A: ");
        Serial.print(va1);
        Serial.print(" and  value B:");
        Serial.print(va2);
        Serial.print("\n\r");
#endif

        return restoreFloat(va1, va2);
    }

    void saveTripDistance(unsigned int value = 0) {
        WireEepRomWriteByte(EEP_ADR_TRD, value / 4);
    }

    int long loadTripDistance(unsigned int value = 0) {
        return WireEepRomRead(EEP_ADR_TRD) * 4;
    }

    void saveCurrentData();

    void loadCurrentData();

private:
    byte noElem = 12;
    unsigned int baseAddress = 0;

    void WireEepRomWriteByte(uint16_t theMemoryAddress, int u8Byte) {

        Wire.beginTransmission(EEP_ROM_ADDRESS);
        Wire.write((theMemoryAddress >> 8) & 0xFF);
        Wire.write((theMemoryAddress >> 0) & 0xFF);
        Wire.write((uint8_t) u8Byte);
        Wire.endTransmission();
        delay(6);
    }


    uint8_t WireEepRomRead(uint16_t theMemoryAddress) {

        uint8_t u8retVal = 0;
        Wire.beginTransmission(EEP_ROM_ADDRESS);
        Wire.write((theMemoryAddress >> 8) & 0xFF);
        Wire.write((theMemoryAddress >> 0) & 0xFF);
        Wire.endTransmission();
        delay(10);
        Wire.requestFrom(EEP_ROM_ADDRESS, 1);
        u8retVal = Wire.read();
        return u8retVal;
    }
};

/**
 * Saves data to EepRom
 */
void EepRom::saveCurrentData() {
    TTL_TLH += CUR_TLH;
    saveTravelConsumption(TTL_TLH);
}


void EepRom::loadCurrentData() {


    TTL_TLH = loadTravelConsumption();

}


#endif