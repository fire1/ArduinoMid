//
// Created by Angel Zaprianov on 28.6.2016 г..
//
// Since EEPROM have live time will use external-sourcecode ...
// http://www.hobbytronics.co.uk/arduino-external-eeprom

// http://playground.arduino.cc/Main/LibraryForI2CEEPROM
#ifdef EEP_ROM_ADDRESS

#include <Wire.h>

#else

#include <EEPROM.h>

#endif


#include "CarSens.h"
#include "MainFunc.h"
#include "WhlSens.h"

#ifndef ARDUINOMID_EepRom_H
#define ARDUINOMID_EepRom_H

//#define EEP_ROM_ADDRESS 0x50    //Address of 24LC256 eeprom chip
#define DEBUG_EEP_ROM
//#define EEP__INJ_SER true

//
//  Mega2560
// 	20 (SDA), 21 (SCL)
//
// Saves Addresses in EEP Rom
const int EEP_ADR_FTK = 0; // Fuel tank Astra G -  52 liter 14 gallons

const uint8_t EEP_ADR_CL1 = 1; // Consumption Float A LPG
const uint8_t EEP_ADR_Cl2 = 2; // Consumption Float B LPG

const uint8_t EEP_ADR_CB1 = 8; // Consumption Float A BNZ
const uint8_t EEP_ADR_CB2 = 9; // Consumption Float B BNZ
// Fuel distance
const uint8_t EEP_ADR_TD1 = 3; //  Travel distance
const uint8_t EEP_ADR_TD2 = 4; //  Travel distance
// Global car distance
const uint8_t EEP_ADR_WD1 = 5; // Work distance
const uint8_t EEP_ADR_WD2 = 6; // Work distance
// Trip distance
const uint8_t EEP_ADR_TR1 = 10; // Trip distance
const uint8_t EEP_ADR_TR2 = 11; // Trip distance
// Trip time
const uint8_t EEP_ADR_TT1 = 12; // Trip Time
const uint8_t EEP_ADR_TT2 = 13; // Trip Time

const int EEP_ADR_TRS = 5; // Tires size
const int EEP_ADR_RMS = 6; // Rims Size

// AT24C256
// We’re using a 256kbit eeprom which is actually 32kbytes of space.
//      262,144 bits / 8 bits in a byte = 32,768 bytes.
//      That’s 62 times the Arduino’s built-in storage!

#ifndef EEP_ROM_WORK_DIV
#define EEP_ROM_WORK_DIV 100
#endif

#define EEP_ROM_INDEXES 17

//
//
class EepRom {

    SavedData container;
    CarSens *car;
    WhlSens *whl;
    float data[EEP_ROM_INDEXES] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

public:
/**
 * EepRom Constructor
 * @param carSens
 */
    EepRom(CarSens &carSens, WhlSens &whlSens) : car(&carSens), whl(&whlSens) {

    }

    void setup() {
#ifdef EEP_ROM_ADDRESS
        Wire.begin();
#endif

    };

    //
    // Called when zeroed current data
    void saveResetData();

    void saveCurrentData();

    void loadCurrentData();

    void injectFromSerial(void);


    float getAverageLitersPer100km() {
        return (getConsumedFuel() * 100) / (container.dist_trv + car->getDst());
    }

/**
 *
 */
    float getConsumedFuel() {

#ifdef SWITCH_FUEL_ON_STATE
        //
        // Switching between LPG / BNZ
        if (car->getFuelState() == 0) { // BNZ [default]
            return container.fuel_def + car->getDefFuelCns();
        }
        if (car->getFuelState() == 1) { // LPG [additional]
            return container.fuel_adt + car->getAdtFuelCns();
        }
#else
        return container.fuel_def + car->getDefFuelCns();
#endif
    }

/** TODO needs to be changed
 * Saves trip data in order to continue the trip
 */
    inline void saveTripData() {
        float time = millis() / MILLIS_PER_HR;
        container.dist_trp = container.dist_trp + car->getDst();
        container.time_trp = container.time_trp + time;
//    saveTripTime(container.time_trp);
//    saveTripDistance(container.dist_trp);
        saveCurrentData(); // Ordinary save of data
    }

/**
 * Clears trip data
 */
    inline void clearTripData() {
        container.time_trp = 0;
        container.dist_trp = 0;
    }

/**
 * Gets Car's total distance
 * @return
 */
    inline int getWorkDistance(void) {
        return int(container.total_km * EEP_ROM_WORK_DIV);
    }

/**
 * Gets saved travel distance
 * @return
 */
    inline float getTravelDistance(void) {
        return car->getDst() + container.dist_trv;
    }

/**
 * Gets default consumed fuel
 * @return
 */
    inline float getDefFuel(void) {
        return container.fuel_def;
    }

/**
 * Gets additional consumed fuel
 * @return
 */
    inline float getAdtFuel(void) {
        return container.fuel_adt;
    }

