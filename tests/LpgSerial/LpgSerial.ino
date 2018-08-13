
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
#define LPG_BITS 13             // Bits to count
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


class LpgSens {


private:


public:


    void begin() {
//        Serial2.begin(1250);

    }

    void listener() {

    }
};

LpgSens lpg;


volatile unsigned long LpgPulseTime;
volatile uint8_t lpgDataOffset = 0;

uint32_t lpgDataBuffer;
volatile unsigned long dumper[14];

void echoPinISR() {
    static unsigned long startTime;

    if (digitalRead(LPG_INPUT)) // Gone HIGH
        startTime = micros();
    else {  // Gone LOW


        LpgPulseTime = micros() - startTime;
        if (LpgPulseTime > LPG_TIMEOUT) {
            lpgDataBuffer = '\0';
            lpgDataOffset = 0;
            LpgPulseTime = 0;
            return;
        }
//        lpgDataOffset++;
        if (lpgDataOffset > 13) {
            lpgDataBuffer = '\0';
            lpgDataOffset = 0;
        }

        if (LpgPulseTime > LPG_BYTE_B1_MIN && LpgPulseTime < LPG_BYTE_B1_MAX) {
            lpgDataBuffer |= 1 << lpgDataOffset;
            lpgDataOffset++;
        } else if (LpgPulseTime > LPG_BYTE_B0_MIN && LpgPulseTime <= LPG_BYTE_B0_MAX) {
            lpgDataBuffer |= 0 << lpgDataOffset;
            lpgDataOffset++;
        }

//        dumper[lpgDataOffset] = micros() - startTime;
//        lpgDataOffset++;
//        if (lpgDataOffset > 14) {
//            lpgDataOffset = 0;
//        }
    }


}


//
// Output
void setup() {
    Serial.begin(115200);
//    lpg.begin();
//    attachInterrupt(digitalPinToInterrupt(18), echoPinISR, CHANGE);
    enableInterrupt(LPG_INPUT, echoPinISR, CHANGE);
}


unsigned long loopCounter;

void loop() {

    lpg.listener();

    if (loopCounter % 500 == 0) {
        Serial.print(" Loop offset ");
        Serial.print(loopCounter);
        if (LpgPulseTime) {
            Serial.print(" / ");
            Serial.print(LpgPulseTime);
            Serial.print(" / ");
            Serial.print(lpgDataBuffer, BIN);

//            for (int i = 0; i < 14; ++i) {
//                Serial.print(" / ");
//                Serial.print(dumper[i]);
//                dumper[i] = '\0';
//            }

            Serial.print(" && bit 13 is: ");
            Serial.println(bitRead(lpgDataBuffer, 13));
        }


        Serial.println();
    }
    loopCounter++;

}


