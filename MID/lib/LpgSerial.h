//
// Created by Angel Zaprianov on 25.7.2017 г..
//

#ifndef ARDUINO_MID_LPG_SERIAL_H
#define ARDUINO_MID_LPG_SERIAL_H

#include <Arduino.h>
#include "../MID.h"
#include "CmdSerial.h"
#include "../glob.h"
#include "InitObj.h"
//
// Marker for start transmitting
#ifndef LPG_SERIAL_T_ST
#define LPG_SERIAL_T_ST 140
#endif
//
// Switching to default fuel
#ifndef LPG_SERIAL_T_FA
#define LPG_SERIAL_T_FA 218 // 219
#endif
//
// Switching to additional fuel (LPG)
#ifndef LPG_SERIAL_T_FBA
#define LPG_SERIAL_T_FBA 15
#endif
//
// Switching to additional fuel B
#ifndef LPG_SERIAL_T_FBB
#define LPG_SERIAL_T_FBB 100
#endif


#if defined(ADT_FUEL_SYSTEM_SERIAL)


#define LPG_EVENT
#ifdef LPG_EVENT
boolean serial2Low = false;
boolean Serial2High = true;
uint8_t serial2Offset;
uint16_t serial2Length;
unsigned long serial2Timing;
//char dataBuff[] = {};
uint32_t dataBuff = 0;


#endif


byte serial2DataBuffer[16] = {};
boolean serial2EventStart = false;
//
//void serialEvent2() {
//
//    while (Serial2.available()) {
//        // get the new byte:
//        Serial2.readBytes(serial2DataBuffer, 15);
//    }
//    serial2EventStart = true;
//}

//
// All buttons up - 18
class LpgSerial : public LpgFuel {

    AmpTime *amp;
    CarSens *car;
private:
    boolean lpgUse = false;
    uint8_t fuelTankAverage = 0;
    uint8_t capture = 0;
    uint8_t trans;
    uint8_t history;
    uint8_t index = 0;

    uint8_t dinamic = 0;
    uint16_t fuelTankIndex = 0;
    uint32_t fuelTankCollector = 0;

    uint8_t data[2] = {};

    //
    // For V2
    boolean transferStart = false;
    uint8_t buffer, counter;
    unsigned long lastHigh;

private:

    void serialEvent2_test() {


        if (digitalRead(17) == LOW) {
            serial2Low = true;
        }

        if (digitalRead(17) == HIGH && serial2Low && !Serial2High) {
            serial2Timing = millis();
            Serial2High = true;
            serial2Low = false;
        }

        if (digitalRead(17) == LOW && Serial2High) {
            serial2Length = (uint8_t) (millis() - serial2Timing); //if it is low, end the time
            Serial2High = false;
            serial2Low = false;

            if (serial2Length <= 3) {
                dataBuff |= 0 << serial2Offset;
//            dataBuff[serial2Index] = 0;
                serial2Offset++;
            }

            if (serial2Length > 3 && serial2Length <= 5) {
                dataBuff |= 1 << serial2Offset;
//            dataBuff[serial2Index] = 1;
                serial2Offset++;
            }


        }
        // LPG
        // 2221 2201
        // 101000000001 101000000001

        // BNZ
        // 1501 1521 C01 1C00 201
        // 110000000001

        if (serial2Length > 5 && dataBuff > 0) {
            dataBuff = 0;
            serial2Offset = 0;


        }


        if (cmd(amp, DBG_SR_LPG)) {
            dump("LPG inuse 0", data[0]);
            dump("LPG inuse 1", data[1]);
            dump("LPG trans  ", trans);
            dump_bin("LPG  binary", dataBuff);
        }


    }

    void setTrans(uint8_t val) {
        /*
         99 = 18 - almost empty BNZ
         20 / 27 / 18 - bnz
         108 - switch
         99 = 100 switched
         148 - bnz
         100 - lpg
         99 = 219 full lpg
         99 = 18 full lpg LPG

         99 == 20 switch to lpg
         99 = 218 switch to bnz
         */
        if (val >= 34 && val < 255 && val != 99 && val != 98 && val != 100) {
            capture = history;
            trans = val;
        }

        //
        // Fixture 1
//        if(val > 17 && val < 28){
//            trans = 148;
//        }
    }

public:
    LpgSerial(AmpTime &ampTime, CarSens &carSens) : amp(&ampTime), car(&carSens) {

    }

    void begin(void) {
        //
        // Original
        Serial2.begin(246);//246

//        Serial2.begin(400); // newer version


// pin 15
        //
        //
//        Serial3.begin(500); // test 550
//        digitalWrite(15, LOW);

    }

/*
 * BNZ switch only
 BIN 1010101110101011101011011111111100000000000 active lpg
 BIN 1010101110101011101010111111111100000000000 disabled
 serial2DataBuffer[22]


 */
    void listener_new() {
        if (serial2EventStart) {
            //
            // 173 bnz
            //
            //
            Serial.print(F("BIN "));
            Serial.println(serial2DataBuffer[2]);
            Serial.println();

            for (uint8_t i = 0; i < 15; ++i) {
                Serial.println(serial2DataBuffer[i]);
                serial2DataBuffer[i] = 0;
            }
            Serial.println();


            serial2EventStart = false;
        }
    }


//#define DEBUG_SR2


    void listener_() {

        if (digitalRead(17) == LOW && !transferStart) {
            transferStart = true;
        }


    }