    inline float getSensVss(void) {
        return container.sens_vss;
    }

    inline float getSensRpm(void) {
        return container.sens_rpm;
    }

    inline float getSensDst(void) {
        return container.sens_dst;
    }

    inline float getSensEco(void) {
        return container.sens_ecu;
    }

/**
 * Gets SavedData data
 * @return
 */
    inline SavedData getData(void) {
        return container;
    }

/**
 *
 * @param value
 * @return
 */
    inline void setWorkDistance(float value) {
        container.total_km = value;
    }

/**
 *
 * @param value
 * @return
 */
    inline void setTravelDistance(float value) {
        container.dist_trv = value;
    }

/**
 *
 * @param value
 * @return
 */
    inline void setDefFuel(float value) {
        container.fuel_def = value;
    }

/**
 *
 * @param value
 * @return
 */
    inline void setAdtFuel(float value) {
        container.fuel_adt = value;
    }

/**
 *
 * @param value
 * @return void
 */
    inline void setSensVss(float value) {
        container.sens_vss = value;
    }

    /*
   * @param value
   * @return void
   */
    inline void setSensRpm(float value) {
        container.sens_rpm = value;
    }

/**
 *
 * @param value
 * @return void
 */
    inline void setSensDst(float value) {
        container.sens_dst = value;
    }

/**
 *
 * @param value
 * @return void
 */
    inline void setSensEcu(float value) {
        container.sens_ecu = value;
    }

    TripData getTrip0() {
        TripData trip;
        trip.fuel = getDefaultFuelUse();
        trip.range = car->getDst();
        return trip;
    }

    TripData getTripA() {
        return container.trip_a;
    }

    TripData getTripB() {
        return container.trip_b;
    }

    TripData getTripC() {
        return container.trip_c;
    }

    void resetTripA() {
        container.trip_a.fuel = 0;
        container.trip_a.range = 0;
    }

    void resetTripB() {
        container.trip_b.fuel = 0;
        container.trip_b.range = 0;
    }

    void resetTripC() {
        container.trip_c.fuel = 0;
        container.trip_c.range = sqrt(-1);
    }


private:


    void fixtureTripNan() {
        if (isnan(container.trip_a.fuel) || isnan(container.trip_a.range)) {
            resetTripA();
            Serial.println("Fix nan trip A");
        }
        if (isnan(container.trip_b.fuel) || isnan(container.trip_b.range)) {
            resetTripB();
            Serial.println("Fix nan trip B");
        }
        if (isnan(container.trip_c.fuel) || isnan(container.trip_c.range)) {
            resetTripC();
            Serial.println("Fix nan trip C");
        }
    }

    float getClearNan(float data) {
        if (isnan(data)) {
            return sqrt(-1);
        }
        return data;
    }

