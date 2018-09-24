
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
#include <EnableInterrupt.h>
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
//File logFile;

//
// Setup the code...
void setup() {
    //
    // Sets display pins to socket
    lcdPwr.begin();
    //
    // Setup SPI lib
    whlSens.begin(ALP_PIN_INP, ALP_PIN_OUT, ALP_PIN_MSK);
    //
    // Shutdown setup vehicle
    shutDown.begin(SAV_PIN_CTR, SAV_PIN_DTC, TONE_ADT_PIN);
    //
    // Debug serial
#if defined(DEBUG) && defined(SERIAL_BAUD)
    dump_init(SERIAL_BAUD);
    delay(50);
    dump_title("ARDUINO MID");
    dump("Firmware", MID_VERSION);
    dump_txt("Debug mode is enabled... type \"dbg=help\" for dump commands.");
    dump_txt("Or \"help=\" for general MID commands")
#endif
    //
    // Data logger
#ifdef LOGGER
    Logger.begin();
#endif
    //
    // Change timer 3
    setupTimer3();
    //
    // Starts sound effects
    melody.begin();
    //
    //
    eepRom.begin();
    //
    // Setup button listener
    btnMenu.begin(BTN_PIN_UP, BTN_PIN_DW, BRK_LGH_PIN, TONE_ADT_PIN);
    //
    // Sets Default Fuel as Benzine (fuel that engine will start) and additional LPG
    carSens.setupFuel({ifc: FUEL_BNZ_IFC, cns: FUEL_BNZ_CNS}, {ifc: FUEL_LPG_IFC, cns: FUEL_LPG_CNS});
    //
    // consumption
    // Engine / Speed sensors
    carSens.setupVehicle(SPD_SNS_PIN, RPM_SNS_PIN, ECU_SGN_PIN, ENG_CLT_PIN, BRK_LGH_PIN, FUL_TNK_PIN);
    //
    // Screen back-light
    carSens.setupScreen(DIM_PIN_VAL, DIM_PIN_OUT);
    //
    // Outside temperature (car stock temp sns)
    carSens.setupTemperature(TMP_PIN_OUT);
    //
    //  Setup car state pins to detect
    carStat.begin();
    //
    //
    lcdMenu.begin();
    //
    // Set MID menu
    midMenu.begin();
    //
    // Restore data
    eepRom.load();
    //
    // Pass saved data to car state for calculation
    carStat.setWorkState(eepRom.getWorkDistance());

    //
    // Shows MID intro
    lcdMenu.intro();
    //
    // Restore custom settings of var
    carSens.setSave(eepRom.getData());

    carStat.setWorkState(eepRom.getWorkDistance());
    //
    // LPG swtiching detection
#ifdef LPG_SWITCHING_DETECT
    lpgCom.begin();
#endif
    //
    // Aptitude
    aptService.begin();
    //
    // Light lamps test
    pinMode(STT_UL1_PIN, INPUT_PULLUP); // HIGH is ok


}


void loop() {
    //
    // Amplitude loop init
    ampInt.listener();
    //
    // Listen sound tempo
    melody.listener();
    //
    // Listen engine
    carSens.listener();
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
    // Listen state pins
    carStat.listener();
    //
    // Listen LPG fuel system from serial com
#ifdef ADT_FUEL_SYSTEM_SERIAL
    lpgCom.listener();
    carSens.setFuelListener(&lpgCom);
    //
    // set usage of LPG system without the switching listener
#elif defined(LPG_INSTALLATION)
    carSens.setFuelListener(true);
#else// Only default fuel system (benzine)
    carSens.setFuelListener();
#endif
    //
    // Display UI
    lcdMenu.draw();
    //
    // Commands that changes global value from serial monitor
    // ttd=<0000> INJECTS: Total distance
    // lpg=<0000> INJECTS: lpg consumption
    // bnz=<0000> INJECTS: bnz consumption
    eepRom.listener();


    if (ampInt.isSecond()) {
//        melody.playSpeed();
//        Serial.println(F("Test ... "));
    }
    //
    // Run other service
    aptService.listener();

}



// TODO make tast with this ISR
//ISR(USART2_RX_vect){
//
//}
//
