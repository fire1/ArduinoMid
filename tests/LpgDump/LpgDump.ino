
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
 * Define size only for "logic 1" pulse with maximum tolerance.
 *
 * This program will help to resolve pulse length.
 * Uncomment <LPG_GROUP_SENS> in order to group pulses.
 *
 */

//#define LPG_GROUP_SENS           // Uncomment to see pulses width in microseconds

//
// Basic settings

#define LPG_INPUT 15            // 15/17 Input pin
#define LPG_TIMEOUT  10000      // Timeout between data signals
#define LPG_BITS 14             // Bits to count (pulses to listen)
#define LPG_PIN_STATE HIGH      //  Pin state to capture


volatile unsigned long dumper[LPG_BITS];
volatile unsigned long lpgPulseTime;
volatile uint16_t lpgDataBuffer;
volatile uint8_t lpgDataOffset = 0;


//
// Reads signals from communication
void echoLpgISR() {
    static unsigned long startTime;

    if (digitalRead(LPG_INPUT) == LPG_PIN_STATE)
        startTime = micros();
    else {
        lpgPulseTime = micros() - startTime;
#ifdef LPG_GROUP_SENS

        if (lpgPulseTime > LPG_TIMEOUT) {
            lpgDataOffset = 0;
            for (int i = 0; i < LPG_BITS; ++i) {
                dumper[i] = '\0';
            }
            return;
        }
        if (lpgPulseTime > 0) {
            dumper[lpgDataOffset] = lpgPulseTime;
            lpgDataOffset++;
            if (lpgDataOffset >= 14) {
                lpgDataOffset = 0;
            }
        }

#else
        Serial.println(lpgPulseTime);
#endif
    }

}


//
// Output
void setup() {
    Serial.begin(115200);
    enableInterrupt(LPG_INPUT, echoLpgISR, CHANGE);
}

unsigned long loopCounter;

void loop() {
    if (loopCounter % 1000 == 0) {
        Serial.print(" Loop offset ");
        Serial.print(loopCounter);
        if (lpgDataOffset > 0) {

#ifdef LPG_GROUP_SENS
            for (int i = 0; i < LPG_BITS; ++i) {
                Serial.print(" / ");
                Serial.print(dumper[i]);
            }
#else

#endif
        }
        Serial.println();
    }
    loopCounter++;

}


