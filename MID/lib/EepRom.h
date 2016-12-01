//#include <USBAPI.h>

//
// Created by Angel Zaprianov on 28.6.2016 г..
//
// Since EEPROM have live time will use external-sourcecode ...
// http://www.hobbytronics.co.uk/arduino-external-eeprom

// http://playground.arduino.cc/Main/LibraryForI2CEEPROM
#include <Wire.h>
#include "CarSens.h"
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

struct SavedData {
    float fuel_adt;
    float fuel_def;
    float distance;
    float total_km;
};

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

/**
 * Injection data from USB serial monitor
 */
    void injectFromSerial(void);

/**
 * Gets worked distance
 * @return
 */
    float getWorkDistance(void);

/**
 * Gets travel distance
 * @return
 */
    float getTraveDistance(void);

/**
 * Gets default consumed fuel liters
 * @return
 */
    float getDefFuel(void);

/**
 * Gets additional consumed fuel liters
 * @return
 */
    float getAdtFuel(void);

/**
 * Sets new Car's worked distance
 * @param value
 * @return
 */
    void setWorkDistance(float value);

/**
 * Sets new travel distance
 * @param value
 * @return
 */
    void setTravelDistance(float value);

/**
 * Sets new Default consumed liters
 * @param value
 * @return
 */
    void setDefFuel(float value);

/**
 * Sets new Additional consumed liters
 * @param value
 * @return
 */
    void setAdtFuel(float value);

/**
 *  Gets all saved data
 * @return SavedData
 */
    SavedData getData(void);

private:
/**
 * Container of saved data
 */
    SavedData container;

/**
 *
 * @return
 */
    float loadAdtCons();

/**
 *
 * @param value
 */
    void saveAdtCons(float value = 0);

/**
 * Loads default consumption
 * @return
 */
    float loadDefCons();

/**
 * Loads default consumption
 * @param value
 */
    void saveDefCons(float value = 0);

/**
 * Loads Work distance
 * @return
 */
    float loadWorkDistance();

/**
 * Saves work distance
 * @param value
 */
    void saveWorkingDistance(float value = 0);

/**
 * loads travel distance
 * @return
 */
    float loadTravelDistance();

/**
 * Saves travel distance
 * @param value
 */
    void saveTravelDistance(float value = 0);

/**
 * Write data driver
 * @param theMemoryAddress
 * @param u8Byte
 */
    void WireEepRomWriteByte(uint16_t theMemoryAddress, int u8Byte);

/**
 * Read data driver
 * @param theMemoryAddress
 * @return
 */
    int WireEepRomRead(uint16_t theMemoryAddress);

};

/**
 * EepRom Constructor
 * @param carSens
 */
EepRom::EepRom(CarSens *carSens) {
    _car = carSens;
}

/**
 *
 * @param theMemoryAddress
 * @param u8Byte
 */
void EepRom::WireEepRomWriteByte(uint16_t theMemoryAddress, int u8Byte) {

    Wire.beginTransmission(EEP_ROM_ADDRESS);
    Wire.write((theMemoryAddress >> 8) & 0xFF);
    Wire.write((theMemoryAddress >> 0) & 0xFF);
    Wire.write((uint8_t) u8Byte);
    Wire.endTransmission();
    delay(10);
}

/**
 *
 * @param theMemoryAddress
 * @return
 */
