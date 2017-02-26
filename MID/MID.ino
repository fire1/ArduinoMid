#include <U8g2lib.h>
#include <U8x8lib.h>

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
#include <Wire.h>
#include <OneWire.h>

#include <MenuBackend.h>
#include <DallasTemperature.h>
#include <LiquidCrystal.h>
#include <U8g2lib.h>
//#include <SoftwareSerial.h>

#include "conf.h"

//
// I2C detector driver
//#include "lib/LpgSens.h"
//
// Interval / Amplitude
#include "lib/IntAmp.h"
//
// Adding Alphine emulator
#include "lib/WhlSens.h"
//
// Main Sensor handler
#include "lib/MainFunc.h"
//
// Engine sensors
#include "lib/CarSens.h"
//
// Car state
#include "lib/CarState.h"
//
// Menu button handler
#include "lib/MenuBtn.h"
//
//
#include "lib/EepRom.h"
//
// Add library
#include "lib/ShutDw.h"
//
// Add car games
#include "lib/CarGames.h"


//
// Amplitude interval
//    ampInt(min,low,mid,sec, big, max);
IntAmp ampInt(5, 10, 50, 100, 200, 1000);
//
// Constructing the class
CarSens carSens(&ampInt);
//
// Constructing car state
CarState carStat(&ampInt);
//
// Constructing the class
WhlSens whlSens(&ampInt);
//
// Data storage
EepRom eepRom(&carSens);
//
//
MenuBtn btnMenu(&ampInt, &carSens, &eepRom, &whlSens, &carStat);
//
// Additional libraries
// ----
// Shutdown constructor
ShutDw shutDown(&eepRom, &ampInt, &carSens, &whlSens);
//
// Car games
CarGames carGames(&ampInt, &carSens);

#if SCREEN == 162 || !defined(SCREEN)
//
// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(32, 33, 34, 35, 36, 37);
//
// Adding display's format
#include "lib/Lcd16x2.h"

IMidMenu *midMenu = new Menu16x2(&btnMenu, menuUseEvent, menuChangeEvent); // TODO move menu to lcd

//
// Event method set
void menuUseEvent(MenuUseEvent used) {
    Menu16x2::menuUsed(used);
}
//
// Event method set
void menuChangeEvent(MenuChangeEvent changed) {
    Menu16x2::menuChanged(changed);
}

ILcdMenu *lcdMenu = new Lcd16x2(&lcd, &btnMenu, midMenu, &carGames, &shutDown);

#elif SCREEN == 24064
//
// Check https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for display setup
U8G2_T6963_240X64_2_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,/*WR*/ 14, /*CE*/ 16, /*dc8*/17, /*RST*/ 18);
//
// Adding display's format
#include "lib/Lcd240x64.h"

IMidMenu *midMenu = new Menu240x60(&btnMenu, menuUseEvent);

ILcdMenu *lcdMenu = new Lcd240x62(&u8g2, &btnMenu, midMenu, &carGames, &shutDown);

#endif


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
    lcdMenu->begin();
    //
    // Set MID menu
    midMenu->setup();
    //
    // Setup SPI lib
    whlSens.setup(ALP_PIN_INP, ALP_PIN_OUT, ALP_PIN_VOL);
    //
    // Test usb detection
#if defined(USBCON)
    Serial.println("USB Connected ...");
#endif
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
    lcdMenu->intro();
}
//
//
// The interrupt can be enabled for each pin individually (analog and digital!),
// but there are only 3 interrupt vectors, so 6-8 pins share one service routine:
// ISR (PCINT1_vect) pin change interrupt for A0 to A5
//ISR (PCINT1_vect) { // handle pin change interrupt for A0 to A5 here
//
//    // Install Pin change interrupt for a pin, can be called multiple times from pciSetup()
//    Serial.print("\n\n\n\n");
//    Serial.print(" LPG dat: ");
//    Serial.print(digitalRead(pinLpgDat));
//    Serial.print(" LPG clc: ");
//    Serial.print(digitalRead(LPG_CLC_PIN));
//    Serial.print("\n\n\n\n");
//}

void loop() {
    //
    // Set new time every begin
    ampInt.setTimer(millis());
    //
    // Amplitude loop init
    ampInt.listener();
    //
    // Listen engine
    carSens.listener();
    //
    // Listen state pins
    carStat.listener();

#ifdef ADT_FUEL_SYSTEM_I2C
    cli();
    carSens.listenerI2cLpg(&i2cLpg);
    sei();
#endif
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
    midMenu->listener(cursorMenu);
    //
    //  Switch to shutdown menu
    shutDown.cursor(cursorMenu);
    //
    // Display UI
    lcdMenu->draw();
    //
    // Commands that changes global value from serial monitor
    // ttd=<0000> INJECTS: Total distance
    // lpg=<0000> INJECTS: lpg consumption
    // bnz=<0000> INJECTS: bnz consumption
    eepRom.injectFromSerial();

}


