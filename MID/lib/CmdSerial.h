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
#include "AptService.h"

const char CMD_SPR_EOL = '\n';

/**
 * Command Serial Prompt
 */
class CmdSerial : public EepRom {

    boolean cmdUse = false;
    CarSens *car;
//    EepRom *eep;
    WhlSens *whl;
    float saveTemp;
    String srlStrName;
    String srlOutputs;
    EepRom *eep;
    AptService *rtc;


    typedef void (*cb_svd)(float, SavedData &);

    typedef void (*cb_eep)(float, EepRom *);

private:

    float getSrlFloat() {
        return Serial.readStringUntil(CMD_SPR_EOL).toFloat();
    }

    long getSrlInt() {
        return Serial.readStringUntil(CMD_SPR_EOL).toInt();
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


/**
 *
 * @param cmd
 * @param txt
 * @param cur
 */
    void setOutputHelpCMD(const __FlashStringHelper *cmd, const __FlashStringHelper *txt, float cur) {
        srlOutputs = F("\t ");
        srlOutputs += cmd;
        srlOutputs += F(")\t ");
        srlOutputs += txt;
        srlOutputs += F("\t CURRENT: ");
        srlOutputs += cur;
        srlOutputs += F("\n");
        Serial.println(srlOutputs);
    }

/**
 *
 * @param offset
 * @param txt
 */
    void setOutputHelpDBG(const uint8_t offset, const __FlashStringHelper *txt) {
        srlOutputs = F("\t ");
        srlOutputs += offset;
        srlOutputs += F(")\t ");
        srlOutputs += txt;
        srlOutputs += F("\n");
        Serial.println(srlOutputs);
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
    CmdSerial(CarSens &carSens, WhlSens &whlSens, AptService &rtcService) : car(&carSens), whl(&whlSens),
                                                                            rtc(&rtcService), EepRom(carSens) {
    }


    void listener(void) {
        //
// Serial injection

        String srlAllData;


        //
        // Default message
        srlOutputs = F(" Command not found... Sorry! ");
        //
        // Execute command from serial
        if (Serial.available() > 0) {


            srlStrName = Serial.readStringUntil('=');


            if (srlStrName.length() > 2) {
                boolean commandOutput = true;
                SavedData savedData = getData();


                if (srlStrName == F("help")) {
                    commandOutput = false;
                    Serial.println();
                    Serial.println(F("=============================================================="));
                    Serial.println(F(" HELP / General command information"));
                    Serial.println();
                    Serial.println(F(".:: SET NEW VALUES :::::::::::::::::."));
                    setOutputHelpCMD(F("lpg=<float>"), F("Sets consumed propane-butane  "), savedData.fuel_adt);
                    setOutputHelpCMD(F("bnz=<float>"), F("Sets consumed benzine(gasoline) "), savedData.fuel_def);
                    setOutputHelpCMD(F("ttd=<float>"), F("Sets traveled distance "), savedData.dist_trv);
                    setOutputHelpCMD(F("wrk=<float>"), F("Sets car work distance "), savedData.total_km);
                    setOutputHelpCMD(F("rtc=<float>"), F("Sets clock time as 12.59h "), 0);
                    Serial.println();
                    Serial.println(F(".:: CORRECTION VALUES :::::::."));
                    setOutputHelpCMD(F("cor_vss=<float>"), F("Vehicle speed  measurements"), savedData.sens_vss);
                    setOutputHelpCMD(F("cor_dst=<float>"), F("Distance measurements"), savedData.sens_dst);
                    setOutputHelpCMD(F("cor_ecu=<float>"), F("Consumption measurements "), savedData.sens_ecu);
                    Serial.println();
                    Serial.println(F(".:: OTHER COMMANDS :::::::::."));
                    setOutputHelpCMD(F("set_cor=<1>"), F("Push data to carSens "), 1);
                    setOutputHelpCMD(F("save=<1>"), F("Saves current changed data "), 1);
                    setOutputHelpCMD(F("reset=<1>"), F("Resets trips of BC  "), 1);
                    setOutputHelpCMD(F("whl=<1+1>"), F("Sends voltage+resistance to radio  "), 0);
                    Serial.println();
                    Serial.println(F(".:: TRIPS COMMANDS :::::::::."));
                    setOutputHelpCMD(F("tf1=<float>"), F("Sets consumed fuel for  Trip 1 "), savedData.trip_a.fuel);
                    setOutputHelpCMD(F("td1=<float>"), F("Sets distance travel for  Trip 1 "), savedData.trip_a.fuel);
                    setOutputHelpCMD(F("tf2=<float>"), F("Sets consumed fuel for  Trip 2 "), savedData.trip_b.fuel);
                    setOutputHelpCMD(F("td2=<float>"), F("Sets distance travel for  Trip 2 "), savedData.trip_b.fuel);
                    setOutputHelpCMD(F("tf3=<float>"), F("Sets consumed fuel for  Trip 3 "), savedData.trip_c.fuel);
                    setOutputHelpCMD(F("td3=<float>"), F("Sets distance travel for  Trip 3 "), savedData.trip_c.fuel);
                    Serial.println();
                    Serial.println(F("=============================================================="));
                }

                // ************************************************************
                // Correction values

                if (srlStrName == F("lpg")) {
                    // Total Liters per hour consumed
                    saveTemp = getSrlFloat();
                    savedData.fuel_adt = saveTemp;
                    srlOutputs = F("LPG fuel ");
                    srlOutputs += saveTemp;
                }

                if (srlStrName == F("bnz")) {
                    // Total Liters per hour consumed
                    saveTemp = getSrlFloat();
//                    EepRom::setDefFuel(saveTemp);
                    savedData.fuel_def = saveTemp;
                    srlOutputs = F("BNZ fuel ");
                    srlOutputs += saveTemp;
                }

                if (srlStrName == "ttd") {
                    // Total Travel distance
                    saveTemp = getSrlFloat();
                    savedData.dist_trv = saveTemp;
//                    setTravelDistance()
                    srlOutputs = F("Travel distance ");
                    srlOutputs += saveTemp;
                }

                if (srlStrName == "wrk") {
                    // Total work distance
                    saveTemp = getSrlFloat();
                    savedData.total_km = saveTemp;
                    srlOutputs = F("Work distance ");
                    srlOutputs += saveTemp;
                }

                if (srlStrName == "cor_vss") {
                    // Total work distance
                    saveTemp = getSrlFloat();
                    savedData.sens_vss = saveTemp;
                    srlOutputs = F("VSS correction ");
                    srlOutputs += saveTemp;
                }

                if (srlStrName == "cor_dst") {
                    // Total work distance
                    saveTemp = getSrlFloat();
                    savedData.sens_dst = saveTemp;
                    srlOutputs = F("DST correction ");
                    srlOutputs += saveTemp;
                }

                if (srlStrName == "cor_ecu") {
                    // Total work distance
                    saveTemp = getSrlFloat();
                    savedData.sens_ecu = saveTemp;
                    srlOutputs = F("ECU correction ");
                    srlOutputs += saveTemp;
                }

                if (srlStrName == "set_cor") {
                    // Saves type
                    saveTemp = getSrlInt();
                    if (saveTemp == 1) {
                        car->setSave(this->getData());
                        srlOutputs = F("Passing value to carSens driver");
                    }
                }
//
                if (srlStrName == "set_wrk") {
                    // Saves type
                    saveTemp = getSrlInt();
                    if (saveTemp == 1) {
                        saveResetData();
                        srlOutputs = F("Saved Work distance ");
                        srlOutputs += saveTemp;
                    }
                }

                if (srlStrName == "save") {
                    // Saves type
                    saveTemp = getSrlInt();
                    if (saveTemp == 1) {
                        saveCurrentData();
                        srlOutputs = F("Saved All data ");
                        srlOutputs += saveTemp;
                    }
                }

                if (srlStrName == "reset") {
                    // Saves type
                    saveTemp = getSrlInt();
                    if (saveTemp == 1) {
                        saveResetData();
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
                // Saved data ...
                if (srlStrName == "trd") {
                    // trip distance
                    saveTemp = getSrlFloat();
                    savedData.dist_trp = saveTemp;
                    srlOutputs = F("Trip distance ");
                    srlOutputs += saveTemp;
                }


                if (srlStrName == "set_tr") {
                    // Saves type
                    saveTemp = getSrlInt();
                    if (saveTemp == 1)this->saveTripData();
                    srlOutputs = F("SAVED trip data ");
                    srlOutputs += saveTemp;
                }
                // ************************************************************
                // Trips record
                //
                // Trip A
                if (srlStrName == ("tf1")) {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip A fuel ");
                    savedData.trip_a.fuel = saveTemp;
                    srlOutputs += saveTemp;
                }

                if (srlStrName == ("td1")) {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip A range ");
                    savedData.trip_a.range = saveTemp;
                    srlOutputs += saveTemp;
                }

                //
                // Trip B
                if (srlStrName == ("tf2")) {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip B fuel ");
                    savedData.trip_b.fuel = saveTemp;
                    srlOutputs += saveTemp;
                }
                if (srlStrName == ("td2")) {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip B range ");
                    savedData.trip_b.range = saveTemp;
                    srlOutputs += saveTemp;
                }

                //
                // Trip C
                if (srlStrName == ("tf3")) {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip C fuel ");
                    savedData.trip_c.fuel = saveTemp;
                    srlOutputs += saveTemp;
                }
                if (srlStrName == ("td3")) {
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets Trip C range ");
                    savedData.trip_c.range = saveTemp;
                    srlOutputs += saveTemp;
                }
                //
                // Commands
                if (srlStrName == F("cmd")) {
                    int cmd = (int) Serial.readStringUntil('\n').toInt();
                    if (cmd == 1001) {
                        isAndroidConnected = true;
                    }
                }
                //
                // Clock
                if (srlStrName == F("rtc")) {
                    int clock[1];
                    saveTemp = getSrlFloat();
                    srlOutputs = F("Sets clock to: ");
                    separateFloat(saveTemp, clock);
                    rtc->setClock(clock[0], clock[1]);
                    srlOutputs += rtc->getClock();
                }

                //
                // Return back changes
                setData(savedData);



                //
                // Debug / dump from serial
                if (srlStrName == F("dbg")) {
                    String cmd = Serial.readStringUntil('\n');
                    cmd.trim();

                    if (cmd == F("help") || cmd == F("")) {

                        commandOutput = false;
                        Serial.println();
                        Serial.println(F("=============================================================="));
                        Serial.println(F(" DEBUG HELP / Available dump offsets (dbg=<NUM>) "));
                        Serial.println();
                        setOutputHelpDBG(DBG_SR_VSS, F("VSS Vehicle Speed Sensor "));
                        setOutputHelpDBG(DBG_SR_RPM, F("RPM Revolutions Per Minute"));
                        setOutputHelpDBG(DBG_SR_ECU, F("ECU Engine Control Unit"));
                        setOutputHelpDBG(DBG_SR_TM1, F("Outside temperature (front)"));
                        setOutputHelpDBG(DBG_SR_TM2, F("Additional  temperature (1)"));
                        setOutputHelpDBG(DBG_SR_TM3, F("Additional  temperature (2)"));
                        setOutputHelpDBG(DBG_SR_CNS, F("Fuel consumption calculation "));
                        setOutputHelpDBG(DBG_SR_IFC, F("Instant consumption calculation"));
                        setOutputHelpDBG(DBG_SR_DIM, F("Display back light level"));
                        setOutputHelpDBG(DBG_SR_ENT, F("Engine coolant temperature"));
                        setOutputHelpDBG(DBG_SR_TNK, F("Fuel tank calculation data"));
                        setOutputHelpDBG(DBG_SR_DST, F("Vehicle distance measured"));
                        setOutputHelpDBG(DBG_SR_GRS, F("Vehicle gear ratio resolver"));
                        setOutputHelpDBG(DBG_SR_AVR, F("Vehicle averages assumed"));
                        setOutputHelpDBG(DBG_SR_MNI, F("UI User Interface menu"));
                        setOutputHelpDBG(DBG_SR_MNB, F("UI button handler"));
                        setOutputHelpDBG(DBG_SR_EPR, F("Internal chip memory"));
                        setOutputHelpDBG(DBG_SR_WHL, F("Steering wheel buttons"));
                        setOutputHelpDBG(DBG_SR_RCR, F("Remote control  radio"));
                        setOutputHelpDBG(DBG_SR_STT, F("Vehicle servicing states"));
                        setOutputHelpDBG(DBG_SR_STW, F("Vehicle warning states"));
                        setOutputHelpDBG(DBG_SR_SAL, F("Vehicle speed alarm"));
                        setOutputHelpDBG(DBG_SR_LPG, F("LPG serial communication"));
                        Serial.println();
                        Serial.println(F("=============================================================="));

                    } else if (cmd == F("stop")) {
                        srlOutputs = F("Debugging offset \t <");
                        srlOutputs += CmdSerialDebugging;
                        srlOutputs += F("> stopped! ");
                        CmdSerialDebugging = 0;
                    } else if (cmd.toInt() > 0) {
                        CmdSerialDebugging = (uint8_t) cmd.toInt();
                        srlOutputs = F("Dumping offset name \t <");
                        srlOutputs += CmdSerialDebugging;
                        srlOutputs += F("> started!");
                    }
                }


                if (commandOutput) {
                    cmdUse = true;
                    //
                    // Show command information to human
                    Serial.println();
                    Serial.println(F("=============================================================="));
                    Serial.println(srlOutputs);
                    Serial.println(F("=============================================================="));
                    Serial.println();
                }
            }

            //
            // USB Power charging option
            // Disable serial pins after a minute of non-use
            if (millis() > MILLIS_PER_MN  && !cmdUse) {
                analogWrite(0, 0);
                analogWrite(1, 0);
            }


        }
    }
};

#endif //ARDUINOMID_DBGSERIAL_H
