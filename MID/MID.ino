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
    // Shows MID intro
    lcdMenu.intro();
    //
    // Restore custom settings of var
    carSens.setSave(eepRom.getData());
    //
    // TODO testing ...

#ifdef ADT_FUEL_SYSTEM_SERIAL
    lpgCom.begin();
#endif

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

    if (ampInt.isSecond()) {
//        Serial.print(F(" Start (RAM): "));
//        Serial.println(getFreeRam());
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
    // Listen LPG fuel system
#ifdef ADT_FUEL_SYSTEM_SERIAL
    lpgCom.listener();
    carSens.setFuelListener(&lpgCom);
#else
    carSens.setFuelListener();
#endif
    //
    // Display UI
    lcdMenu.draw();
    if (ampInt.isSecond()) {
//        Serial.print(F(" Draw (RAM): "));
//        Serial.println(getFreeRam());
        Serial.print(F(" Fuel state is "));
        Serial.println(carSens.getFuelState());
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
//
//        Serial.print("Correction Dst: ");
//        Serial.println(eepRom.getData().sens_dst);

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


