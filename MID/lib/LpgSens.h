//
// Created by Angel Zaprianov on 2018-09-25.
//

#ifndef ARDUINOMID_LPGSENS_H
#define ARDUINOMID_LPGSENS_H
#include <Arduino.h>
#include <EnableInterrupt.h>

/**
 *  ArduinoMid tutorial to providing LPG switch signal.
 * In order to capture signals from LPG ECU to digital"Switch Unit"
 * check 4 wires for voltages with multimeter.
 * Two of them needs to provide power supply for switch,
 * other two needs to  provide data and button press sensing.
 *
 * Data signal wire will have some voltage drop over time cycle (max voltage needs to be 5V),
 * when you find this wire run this script with enabled (uncomment) <LPG_TIME_SENS> to
 * determinate data pulse time size.
 *
 * Pulse sizes need to be two types (one for logic "1" and one for logic "0").
 * Define size only for "logic 1" pulse in maximum tolerance.
 *
 *  Example:
 *
        0
        0
        3982 // logic 1
        1993 // logic 0
        1993
        1993
        1994
        3978 // logic 1
        1993
        1993
        3978
        1994
        1993
        1995
        3979 // logic 1
        3978 // logic 1
        0
        0
        0
        ...
 *
 *     Since logic 0 have maximum pulse time of ~2000
 *     for logic 1 pulse time will be from 3500 to 4500
 *
 *
 *     Test result from europe gas
 *
 *      Fuel level one dot
 *      10011000100001 - LPG is disabled (BNZ only)
 *      10010100100001 - LPG is active and wait to start
 *      10010000100001 - Engine is running on LPG
 *      10001000100001 - LPG consuming
 *
 *
 */

//#define LPG_TIME_SENS           // Uncomment to see pulses width in microseconds

//
// Basic settings
#ifndef LPG_INPUT
#define LPG_INPUT 15            // 15/17 Input pin
#endif
#ifndef LPG_TIMEOUT
#define LPG_TIMEOUT  5000     // Timeout between data signals in micros (last 10000)
#endif
#ifndef LPG_BITS
#define LPG_BITS 14             // Bits to capture
#endif
#ifndef LPG_BYTE_TS
#define LPG_BYTE_TS 100         // Tolerance micro time +/-
#endif
#ifndef LPG_BYTE_B0
#define LPG_BYTE_B0 2000        // HIGH state length for 0 bit
#endif
#ifndef LPG_BYTE_B1
#define LPG_BYTE_B1 4000        //  HIGH state length 1 bit
#endif
#ifndef LPG_PIN_STATE
#define LPG_PIN_STATE HIGH       //  Pin state to capture
#endif


volatile unsigned long lpgPulseTime;
volatile uint16_t lpgDataBuffer;
volatile uint8_t lpgDataOffset = 0;


//
// Reads signals from communication
void echoLpgISR() {
    static unsigned long startTime;

    if (digitalRead(LPG_INPUT) == LPG_PIN_STATE) // Listen the state
        startTime = micros();
    else {  // Change state
        lpgPulseTime = micros() - startTime;

        if (lpgPulseTime >= LPG_TIMEOUT) {
            lpgDataBuffer = '\0';
            lpgDataOffset = 0;
            lpgPulseTime = 0;
            return;
        }

        if (lpgDataOffset >= LPG_BITS) {
            lpgDataBuffer = '\0';
            lpgDataOffset = 0;
        }

        if (lpgPulseTime > (LPG_BYTE_B1 - LPG_BYTE_TS) && lpgPulseTime <= (LPG_BYTE_B1 + LPG_BYTE_TS)) {
            lpgDataBuffer |= 1 << lpgDataOffset;
            lpgDataOffset++;
        } else if (lpgPulseTime > (LPG_BYTE_B0 - LPG_BYTE_TS) && lpgPulseTime <= (LPG_BYTE_B0 + LPG_BYTE_TS)) {
            lpgDataBuffer |= 0 << lpgDataOffset;
            lpgDataOffset++;
        }

#ifdef LPG_TIME_SENS
        dumper[lpgDataOffset] = lpgPulseTime;
        lpgDataOffset++;
        if (lpgDataOffset >= 14) {
            lpgDataOffset = 0;
        }

#endif
    }
}


class LpgSens {


private:
    boolean isReceive = false;
    uint8_t dataOffset = 0;
    uint16_t dataContainer[2];

    void saveData() {
        if (dataOffset > 1) {
            dataOffset = 0;
            isReceive = true;
        }
        if (lpgDataOffset >= LPG_BITS && lpgDataBuffer > 0) {
            dataContainer[dataOffset] = lpgDataBuffer;
            dataOffset++;
            lpgDataBuffer = '\0'; // clear used buffer

        }
    }

    boolean isOk() {
        return bitRead(getData(0), 13) && bitRead(getData(1), 13) && bitRead(getData(0), 0) && bitRead(getData(1), 0);
    }

    void verify() {
        if (!isOk()) {
            lpgDataOffset = 0;
            lpgDataBuffer = 0;
            isReceive = false;
        }
    }


public:
    void begin() {
        enableInterrupt(LPG_INPUT, echoLpgISR, CHANGE);

    }

    boolean isAvailable() {
        return isReceive;
    }

    void listener() {
        //
        // Save captured data
        saveData();

        if (isAvailable()) {
//            verify();
        }

    }

    void setReceived() {
        isReceive = false;
    }

/**
 *
 * @param offset
 * @return
 */
    uint16_t getData(uint8_t offset) {
        return dataContainer[offset];
    }

};

#endif //ARDUINOMID_LPGSENS_H