int EepRom::WireEepRomRead(uint16_t theMemoryAddress) {

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

/**
 *  Loads travel consumption
 */
float EepRom::loadAdtCons() {
    int va1 = WireEepRomRead(EEP_ADR_CL1);
    int va2 = WireEepRomRead(EEP_ADR_Cl2);
    return restoreFloat(va1, va2);
}

/**
 *  Saves travel consumption
 */
void EepRom::saveAdtCons(float value) {
    int val[2];
    separateFloat(value, val);
    WireEepRomWriteByte(EEP_ADR_CL1, val[0]);
    WireEepRomWriteByte(EEP_ADR_Cl2, val[1]);
}

/**
 *  Loads travel consumption
 */
float EepRom::loadDefCons() {
    int va1 = WireEepRomRead(EEP_ADR_CB1);
    int va2 = WireEepRomRead(EEP_ADR_CB2);
    return restoreFloat(va1, va2);
}

/**
 *  Saves travel consumption
 */
void EepRom::saveDefCons(float value) {
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
void EepRom::saveTravelDistance(float value) {
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
void EepRom::saveWorkingDistance(float value) {
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

    container.fuel_adt = container.fuel_adt + _car->getAdtFuelCns();
    saveAdtCons(container.fuel_adt);

    container.fuel_def = container.fuel_def + _car->getDefFuelCns();
    saveDefCons(container.fuel_def);

    container.distance = container.distance + _car->getDst();
    saveTravelDistance(container.distance);
}

/**
 * Load data from chip
 */
void EepRom::loadCurrentData() {
    container.fuel_adt = loadAdtCons();
    container.fuel_def = loadDefCons();
    container.distance = loadTravelDistance();
    container.total_km = loadWorkDistance();
    delay(5);
}

/**
 * Saves final data
 */
void EepRom::saveZeroingData() {
    int assumedTravel = int(container.distance);
    container.fuel_adt = 0;
    container.fuel_def = 0;

    container.distance = 0;
    container.total_km = container.total_km + (assumedTravel / 1000);

    saveAdtCons(container.fuel_adt);
    saveDefCons(container.fuel_def);
    saveTravelDistance(container.distance);
    saveWorkingDistance(container.total_km);
}

/**
 * Gets Car's total distance
 * @return
 */
float EepRom::getWorkDistance(void) {
    return container.total_km;
}

/**
 * Gets saved travel distance
 * @return
 */
float EepRom::getTraveDistance(void) {
    return container.distance;
}

/**
 * Gets default consumed fuel
 * @return
 */
float EepRom::getDefFuel(void) {
    return container.fuel_def;
}

/**
 * Gets additional consumed fuel
 * @return
 */
float EepRom::getAdtFuel(void) {
    return container.fuel_adt;
}

/**
 * Gets SavedData data
 * @return
 */
SavedData EepRom::getData(void) {
    return container;
}

/**
 *
 * @param value
 * @return
 */
void EepRom::setWorkDistance(float value) {
    container.total_km = value;
}

/**
 *
 * @param value
 * @return
 */
void EepRom::setTravelDistance(float value) {
    container.distance = value;
}

/**
 *
 * @param value
 * @return
 */
void EepRom::setDefFuel(float value) {
    container.fuel_def = value;
}

/**
 *
 * @param value
 * @return
 */
void EepRom::setAdtFuel(float value) {
    container.fuel_adt = value;
}

/**
 * Injection data from USB serial monitor
 */
void EepRom::injectFromSerial(void) {
    //
// Serial injection
#if defined(SERIAL_INJECT_DATA)
    String srlAllData;
    String srlStrName;
    String srlOutputs;
    //
    // Default message
    srlOutputs = " None ... Sorry! ";
    //
    // Execute command from serial
    if (Serial.available() > 0) {
        float saveTemp;

        srlStrName = Serial.readStringUntil('=');
        //
        // So ... C++ is static language...
        //      is not good idea to re-set dynamically
        if (srlStrName == "lpg") {
            // Total Liters per hour consumed
            saveTemp = Serial.readStringUntil('\n').toInt() * 0.01;
            setAdtFuel(saveTemp);
            srlOutputs = "LPG fuel ";
            srlOutputs += saveTemp;
        }
        if (srlStrName == "bnz") {
            // Total Liters per hour consumed
            saveTemp = Serial.readStringUntil('\n').toInt() * 0.01;
            setDefFuel(saveTemp);
            srlOutputs = "BNZ fuel ";
            srlOutputs += saveTemp;
        }
        if (srlStrName == "ttd") {
            // Total Travel distance
            saveTemp = Serial.readStringUntil('\n').toInt() * 0.01;
            setTravelDistance(saveTemp);
            srlOutputs = "Travel distance ";
            srlOutputs += saveTemp;
        }
        if (srlStrName == "wrd" || srlStrName == "wrk") {
            // Total Liters per hour consumed
            saveTemp = Serial.readStringUntil('\n').toInt() * 0.01;
            setWorkDistance(saveTemp);
            srlOutputs = "Work distance ";
            srlOutputs += saveTemp;
        }
        //
        // Show command information to human
        Serial.print("\n\n==============================================================\n");
        Serial.print("\n \t\t  [MID $]> Affected value of " + srlOutputs + "\n\n");
        Serial.print("\n==============================================================\n\n");
        //
        // Remove containers
        srlStrName = "";
        srlAllData = ""; // Clear recieved buffer
        srlOutputs = "";
    }
#endif
}


#endif