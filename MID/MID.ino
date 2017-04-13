/*
---------------------------------------------------
    Arduino MID

     Author: Angel Zaprianov <me@fire1.eu>
     Version: 0.1 [pre-alpha]
     Start Date: 09.06.2016

     For the full copyright and license information, please view the LICENSE
     file that was distributed with this source code.
---------------------------------------------------

 MID function menu
*/

//
//
// Maybe Mid ino needs to be separated in two different variants 16x2 && 240x64


#include <Arduino.h>
#include <SPI.h>
#include <OneWire.h>
#include <MenuBackend.h>
#include <DallasTemperature.h>

//
//
#include "conf.h"
#include "lib/InitObj.h"
#include "lib/InitLcd.h"

//
//
//LpgSens lpgSens;
#ifdef ADT_FUEL_SYSTEM_I2C
I2cSimpleListener i2cLpg(pinLpgDat, LPG_CLC_PIN);
#endif
//
// Setup the code...
void setup() {
    //
    // Sets display pins to socket
    lcdPwr.begin();
    //
    // Shutdown setupEngine
    shutDown.setup(SAV_PIN_CTR, SAV_PIN_DTC, TONE_ADT_PIN);
    //
    //
//    lpgSens.setup(pinLpgDat, LPG_CLC_PIN);

    //
    // Debug serial
    Serial.begin(SERIAL_MONITOR_BAUD);
    //
    // Change timer 3
    setupTimer3();
    //
    //
    eepRom.setup();
    //
    // Setup button listener
    btnMenu.setup(BTN_PIN_UP, BTN_PIN_DW, TONE_ADT_PIN);
    //
    // Sets Default Fuel as Benzine (fuel that engine will start) and additional LPG
    carSens.setupFuel({FUEL_BNZ_IFC, FUEL_BNZ_CNS}, {FUEL_LPG_IFC, FUEL_LPG_CNS});
    //
    // Setup fuel gauge two required wires
    carSens.setupAdtFuel(pinLpgDat, LPG_CLC_PIN);
    //
    // consumption
    // Engine / Speed sensors
    carSens.setupEngine(SPD_SNS_PIN, RPM_SNS_PIN, ECU_SGN_PIN, ENG_CLT_PIN);
    //
    // Screen back-light
    carSens.setupScreen(DIM_PIN_VAL, DIM_PIN_OUT);
    //
    // Outside temperature (car stock temp sns)
    carSens.setupTemperature(TMP_PIN_OUT);
    //
    //  Setup car state pins to detect
    carStat.setup(STT_OIL_PIN, STT_CLN_PIN, STT_WNW_PIN, STT_BRK_PIN, STT_VLT_PIN);
    //
    //
    lcdMenu.begin();
    //
    // Set MID menu
    midMenu.setup();
    //
    // Setup SPI lib
    whlSens.setup(ALP_PIN_INP, ALP_PIN_OUT, ALP_PIN_VOL);
    //
    // Restore data
    eepRom.loadCurrentData();
    //
    // Pass saved data to car state for calculation
    carStat.setWorkState(eepRom.getWorkDistance());
    //
    // Setup LPG pins
    pciSetup(LPG_CLC_PIN); //   attachInterrupt (digitalPinToInterrupt (LPG_CLC_PIN), isr, CHANGE);
    pciSetup(pinLpgDat); //   attachInterrupt (digitalPinToInterrupt (pinLpgDat), isr, CHANGE);

    //
    // Shows MID intro
    lcdMenu.intro();

}
//
//
// The interrupt can be enabled for each pin individually (analog and digital!),
// but there are only 3 interrupt vectors, so 6-8 pins share one service routine:
// ISR (PCINT1_vect) pin change interrupt for A0 to A5
//ISR (PCINT1_vect) { // handle pin change interrupt for A0 to A5 here
//
//    // Install Pin change interrupt for a pin, can be called multiple times from pciSetup()
//     Serial.print(F("\n\n\n\n");
//     Serial.print(F(" LPG dat: ");
//    Serial.print(digitalRead(pinLpgDat));
//     Serial.print(F(" LPG clc: ");
//    Serial.print(digitalRead(LPG_CLC_PIN));
//     Serial.print(F("\n\n\n\n");
//}

//#define HEAP_DEBUG

void loop() {
    if (ampInt.isSecond()) {
         Serial.print(F(" Start (RAM): "));
        Serial.println(getFreeRam());
    }
    //
    // Set new time every begin
    if (ampInt.isSecond()) {
         Serial.print(F(" Timer before (RAM): "));
        Serial.println(getFreeRam());
    }
    //
    // Amplitude loop init
    ampInt.listener();
    if (ampInt.isSecond()) {
         Serial.print(F(" Timer Listen (RAM): "));
        Serial.println(getFreeRam());
    }
    //
    // Listen engine
    carSens.listener();

    if (ampInt.isSecond()) {
         Serial.print(F(" Car Sens (RAM): "));
        Serial.println(getFreeRam());
    }
    //
    // Listen state pins
    carStat.listener();

    if (ampInt.isSecond()) {
         Serial.print(F(" Car State Listen (RAM): "));
        Serial.println(getFreeRam());
    }

#ifdef ADT_FUEL_SYSTEM_I2C
    cli();
    carSens.listenerI2cLpg(&i2cLpg);
    sei();
#endif

    if (ampInt.isSecond()) {
         Serial.print(F(" LPG Listen (RAM): "));
        Serial.println(getFreeRam());
    }

    //
    // Reads buttons from steering
    whlSens.listener();

    if (ampInt.isSecond()) {
         Serial.print(F(" Wheel Listen (RAM): "));
        Serial.println(getFreeRam());
    }
    //
    // Listener shutdown
    shutDown.listener();

    if (ampInt.isSecond()) {
         Serial.print(F(" Shutdown Listen (RAM): "));
        Serial.println(getFreeRam());
    }
    //
    //  Read main buttons
    btnMenu.listener();

    if (ampInt.isSecond()) {
         Serial.print(F(" Button Listen (RAM): "));
        Serial.println(getFreeRam());
    }
    //
    // Navigate menu from button listener
    menuBase.listener();

    if (ampInt.isSecond()) {
         Serial.print(F(" MenuBase Listen (RAM): "));
        Serial.println(getFreeRam());
    }
    //
    //  Switch to shutdown menu
    shutDown.cursor();

    if (ampInt.isSecond()) {
         Serial.print(F(" Shutdown cursor (RAM): "));
        Serial.println(getFreeRam());
    }
    //
    // Display UI
    lcdMenu.draw();

    if (ampInt.isSecond()) {
         Serial.print(F(" Lcd Draw (RAM): "));
        Serial.println(getFreeRam());
    }
    //
    // Commands that changes global value from serial monitor
    // ttd=<0000> INJECTS: Total distance
    // lpg=<0000> INJECTS: lpg consumption
    // bnz=<0000> INJECTS: bnz consumption
    eepRom.injectFromSerial();

    if (ampInt.isSecond()) {
         Serial.print(F(" Serial inject (RAM): "));
        Serial.println(getFreeRam());
    }
    //
    // Calls StackCount() to report the unused RAM
    if (ampInt.isSecond()) {
         Serial.print(F(" End free heap (RAM): "));
        Serial.println(getFreeRam());
    }
}


