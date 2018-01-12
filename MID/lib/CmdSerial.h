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

/**
 * Command Serial Prompt
 */
class CmdSerial {


    CarSens *car;
    EepRom *eep;
    WhlSens *whl;
    float saveTemp;
    String srlStrName;
    String srlOutputs;


    typedef void (*cb_svd)(float, SavedData &);

    typedef void (*cb_eep)(float, EepRom *);

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

    char *string2char(String command) {
        if (command.length() != 0) {
            char *p = const_cast<char *>(command.c_str());
            return p;
        }
    }


    void cmd_eep(const __FlashStringHelper *cmd, cb_eep _call, float value, EepRom *eep,
                 const __FlashStringHelper *inf = 0) {
        if (srlStrName == cmd) {
            saveTemp = value;
            (*_call)(value, eep);
            srlOutputs = F("[MID$]> EEP CMD <");
            srlOutputs += cmd;
            srlOutputs += F("> VAL <");
            srlOutputs += saveTemp;
            srlOutputs += F("> ");
            srlOutputs += inf;
            srlOutputs += F(";");
        }
    }


    void cmd_svd(const __FlashStringHelper *cmd, cb_svd cm_callback, SavedData &save,
                 const __FlashStringHelper *inf = 0) {
        if (srlStrName == cmd) {
            saveTemp = getSrlFloat();
            (*cm_callback)(saveTemp, save);

            srlOutputs = F("[MID$]> SVD CMD <");
            srlOutputs += cmd;
            srlOutputs += F("> \t VAL <");
            srlOutputs += saveTemp;
            srlOutputs += F("> ");
            srlOutputs += inf;
            srlOutputs += F(";");
        }
    }


public:
    CmdSerial(CarSens &carSens, EepRom &eepRom, WhlSens &whlSens) : car(&carSens), eep(&eepRom), whl(&whlSens) {

    }