    //
    // Gets target fuel of use
    float getDefaultFuelUse() {
#if (DEFAULT_FUEL_USING == 1)
        return car->getAdtFuelCns();
#else
        return car->getDefFuelCns();
#endif
    }

/**
 *
 * @return
 *
    float loadAdtCons();

/**
 *
 * @param value
 *
    void saveAdtCons(float value = 0);

/**
 * Loads default consumption
 * @return
 *
    float loadDefCons();

/**
 * Loads default consumption
 * @param value
 *
    void saveDefCons(float value = 0);

/**
 * Loads Work distance
 * @return
 *
    float loadWorkDistance();

/**
 * Saves work distance
 * @param value
 *
    void saveWorkingDistance(float value = 0);

/**
 * loads travel distance
 * @return
 *
    float loadTravelDistance();

/**
 * Saves travel distance
 * @param value
 *
    void saveTravelDistance(float value = 0);

/**
 * Load trip distance
 *
    float loadTripDistance();

/**
 * Save trip distance
 *
    void saveTripDistance(float value = 0);

/**
 * Save trip Time
 *
    float loadTripTime();

/**
 * Save trip Time
 *
    void saveTripTime(float value = 0);

/**
 * Write data driver
 * @param theMemoryAddress
 * @param u8Byte
 */
    void WireEepRomWriteByte(uint16_t theMemoryAddress, uint8_t u8Byte);

/**
 * Read data driver
 * @param theMemoryAddress
 * @return
 */
    uint8_t WireEepRomRead(uint16_t theMemoryAddress);

};


/**
 *
 * @param theMemoryAddress
 * @param u8Byte
 */
void EepRom::WireEepRomWriteByte(uint16_t theMemoryAddress, uint8_t u8Byte) {

#ifdef  EEP_ROM_ADDRESS
    Wire.beginTransmission(EEP_ROM_ADDRESS);
    Wire.write((theMemoryAddress >> 8) & 0xFF);
    Wire.write((theMemoryAddress >> 0) & 0xFF);
    Wire.write((uint8_t) u8Byte);
    Wire.endTransmission();
#else
    EEPROM.write(theMemoryAddress, u8Byte);
#endif

    delay(10);
}

/**
 *
 * @param theMemoryAddress
 * @return
 */
uint8_t EepRom::WireEepRomRead(uint16_t theMemoryAddress) {
    uint8_t u8retVal = 0;
#ifdef  EEP_ROM_ADDRESS

    Wire.beginTransmission(EEP_ROM_ADDRESS);
    Wire.write((theMemoryAddress >> 8) & 0xFF);
    Wire.write((theMemoryAddress >> 0) & 0xFF);
    Wire.endTransmission();
    delay(10);
    Wire.requestFrom(EEP_ROM_ADDRESS, 1);
    if (Wire.available()) u8retVal = Wire.read();
#else
    u8retVal = EEPROM.read(theMemoryAddress);
#endif
    return u8retVal;
}

