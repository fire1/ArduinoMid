
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
 */

//#define LPG_TIME_SENS           // Uncomment to see pulses width in microseconds

//
// Basic settings
#ifndef PRK_INPUT
#define PRK_INPUT 15            // 15/17 Input pin
#endif
#ifndef PRK_TIMEOUT
#define PRK_TIMEOUT  1500     // Timeout between data signals in micros
#endif
#ifndef PRK_BITS
#define PRK_BITS 8             // Bits to capture
#endif
#ifndef PRK_BYTE_TS
#define PRK_BYTE_TS 20         // Tolerance micro time +/-
#endif
#ifndef PRK_BYTE_B0
#define PRK_BYTE_B0 200        // HIGH state length for 0 bit
#endif
#ifndef PRK_BYTE_B1
#define PRK_BYTE_B1 100        //  HIGH state length 1 bit
#endif
#ifndef PRK_PIN_STATE
#define PRK_PIN_STATE HIGH       //  Pin state to capture
#endif

#ifndef PRK_DATA_OFFSET
#define PRK_DATA_OFFSET 6       //  Pin state to capture
#endif

volatile unsigned long lpgPulseTime;
volatile uint16_t lpgDataBuffer;
volatile uint8_t lpgDataOffset = 0;


//
// Reads signals from communication
void echoLpgISR() {
    static unsigned long startTime;

    if (digitalRead(PRK_INPUT) == PRK_PIN_STATE) // Listen the state
        startTime = millis();
    else {  // Change state
        lpgPulseTime = millis() - startTime;

        if (lpgPulseTime >= PRK_TIMEOUT) {
            lpgDataBuffer = '\0';
            lpgDataOffset = 0;
            lpgPulseTime = 0;
            return;
        }

        if (lpgDataOffset >= PRK_BITS) {
            lpgDataBuffer = '\0';
            lpgDataOffset = 0;
        }

        if (lpgPulseTime > (PRK_BYTE_B1 - PRK_BYTE_TS) && lpgPulseTime <= (PRK_BYTE_B1 + PRK_BYTE_TS)) {
            lpgDataBuffer |= 1 << lpgDataOffset;
            lpgDataOffset++;
        } else if (lpgPulseTime > (PRK_BYTE_B0 - PRK_BYTE_TS) && lpgPulseTime <= (PRK_BYTE_B0 + PRK_BYTE_TS)) {
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
    uint16_t dataContainer[PRK_DATA_OFFSET];

    void saveData() {
        if (dataOffset > 1) {
            dataOffset = 0;
            isReceive = true;
        }
        if (lpgDataOffset >= PRK_BITS && lpgDataBuffer > 0) {
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
        enableInterrupt(PRK_INPUT, echoLpgISR, CHANGE);

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

LpgSens lpg;

void EngSens_catchRpmHits() {}

//
// Output
void setup() {
    Serial.begin(115200);
    lpg.begin();
    Serial.print("Start listening");
}


unsigned long loopCounter;

void loop() {

    lpg.listener();

//    if (loopCounter % 1000 == 0) {
    if (lpg.isAvailable()) {
        for (uint8_t i = 0; i < PRK_DATA_OFFSET; i++) {
            Serial.print(" / ");
            Serial.print(i);
            Serial.print(": ");
            Serial.print(lpg.getData(i), BIN);
        }
        Serial.println();
        lpg.setReceived();

#ifdef LPG_TIME_SENS
        for (int i = 0; i < 14; ++i) {
            Serial.print(" / ");
            Serial.print(dumper[i]);
            dumper[i] = '\0';
        }
#endif

        Serial.println();
//        }
    }
//    loopCounter++;

}