    void listener(void) {
        //
// Serial injection

        String srlAllData;


        //
        // Default message
        srlOutputs = F(" None ... Sorry! ");
        //
        // Execute command from serial
        if (Serial.available() > 0) {


            srlStrName = Serial.readStringUntil('=');


            if (srlStrName.length() > 2) {
                SavedData savedData = eep->getData();

                // ************************************************************
                // Correction values

                if (srlStrName == F("lpg")) {
                    // Total Liters per hour consumed
                    saveTemp = getSrlFloat();
                    eep->setAdtFuel(saveTemp);
                    srlOutputs = F("LPG fuel ");
                    srlOutputs += saveTemp;
                }
//                cmd_eep(F("lpg"), [](float value, EepRom *e) { e->setAdtFuel(value); }, getSrlFloat(), eep);


                if (srlStrName == F("bnz")) {
                    // Total Liters per hour consumed
                    saveTemp = getSrlFloat();
                    eep->setDefFuel(saveTemp);
                    srlOutputs = F("BNZ fuel ");
                    srlOutputs += saveTemp;
                }
//                cmd_eep(F("bnz"), [](float value, EepRom *e) { e->setDefFuel(value); }, getSrlFloat(), eep);

                if (srlStrName == "ttd") {
                    // Total Travel distance
                    saveTemp = getSrlFloat();
                    eep->setTravelDistance(saveTemp);
                    srlOutputs = F("Travel distance ");
                    srlOutputs += saveTemp;
                }
//                cmd_eep(F("ttd"), [](float value, EepRom *e) { e->setTravelDistance(value); }, getSrlFloat(), eep);


                if (srlStrName == "wrk") {
                    // Total work distance
                    saveTemp = getSrlFloat();
                    eep->setWorkDistance(saveTemp);
                    srlOutputs = F("Work distance ");
                    srlOutputs += saveTemp;
                }
//                cmd_eep(F("ttd"), [](float value, EepRom *e) { e->setWorkDistance(value); }, getSrlFloat(), eep);

                // ************************************************************
                // Correction value inject
//                if (srlStrName == "cor_rpm") {
//                    // Total work distance
//                    saveTemp = getSrlFloat();
//                    eep->setSensRpm(saveTemp);
//                    srlOutputs = F("RPM correction ");
//                    srlOutputs += saveTemp;
//                }
//                cmd_eep(F("cor_rpm"), [](float value, EepRom *e) { e->setSensRpm(value); }, getSrlFloat(), eep);

                if (srlStrName == "cor_vss") {
                    // Total work distance
                    saveTemp = getSrlFloat();
                    eep->setSensVss(saveTemp);
                    srlOutputs = F("VSS correction ");
                    srlOutputs += saveTemp;
                }
//                cmd_eep(F("cor_vss"), [](float value, EepRom *e) { e->setSensVss(value); }, getSrlFloat(), eep);

                if (srlStrName == "cor_dst") {
                    // Total work distance
                    saveTemp = getSrlFloat();
                    eep->setSensDst(saveTemp);
                    srlOutputs = F("DST correction ");
                    srlOutputs += saveTemp;
                }
//                cmd_eep(F("cor_dst"), [](float value, EepRom *e) { e->setSensDst(value); }, getSrlFloat(), eep);

                if (srlStrName == "cor_ecu") {
                    // Total work distance
                    saveTemp = getSrlFloat();
                    eep->setSensEcu(saveTemp);
                    srlOutputs = F("ECU correction ");
                    srlOutputs += saveTemp;
                }
//                cmd_eep(F("cor_ecu"), [](float value, EepRom *e) { e->setSensEcu(value); }, getSrlFloat(), eep);

                if (srlStrName == "set_cor") {
                    // Saves type
                    saveTemp = getSrlInt();
                    if (saveTemp == 1) {
                        car->setSave(eep->getData());
                        srlOutputs = F("Passing value to carSens driver");
                    }
                }
//
                if (srlStrName == "set_wrk") {
                    // Saves type
                    saveTemp = getSrlInt();
                    if (saveTemp == 1) {
                        eep->saveResetData();
                        srlOutputs = F("Saved Work distance ");
                        srlOutputs += saveTemp;
                    }
                }
//                cmd_eep(F("set_wrk"), [](float value, EepRom *e) { if ((uint8_t) value == 1) e->saveResetData(); },
//                        getSrlFloat(), eep, F("SAVED Work distance "));
//
                if (srlStrName == "save") {
                    // Saves type
                    saveTemp = getSrlInt();
                    if (saveTemp == 1) {
                        eep->saveCurrentData();
                        srlOutputs = F("Saved All data ");
                        srlOutputs += saveTemp;
                    }
                }
//                cmd_eep(F("save"), [](float value, EepRom *e) { if ((uint8_t) value == 1) e->saveCurrentData(); },
//                        getSrlFloat(), eep, F("SAVED ALL DATA "));

//
                if (srlStrName == "reset") {
                    // Saves type
                    saveTemp = getSrlInt();
                    if (saveTemp == 1) {
                        eep->saveResetData();
                        srlOutputs = F("Saved All data and resetting...");
                        srlOutputs += saveTemp;
                    }

                }
//
//                cmd_eep(F("reset"), [](float value, EepRom *e) { if ((uint8_t) value == 1) e->saveResetData(); },
//                        getSrlFloat(), eep, F("SAVED-RESET LAST DATA "));

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
                // Saved data ...
                if (srlStrName == "trd") {
                    // trip distance
                    saveTemp = getSrlFloat();
                    savedData.dist_trp = saveTemp;
                    srlOutputs = F("Trip distance ");
                    srlOutputs += saveTemp;
                }
//                cmd_svd(F("trd"), [](float value, SavedData &save) { save.dist_trp = value; }, savedData);

//                if (srlStrName == "trt") {
//                    // Trip time
//                    saveTemp = getSrlFloat();
//                    savedData.time_trp = saveTemp;
//                    srlOutputs = F("Trip time ");
//                    srlOutputs += saveTemp;
//                }
//                cmd_svd(F("trt"), [](float value, SavedData &save) { save.time_trp = value; }, savedData);


                if (srlStrName == "set_tr") {
                    // Saves type
                    saveTemp = getSrlInt();
                    if (saveTemp == 1)eep->saveTripData();
                    srlOutputs = F("SAVED trip data ");
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
//                cmd_svd(F("tf1"), [](float value, SavedData &save) { save.trip_a.fuel = value; }, savedData,
//                        F("TRIP A fuel"));

                if (srlStrName == "td1") {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip A range ");
                    savedData.trip_a.range = saveTemp;
                    srlOutputs += saveTemp;
                }
//                cmd_svd(F("td1"), [](float value, SavedData &save) { save.trip_a.range = value; }, savedData,
//                        F("TRIP A range"));

                //
                // Trip B
                if (srlStrName == "tf2") {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip B fuel ");
                    savedData.trip_b.fuel = saveTemp;
                    srlOutputs += saveTemp;
                }
//                cmd_svd(F("tf2"), [](float value, SavedData &save) { save.trip_b.fuel = value; }, savedData,
//                        F("TRIP B range"));
//
                if (srlStrName == "td2") {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip B range ");
                    savedData.trip_b.range = saveTemp;
                    srlOutputs += saveTemp;
                }
//                cmd_svd(F("td2"), [](float value, SavedData &save) { save.trip_b.range = value; }, savedData,
//                        F("TRIP B range"));

                //
                // Trip C
                if (srlStrName == "tf3") {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip C fuel ");
                    savedData.trip_c.fuel = saveTemp;
                    srlOutputs += saveTemp;
                }
//                cmd_svd(F("tf3"), [](float value, SavedData &save) { save.trip_c.fuel = value; }, savedData,
//                        F("TRIP C fuel"));

                if (srlStrName == "td3") {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip C range ");
                    savedData.trip_c.range = saveTemp;
                    srlOutputs += saveTemp;
                }
//                cmd_svd(F("td3"), [](float value, SavedData &save) { save.trip_c.range = value; }, savedData,
//                        F("TRIP C range"));
                //
                // Return back changes
                eep->setData(savedData);


                if (srlStrName == F("dbg")) {
                    CmdSerialDebugging = Serial.readStringUntil('\n');
                    if (CmdSerialDebugging == F("stop")) {
                        srlOutputs += CmdSerialDebugging;
                        srlOutputs += F("> ");
                        CmdSerialDebugging = "";
                    } else {
                        srlOutputs = "DUMPING string name <";
                        srlOutputs += CmdSerialDebugging;
                        srlOutputs += F("> ");
                    }
                }



                //
                // Show command information to human
                Serial.print(F("\n\n==============================================================\n"));
                Serial.print(srlOutputs);
                Serial.print(F("\n==============================================================\n\n"));

            }
/*
            srlStrName = Serial.readStringUntil('*');
            // ************************************************************
            // Debug partition
            if (srlStrName == F("dbg")) {

#ifdef DEBUG
                String tempo = Serial.readStringUntil('=');
                if (tempo == F("min")) {
//                    if (amp->isMin()) {
//                        //
//                        // Show command information to human
//                        Serial.println(srlOutputs);
//                    }
                }

#else
                Serial.println(F("Debug functionality is disabled! "));
#endif
            }

            */



        }
    }
};

#endif //ARDUINOMID_DBGSERIAL_H