/**
 *  Loads travel consumption
 *
float EepRom::loadAdtCons() {
    uint8_t va1 = WireEepRomRead(EEP_ADR_CL1);
    uint8_t va2 = WireEepRomRead(EEP_ADR_Cl2);
    return restoreFloat(va1, va2);
}

/**
 *  Saves travel consumption
 *
void EepRom::saveAdtCons(float value) {
    uint8_t val[2];
    separateFloat(value, val);
    WireEepRomWriteByte(EEP_ADR_CL1, (uint8_t) val[0]);
    WireEepRomWriteByte(EEP_ADR_Cl2, (uint8_t) val[1]);
}

/**
 *  Loads travel consumption
 *
float EepRom::loadDefCons() {
    uint8_t va1 = WireEepRomRead(EEP_ADR_CB1);
    uint8_t va2 = WireEepRomRead(EEP_ADR_CB2);
    return restoreFloat(va1, va2);
}

/**
 *  Saves travel consumption
 *
void EepRom::saveDefCons(float value) {
    uint8_t val[2];
    separateFloat(value, val);
    WireEepRomWriteByte(EEP_ADR_CB1, val[0]);
    WireEepRomWriteByte(EEP_ADR_CB2, val[1]);
}

/**
 *  LOAD Travel distance
 *
float EepRom::loadTripDistance() {
    uint8_t va1 = WireEepRomRead(EEP_ADR_TR1);
    uint8_t va2 = WireEepRomRead(EEP_ADR_TR2);
    return restoreFloat(va1, va2);
}

/**
 *  SAVE Travel distance
 *
void EepRom::saveTripDistance(float value) {
    uint8_t val[2];
    separateFloat(value, val);
    WireEepRomWriteByte(EEP_ADR_TR1, val[0]);
    WireEepRomWriteByte(EEP_ADR_TR2, val[1]);
}

/**
 *  LOAD Travel distance
 *
float EepRom::loadTripTime() {
    uint8_t va1 = WireEepRomRead(EEP_ADR_TT1);
    uint8_t va2 = WireEepRomRead(EEP_ADR_TT2);
    return restoreFloat(va1, va2);
}

/**
 *  SAVE Travel distance
 *
void EepRom::saveTripTime(float value) {
    uint8_t val[2];
    separateFloat(value, val);
    WireEepRomWriteByte(EEP_ADR_TT1, val[0]);
    WireEepRomWriteByte(EEP_ADR_TT2, val[1]);
}

/**
 *  LOAD Travel distance
 *
float EepRom::loadTravelDistance() {
    uint8_t va1 = WireEepRomRead(EEP_ADR_TD1);
    uint8_t va2 = WireEepRomRead(EEP_ADR_TD2);
    return restoreFloat(va1, va2);
}

/**
 *  SAVE Travel distance
 *
void EepRom::saveTravelDistance(float value) {
    uint8_t val[2];
    separateFloat(value, val);
    WireEepRomWriteByte(EEP_ADR_TD1, val[0]);
    WireEepRomWriteByte(EEP_ADR_TD2, val[1]);
}

/**
 *  LOAD Work distance
 *
float EepRom::loadWorkDistance() {
    uint8_t va1 = WireEepRomRead(EEP_ADR_WD1);
    uint8_t va2 = WireEepRomRead(EEP_ADR_WD2);
    return restoreFloat(va1, va2);
}

/**
 *  Saves Work distance
 *
void EepRom::saveWorkingDistance(float value) {
    uint8_t val[2];
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

    data[1] = container.fuel_adt + car->getAdtFuelCns();
    data[2] = container.fuel_def + car->getDefFuelCns();
    data[3] = container.dist_trv + car->getDst();
    data[4] = container.total_km;
    // trip
    data[5] = container.dist_trp;
    data[6] = container.time_trp;
    //
    // Sens editor
    data[7] = container.sens_vss;
    data[8] = container.sens_rpm;
    data[9] = container.sens_dst;
    data[10] = container.sens_ecu;
    //
    // Trip records
    data[11] = container.trip_a.fuel + getDefaultFuelUse();
    data[12] = container.trip_a.range + car->getDst();

    data[13] = container.trip_b.fuel + getDefaultFuelUse();
    data[14] = container.trip_b.range + car->getDst();

    data[15] = container.trip_c.fuel + getDefaultFuelUse();
    data[16] = container.trip_c.range + car->getDst();


    for (int i = 1; i < (EEP_ROM_INDEXES + 1); i++) {
        EEPROM.put(i * sizeof(data[i]), data[i]);
    }
}

/**
 * Saves final data
 */
void EepRom::saveResetData() {
    //
    // Calculate total work distance
    if (container.dist_trv > 10) {
        if (container.total_km < 1) {
            container.total_km = 0;
        }
        container.total_km = container.total_km + (container.dist_trv / EEP_ROM_WORK_DIV);
    }
    //
    // Resenting ...
    data[1] = container.fuel_adt = 0;
    data[2] = container.fuel_def = 0;
    data[3] = container.dist_trv = 0;
    //
    // Pass work distance for save
    data[4] = container.total_km;
    // trip
    data[5] = container.dist_trp;
    data[6] = container.time_trp;

    //
    // Sens editor
    data[7] = container.sens_vss;
    data[8] = container.sens_rpm;
    data[9] = container.sens_dst;
    data[10] = container.sens_ecu;

    //
    // Trip record
    data[11] = container.trip_a.fuel + getDefaultFuelUse();
    data[12] = container.trip_a.range + car->getDst();

    data[13] = container.trip_b.fuel + getDefaultFuelUse();
    data[14] = container.trip_b.range + car->getDst();

    data[15] = container.trip_c.fuel + getDefaultFuelUse();
    data[16] = container.trip_c.range + car->getDst();

    for (int i = 1; i < (EEP_ROM_INDEXES + 1); i++) {
        EEPROM.put(i * sizeof(data[i]), data[i]);
        delay(5);
    }
    //
    // In order to fix and clear assumed data
    car->clearBuffer();

}

/**
 * Load data from chip
 */