    void listener(void) {

        serialEvent2_test();

// Button serial
        // TODO test for clock by digital low
//        while (Serial3.available()) {
//            Serial.println();
//            Serial.print("Serial 3: ");
//            Serial.println(Serial3.read());
//            Serial.println();
//        }


//        if (!compare) {
//            if (amp->is2Seconds())
//                if (car->getEngTmp() > 79) {
//                    trans = 50;
//                    compare = true;
//                    return;
//                }
//        } else return;



        // 30bits / 400bit rate
        // Arduhdlc library

        //
        // Do
        if (Serial2.available() > 0 && !amp->isSens()) {


            if (trans != LPG_SERIAL_T_ST) {
                history = trans;
                //
                // Calculate averages
                if (fuelTankIndex > 10) {
                    fuelTankAverage = fuelTankCollector / fuelTankIndex;
                    fuelTankCollector = 0;
                    fuelTankIndex = 0;
                }

            }
            uint8_t val = (uint8_t) Serial2.read();

            if (val == 0 && lpgUse) {
                val = 100;
            } else if (val == 0 && !lpgUse) {
                val = 20;
            }

            if (val != 255 && val != 99) {
                data[index] = val;
                index++;
            }


            // 100 lpg run
            // 20 bnz
            // 18 stand by mode

            //from 19 to 18 - lpg


            // 19 lpg ??
            // 108 lpg
            // 236 lpg
            captureLpg(108); // 18 full
            captureLpg(100); // 18 full
//            captureLpg(18); // 219 none // at free run
            captureLpg(34); // 218 2 dots
            captureLpg(47); // 140 1 dots
            captureLpg(19); // 219 -idle / 218-bnz /  4 dots
            // 108,34 as lpg

            if (dinamic != 0) {
                captureLpg(dinamic);
            }

            // 219 none
            // 18 none
            // 20 conflict full lpg

//            captureBnz(18);
//            captureBnz(20);
            captureBnz(218);
            captureBnz(140);// BNZ 1 dot
            captureBnz(155);// BNZ on the run


            // Skip action
//            if (data[0] == 100 || data[1] == 100 || history == 100) {
//                capture = trans;
//                if (!lpgUse) { // checks for opposite
//                    car->passMelodyClass()->play(1);
//                }
//                lpgUse = true;
//            }

//            if (data[0] == 18 || data[1] == 18 || history == 18) {
//                capture = trans;
//                if (!lpgUse) { // checks for opposite
//                    car->passMelodyClass()->play(1);
//                }
//                lpgUse = true;
//            }

            //
            // 218
            if (data[0] == 20 && history == 20 || data[1] == 20 && history == 20) {
                if (lpgUse) { // checks for opposite
                    car->passMelodyClass()->play(6);
                }
                lpgUse = false;
            }

            history = trans;
            if (val != 255)
                trans = val;

#ifdef DEBUG
            if (cmd(amp, DBG_SR_LPG)) {
                dump("Data 0", data[0]);
                dump("Data 1", data[1]);
                dump("Trans ", trans);
            }
#endif

            if (index >= 2) {
                index = 0;
                data[0] = 0;
                data[1] = 0;
            }


            //
            // Agg to average
//            fuelTankCollector = fuelTankCollector + trans;
//            fuelTankIndex++;
//
//
//            if (fuelTankAverage == 0) {
//                fuelTankAverage = trans;
//            }

        }


    }

/**
 *
 * @param value
 */
    void captureLpg(uint8_t value) {
        if (data[0] == value || data[1] == value || history == value) {
            capture = trans;
            if (!lpgUse) { // checks for opposite
                car->passMelodyClass()->play(1);
            }
            lpgUse = true;
        }
    }

/**
 *
 * @param value
 */
    void captureBnz(uint8_t value) {
        if (data[0] == value || data[1] == value || history == value) {
            capture = trans;
            if (lpgUse) { // checks for opposite
                car->passMelodyClass()->play(6);
            }
            lpgUse = false;
        }
    }

/**
 * Adds dynamic data to switch
 */
    void setDynamic() {
        dinamic = trans;
    }

    /**
     *
     */
    uint8_t getCurrentValue() override {
        return trans;
    }

    /**
     * Gets fuel tank level
     */
    uint8_t getFuelTankLiters() override {

        if (fuelTankAverage > 140) {
            return (uint8_t) map(fuelTankAverage, 215, 145, 65, 15);
        }
        return (uint8_t) map(fuelTankAverage, 75, 15, 5, 30);
    }

/**
 *  Is additional fuel active
 */
    boolean isLPG() override {
//        return (history < 140 && history > 27 || trans < 140 && trans > 27 || lpg == 1) ? true : false;

        return lpgUse;
    }

/**
 *  Is default fuel active
 */
    inline boolean isBNZ() override {
//        return (trans > 140 && trans == history || trans == 27 || stateStart == false || lpg == 2) ? true : false;
        return (!lpgUse) ? true : false;
    }

};

#elif defined(LPG_SWTC_PIN)


class LpgSwitch : public LpgFuel {

    AmpTime *amp;
    boolean state = false;

public:
    LpgSwitch(AmpTime &ampTime) : amp(&ampTime) {

    }

    void begin(void){
        pinMode(LPG_SWTC_PIN, INPUT);
        digitalWrite(LPG_SWTC_PIN, LOW);
    }

    void listener() {
        if (amp->isSecond()) {
            state = (digitalRead(LPG_SWTC_PIN) == LPG_SWTC_STT) ? true : false;
        }
    }

    uint8_t getCurrentValue() {
        return 0;
    };

    virtual uint8_t getFuelTankLiters() {
        return 0;
    };

    boolean isLPG() {
        return state;
    }

    inline boolean isBNZ() {
        return !state;
    }

};

#endif

#endif //ARDUINOMID_LPGSERIAL_H
