//
// Created by Angel Zaprianov on 2.1.2018 г..
//

#ifndef ARDUINO_MID_DBG_SERIAL_H
#define ARDUINO_MID_DBG_SERIAL_H

#include <Arduino.h>
#include "../glob.h"
#include "CarSens.h"
#include "EepRom.h"
#include "WhlSens.h"
#include "InitObj.h"

#define SERIAL_INJECT_DATA

class DbgSerial {

    CarSens *car;
    EepRom *eep;
    WhlSens *whl;


private:

    float getSrlFloat() {
        return Serial.readStringUntil('\n').toFloat();
    }

    long getSrlInt() {
        return Serial.readStringUntil('\n').toInt();
    }

    long getSrlInt2() {
        return Serial.readStringUntil('+').toInt();
    }


public:
    DbgSerial(CarSens &carSens, EepRom &eepRom, WhlSens &whlSens) : car(&carSens), eep(&eepRom), whl(&whlSens) {

    }

    void listener(void) {
        //
// Serial injection

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

            if (srlStrName.length() > 1) {
                SavedData savedData = eep->getData();


                //
                // So ... C++ is static language...
                //      is not good idea to re-set dynamically
                if (srlStrName == "trd") {
                    // trip distance
                    saveTemp = getSrlFloat();
                    savedData.dist_trp = saveTemp;
                    srlOutputs = F("Trip distance ");
                    srlOutputs += saveTemp;
                }

                if (srlStrName == "trt") {
                    // Trip time
                    saveTemp = getSrlFloat();
                    savedData.time_trp = saveTemp;
                    srlOutputs = F("Trip time ");
                    srlOutputs += saveTemp;
                }

                if (srlStrName == "set_tr") {
                    // Saves type
                    saveTemp = getSrlInt();
                    if (saveTemp == 1)eep->saveTripData();
                    srlOutputs = F("Change trip data ");
                    srlOutputs += saveTemp;
                }
                if (srlStrName == "lpg") {
                    // Total Liters per hour consumed
                    saveTemp = getSrlFloat();
                    eep->setAdtFuel(saveTemp);
                    srlOutputs = F("LPG fuel ");
                    srlOutputs += saveTemp;
                }
                if (srlStrName == "bnz") {
                    // Total Liters per hour consumed
                    saveTemp = getSrlFloat();
                    eep->setDefFuel(saveTemp);
                    srlOutputs = F("BNZ fuel ");
                    srlOutputs += saveTemp;
                }
                if (srlStrName == "ttd") {
                    // Total Travel distance
                    saveTemp = getSrlFloat();
                    eep->setTravelDistance(saveTemp);
                    srlOutputs = F("Travel distance ");
                    srlOutputs += saveTemp;
                }
                if (srlStrName == "wrk") {
                    // Total work distance
                    saveTemp = getSrlFloat();
                    eep->setWorkDistance(saveTemp);
                    srlOutputs = F("Work distance ");
                    srlOutputs += saveTemp;
                }
                // ************************************************************
                // Correction value inject
                if (srlStrName == "cor_rpm") {
                    // Total work distance
                    saveTemp = getSrlFloat();
                    eep->setSensRpm(saveTemp);
                    srlOutputs = F("RPM correction ");
                    srlOutputs += saveTemp;
                }
                if (srlStrName == "cor_vss") {
                    // Total work distance
                    saveTemp = getSrlFloat();
                    eep->setSensVss(saveTemp);
                    srlOutputs = F("VSS correction ");
                    srlOutputs += saveTemp;
                }
                if (srlStrName == "cor_dst") {
                    // Total work distance
                    saveTemp = getSrlFloat();
                    eep->setSensDst(saveTemp);
                    srlOutputs = F("DST correction ");
                    srlOutputs += saveTemp;
                }
                if (srlStrName == "cor_ecu") {
                    // Total work distance
                    saveTemp = getSrlFloat();
                    eep->setSensEcu(saveTemp);
                    srlOutputs = F("ECU correction ");
                    srlOutputs += saveTemp;
                }
                if (srlStrName == "set_cor") {
                    // Saves type
                    saveTemp = getSrlInt();
                    if (saveTemp == 1) {
                        car->setSave(eep->getData());
                        srlOutputs = F("Passing value to carSens ");
                    }
                }
                if (srlStrName == "set_wrk") {
                    // Saves type
                    saveTemp = getSrlInt();
                    if (saveTemp == 1) {
                        eep->saveResetData();
                        srlOutputs = F("Saved Work distance ");
                        srlOutputs += saveTemp;
                    }
                }
                if (srlStrName == "save") {
                    // Saves type
                    saveTemp = getSrlInt();
                    if (saveTemp == 1) {
                        eep->saveCurrentData();
                        srlOutputs = F("Saved All data ");
                        srlOutputs += saveTemp;
                    }

                }
                if (srlStrName == "reset") {
                    // Saves type
                    saveTemp = getSrlInt();
                    if (saveTemp == 1) {
                        eep->saveResetData();
                        srlOutputs = F("Saved All data and resetting...");
                        srlOutputs += saveTemp;
                    }

                }

                // ************************************************************
                // Steering buttons digPod inject
                if (srlStrName == "whl") {
                    // Saves type
                    uint8_t vol = (uint8_t) getSrlInt2();
                    saveTemp = getSrlInt();
                    srlOutputs = F("Wheel Simulate resistance ");
                    whl->sendRadioButtons((uint8_t) saveTemp, vol);
                    srlOutputs += saveTemp;
                }

                // ************************************************************
                // Trips record
                //
                // Trip A
                if (srlStrName == "tf1") {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip A fuel ");
                    savedData.trip_a.fuel = saveTemp;
                    srlOutputs += saveTemp;
                }
                if (srlStrName == "td1") {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip A range ");
                    savedData.trip_a.range = saveTemp;
                    srlOutputs += saveTemp;
                }
                //
                // Trip B
                if (srlStrName == "tf2") {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip B fuel ");
                    savedData.trip_b.fuel = saveTemp;
                    srlOutputs += saveTemp;
                }
                if (srlStrName == "td2") {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip B range ");
                    savedData.trip_b.range = saveTemp;
                    srlOutputs += saveTemp;
                }
                //
                // Trip C
                if (srlStrName == "tf3") {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip C fuel ");
                    savedData.trip_c.fuel = saveTemp;
                    srlOutputs += saveTemp;
                }
                if (srlStrName == "td3") {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip C range ");
                    savedData.trip_c.range = saveTemp;
                    srlOutputs += saveTemp;
                }

                //
                // Return back changes
                eep->setData(savedData);

                //
                // Show command information to human
                Serial.print(F("\n\n==============================================================\n"));
                Serial.print(F("\n \t\t  [MID $]> Affected value of "));
                Serial.print(srlOutputs);
                Serial.print(F("\n\n\n==============================================================\n\n"));

            }
        }
    }
};

#endif //ARDUINOMID_DBGSERIAL_H