void EepRom::loadCurrentData() {

    float eGetValue;
    int eLocation = 0;

    for (int i = 0; i < (EEP_ROM_INDEXES + 1); i++) {

        Serial.println(F("Restore Value:"));
        EEPROM.get(eLocation, eGetValue);
        Serial.println(eGetValue, 2);
        eLocation = eLocation + sizeof(eGetValue);
        Serial.println("");
        data[i] = eGetValue;
        delay(5);
    }

    container.fuel_adt = data[1];
    container.fuel_def = data[2];
    container.dist_trv = data[3];
    container.total_km = data[4];
    // Trip
    container.dist_trp = data[5];
    container.time_trp = data[6];

    //
    // Sens Edit
    container.sens_vss = data[7];
    container.sens_rpm = data[8];
    container.sens_dst = data[9];
    container.sens_ecu = data[10];

    //
    // Trips record
    container.trip_a.fuel = data[11];
    container.trip_a.range = data[12];

    container.trip_b.fuel = data[13];
    container.trip_b.range = data[14];

    container.trip_c.fuel = data[15];
    container.trip_c.range = data[16];
    this->fixtureTripNan(); // Fix broken not NAN  values


    Serial.println(data[1], 2);
    Serial.println(data[2], 2);
    Serial.println(data[3], 2);
    Serial.println(data[4], 2);
    Serial.println(data[7]);
    Serial.println(data[8]);
    Serial.println(data[9]);
    Serial.println(data[10]);
    delay(10);
    Serial.println("Trip records: ");
    Serial.println(data[11]);
    Serial.println(data[12]);
    Serial.println(data[13]);
    Serial.println(data[14]);
    Serial.println(data[15]);
    Serial.println(data[16]);
    delay(10);
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
    srlOutputs = F(" None ... Sorry! ");
    //
    // Execute command from serial
    if (Serial.available() > 0) {
        float saveTemp;

        srlStrName = Serial.readStringUntil('=');
        //
        // So ... C++ is static language...
        //      is not good idea to re-set dynamically

        if (srlStrName == "trd") {
            // trip distance
            saveTemp = Serial.readStringUntil('\n').toFloat();
            container.dist_trp = saveTemp;
            srlOutputs = F("Trip distance ");
            srlOutputs += saveTemp;
        }

        if (srlStrName == "trt") {
            // Trip time
            saveTemp = Serial.readStringUntil('\n').toFloat();
            container.time_trp = saveTemp;
            srlOutputs = F("Trip time ");
            srlOutputs += saveTemp;
        }

        if (srlStrName == "set_tr") {
            // Saves type
            saveTemp = Serial.readStringUntil('\n').toInt();
            if (saveTemp == 1)saveTripData();
            srlOutputs = F("Change trip data ");
            srlOutputs += saveTemp;
        }

        if (srlStrName == "lpg") {
            // Total Liters per hour consumed
            saveTemp = Serial.readStringUntil('\n').toFloat();
            setAdtFuel(saveTemp);
            srlOutputs = F("LPG fuel ");
            srlOutputs += saveTemp;
        }
        if (srlStrName == "bnz") {
            // Total Liters per hour consumed
            saveTemp = Serial.readStringUntil('\n').toFloat();
            setDefFuel(saveTemp);
            srlOutputs = F("BNZ fuel ");
            srlOutputs += saveTemp;
        }
        if (srlStrName == "ttd") {
            // Total Travel distance
            saveTemp = Serial.readStringUntil('\n').toFloat();
            setTravelDistance(saveTemp);
            srlOutputs = F("Travel distance ");
            srlOutputs += saveTemp;
        }
        if (srlStrName == "wrk") {
            // Total work distance
            saveTemp = Serial.readStringUntil('\n').toFloat();
            setWorkDistance(saveTemp);
            srlOutputs = F("Work distance ");
            srlOutputs += saveTemp;
        }
        //
        // Correction value inject
        if (srlStrName == "cor_rpm") {
            // Total work distance
            saveTemp = Serial.readStringUntil('\n').toFloat();
            setSensRpm(saveTemp);
            srlOutputs = F("RPM correction ");
            srlOutputs += saveTemp;
        }
        if (srlStrName == "cor_vss") {
            // Total work distance
            saveTemp = Serial.readStringUntil('\n').toFloat();
            setSensVss(saveTemp);
            srlOutputs = F("VSS correction ");
            srlOutputs += saveTemp;
        }
        if (srlStrName == "cor_dst") {
            // Total work distance
            saveTemp = Serial.readStringUntil('\n').toFloat();
            setSensDst(saveTemp);
            srlOutputs = F("DST correction ");
            srlOutputs += saveTemp;
        }

        if (srlStrName == "cor_ecu") {
            // Total work distance
            saveTemp = Serial.readStringUntil('\n').toFloat();
            setSensEcu(saveTemp);
            srlOutputs = F("ECU correction ");
            srlOutputs += saveTemp;
        }

        if (srlStrName == "set_cor") {
            // Saves type
            saveTemp = Serial.readStringUntil('\n').toInt();
            if (saveTemp == 1) {
                car->setSave(container);
                srlOutputs = F("Passing value to carSens ");
            }
        }

        if (srlStrName == "set_wrk") {
            // Saves type
            saveTemp = Serial.readStringUntil('\n').toInt();
            if (saveTemp == 1) {
                saveResetData();
                srlOutputs = F("Saved Work distance ");
                srlOutputs += saveTemp;
            }
        }
        if (srlStrName == "save") {
            // Saves type
            saveTemp = Serial.readStringUntil('\n').toInt();
            if (saveTemp == 1) {
                saveCurrentData();
                srlOutputs = F("Saved All data ");
                srlOutputs += saveTemp;
            }

        }
        if (srlStrName == "reset") {
            // Saves type
            saveTemp = Serial.readStringUntil('\n').toInt();
            if (saveTemp == 1) {
                this->saveResetData();
                srlOutputs = F("Saved All data and resetting...");
                srlOutputs += saveTemp;
            }

        }


        if (srlStrName == "whl") {
            // Saves type
            saveTemp = Serial.readStringUntil('\n').toInt();
            srlOutputs = F("Wheel Simulate resistance ");
            whl->sendRadioButtons(saveTemp);
            srlOutputs += saveTemp;
        }

        //
        // Trips record
        //
        // Trip A
        if (srlStrName == "tf1") {
            saveTemp = Serial.readStringUntil('\n').toFloat();
            srlOutputs = F("Sets Trip A fuel ");
            container.trip_a.fuel = saveTemp;
            srlOutputs += saveTemp;
        }
        if (srlStrName == "td1") {
            saveTemp = Serial.readStringUntil('\n').toFloat();
            srlOutputs = F("Sets Trip A range ");
            container.trip_a.range = saveTemp;
            srlOutputs += saveTemp;
        }
        //
        // Trip B
        if (srlStrName == "tf2") {
            saveTemp = Serial.readStringUntil('\n').toFloat();
            srlOutputs = F("Sets Trip B fuel ");
            container.trip_b.fuel = saveTemp;
            srlOutputs += saveTemp;
        }
        if (srlStrName == "td2") {
            saveTemp = Serial.readStringUntil('\n').toFloat();
            srlOutputs = F("Sets Trip B range ");
            container.trip_b.range = saveTemp;
            srlOutputs += saveTemp;
        }
        //
        // Trip C
        if (srlStrName == "tf3") {
            saveTemp = Serial.readStringUntil('\n').toFloat();
            srlOutputs = F("Sets Trip C fuel ");
            container.trip_c.fuel = saveTemp;
            srlOutputs += saveTemp;
        }
        if (srlStrName == "td3") {
            saveTemp = Serial.readStringUntil('\n').toFloat();
            srlOutputs = F("Sets Trip C range ");
            container.trip_c.range = saveTemp;
            srlOutputs += saveTemp;
        }

        //
        // Show command information to human
        Serial.print(F("\n\n==============================================================\n"));
        Serial.print(F("\n \t\t  [MID $]> Affected value of "));
        Serial.print(srlOutputs);
        Serial.print(F("\n\n\n==============================================================\n\n"));
        //
        // Remove containers
        srlStrName = "";
        srlAllData = ""; // Clear recieved buffer
        srlOutputs = "";
    }
#endif
}


#endif