
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
 */

//#define LPG_TIME_SENS           // Uncomment to see pulses width in microseconds

//
// Basic settings
#ifndef LPG_INPUT
#define LPG_INPUT 15            // 15/17 Input pin
#endif
#ifndef LPG_TIMEOUT
#define LPG_TIMEOUT  10000     // Timeout between data signals
#endif
#ifndef LPG_BITS
#define LPG_BITS 14             // Bits to count
#endif
#ifndef LPG_BYTE_B0_MIN
#define LPG_BYTE_B0_MIN 1900  // Minimum starting time for HIGH state (1 bit)
#endif
#ifndef LPG_BYTE_B0_MAX
#define LPG_BYTE_B0_MAX 2200  // Minimum starting time for HIGH state (1 bit)
#endif
#ifndef LPG_BYTE_B1_MIN
#define LPG_BYTE_B1_MIN 3900  // Maximum starting time for HIGH state (1 bit)
#endif
#ifndef LPG_BYTE_B1_MAX
#define LPG_BYTE_B1_MAX 4200  // Maximum starting time for HIGH state (1 bit)
#endif
#ifndef LPG_BAUD_RATE
#define LPG_BAUD_RATE 180       // Baud rate of serial2
#endif

volatile unsigned long lpgPulseTime;
volatile uint16_t lpgDataBuffer;
volatile uint8_t lpgDataOffset = 0;


//
// Reads signals from communication
void echoLpgISR() {
    static unsigned long startTime;

    if (digitalRead(LPG_INPUT)) // Gone HIGH
        startTime = micros();
    else {  // Gone LOW
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

        if (lpgPulseTime > LPG_BYTE_B1_MIN && lpgPulseTime < LPG_BYTE_B1_MAX) {
            lpgDataBuffer |= 1 << lpgDataOffset;
            lpgDataOffset++;
        } else if (lpgPulseTime > LPG_BYTE_B0_MIN && lpgPulseTime <= LPG_BYTE_B0_MAX) {
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
            verify();
        }

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

//
// Output
void setup() {
    Serial.begin(115200);
    lpg.begin();
}


unsigned long loopCounter;

void loop() {

    lpg.listener();

    if (loopCounter % 1000 == 0) {
        Serial.print(" Loop offset ");
        Serial.print(loopCounter);
        if (lpg.isAvailable()) {

            Serial.print(" / ");
            Serial.print(lpg.getData(0), BIN);
            Serial.print(" : ");
            Serial.print(lpg.getData(1), BIN);
            Serial.print(" && bit 4 is: ");
            Serial.println(bitRead(lpg.getData(1), 4));

#ifdef LPG_TIME_SENS
            for (int i = 0; i < 14; ++i) {
                Serial.print(" / ");
                Serial.print(dumper[i]);
                dumper[i] = '\0';
            }
#endif


        }


        Serial.println();
    }
    loopCounter++;

}


