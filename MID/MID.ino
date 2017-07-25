/*
---------------------------------------------------
    Arduino MID

     Author: Angel Zaprianov <me@fire1.eu>
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
#include <SD.h>
//
#ifdef EEP_ROM_ADDRESS

#include <Wire.h>

#else

#include <EEPROM.h>

#endif
//
#include "MID.h"
#include "lib/InitObj.h"
#include "lib/InitLcd.h"


//
//
//LpgSens lpgSens;
#ifdef ADT_FUEL_SYSTEM_I2C
//I2cSimpleListener i2cLpg(pinLpgDat, LPG_CLC_PIN);
#endif
//
// Log file
File logFile;

//
// Setup the code...
void setup() {
    //
    // Sets display pins to socket
    lcdPwr.begin();
    //
    // Shutdown setupvehicle
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
    btnMenu.setup(BTN_PIN_UP, BTN_PIN_DW, BRK_LGH_PIN, TONE_ADT_PIN);
    //
    // Sets Default Fuel as Benzine (fuel that engine will start) and additional LPG
    carSens.setupFuel({FUEL_BNZ_IFC, FUEL_BNZ_CNS}, {FUEL_LPG_IFC, FUEL_LPG_CNS});
    //
    // Setup fuel gauge two required wires
    carSens.setupAdtFuel(pinLpgDat, LPG_CLC_PIN);
    //
    // consumption
    // Engine / Speed sensors
    carSens.setupVehicle(SPD_SNS_PIN, RPM_SNS_PIN, ECU_SGN_PIN, ENG_CLT_PIN, BRK_LGH_PIN);
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
    whlSens.setup(ALP_PIN_INP, ALP_PIN_OUT, ALP_PIN_RLY);
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
    //
    // Restore custom settings of var
    carSens.setSave(eepRom.getData());
    //
    // TODO testing ...
    // for 7bit frame 109
//    pinMode(17, INPUT); Serial

    Serial2.begin(246); // pin 17  input  245
    // https://community.particle.io/t/serial-7-bit-data-even-parity/23446/10
    Serial.print("Starting SD..");
    if(!SD.begin(8)) Serial.println("failed");
    else Serial.println("ok");

    // open the file. note that only one file can be open at a time,
    // so you have to close this one before opening another.
//    logFile =  SD.open("test.txt", FILE_WRITE);
    // if the file opened okay, write to it:

/*
    // re-open the file for reading:
    logFile = SD.open("test.txt");
    if (logFile) {
        Serial.println("test.txt:");
        // read from the file until there's nothing else in it:
        while (logFile.available()) {
            Serial.write(logFile.read());
        }
        // close the file:
        logFile.close();
    } else {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
    }
    */

    logFile =  SD.open("test.txt", FILE_WRITE);
    if (logFile) {
        Serial.print("Writing log to test.txt...");
        logFile.println("Start new session ------------------------------------");
        // close the file:
        logFile.close();
        Serial.println("done.");
    }
}
/*
     if (logFile) {
        Serial.print("Writing to test.txt...");
        logFile.println("Some test");
        // close the file:
        logFile.close();
        Serial.println("done.");
    } else {
        // if the file didn't open, print an error:
        Serial.println("error opening test.txt");
    }
 */
void loop() {

    if (Serial2.available() > 0) {
        int ch = Serial2.read();
//        ch = ;
//        Serial.println("");
//        Serial.print("LPG: ");
//        Serial.println(Serial2.read(),HEX);
//        Serial.println("");
        logFile =  SD.open("test.txt", FILE_WRITE);
        if (logFile) {
            Serial.print("Writing log to test.txt...");
            logFile.println(ch, HEX);
            // close the file:
            logFile.close();
            Serial.println("done.");
        }

        //
        // Start Executing command
        if(0x8C == ch){

        }

    }


    if (ampInt.isSecond()) {
        Serial.print(F(" Start (RAM): "));
        Serial.println(getFreeRam());
    }
    //
    // Amplitude loop init
    ampInt.listener();
    //
    // Listen engine
    carSens.listener();
    //
    // Listen state pins
    carStat.listener();
//#ifdef ADT_FUEL_SYSTEM_I2C
//    cli();
//    carSens.listenerI2cLpg(&i2cLpg);
//    sei();
//#endif
    //
    // Reads buttons from steering
    whlSens.listener();
    //
    // Listener shutdown
    shutDown.listener();

    //
    //  Read main buttons
    btnMenu.listener();
    //
    // Navigate menu from button listener
    menuBase.listener();
    //
    //  Switch to shutdown menu
    shutDown.cursor();

    //
    // Display UI
    lcdMenu.draw();
    if (ampInt.isSecond()) {
        Serial.print(F(" Draw (RAM): "));
        Serial.println(getFreeRam());
    }
    //
    // Commands that changes global value from serial monitor
    // ttd=<0000> INJECTS: Total distance
    // lpg=<0000> INJECTS: lpg consumption
    // bnz=<0000> INJECTS: bnz consumption
    eepRom.injectFromSerial();
    //
    // Calls StackCount() to report the unused RAM
    if (ampInt.isSecond()) {

        Serial.print("Correction Dst: ");
        Serial.println(eepRom.getData().sens_dst);

        /*
        Serial.print(F(" End free heap (RAM): "));
        Serial.println(getFreeRam());
        Serial.print(F(" VSS "));
        Serial.println(carSens.getVss());
        Serial.print(F(" RPM "));
        Serial.println(carSens.getRpm());
        Serial.print(F(" ECU "));
        Serial.println(carSens.getEcu());

        Serial.print(F(" RpmC "));
        Serial.println(eepRom.getSensRpm());
//        Serial.print(F(" TM3 " ));
//        Serial.println(testTimer);
         */

    }
}


