#include <Arduino.h>
#include <SPI.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <MenuBackend.h>


/*
---------------------------------------------------
    Arduino MID

     Author: Angel Zaprianov <me@fire1.eu>
     Version: 0.1 [pre-alpha]
     Start Date: 09.06.2016

     For the full copyright and license information, please view the LICENSE
     file that was distributed with this source code.
---------------------------------------------------

*/

//
// Uncommented to debug basics
//#define GLOBAL_SENS_DEBUG
//
// Time information
#define MILLIS_PER_HR    3600000 // Hour
#define MILLIS_PER_MN    60000    // Minute
#define MILLIS_PER_SC    1000    // Second
//
//
bool THROTTLE_UP = false; // Is open throttle  (acceleration)
const double CON_ENG_CC = 1.796; // Engine capacity [X18XE1]
const int CON_ENG_CL = 4; // Cylinders
const double FLW_MTR_FR = 1.414; // Flowmeter factor (revers-pressure)
const double AIR_FUL_RT = 14.70; // 14.7(oxygen) : 1(fuel) for burning
const double VEC_FUL_RT = 1.0;    // 14.7(oxygen) : 1(fuel) for burning
const double FuelDensityGramsPerLiter = 750.0;   // константа - грамм бензина в 1 литре бензина
const double AirFuelRatio = 14.70;  // константа расхода 14,7 воздуха к 1 литра бензина, у дизеля своя, у газа своя

//
// MID plug pins definition over Arduino
//
// Define button pins for steering controller
const uint8_t BTN_PIN_UP = 8;
const uint8_t BTN_PIN_DW = 9;
//
// Shutdown protection pin
const uint8_t SAV_PIN_CTR = A6; //
const uint8_t SAV_PIN_DTC = A7; //
//
// Engine pins
const uint8_t ENG_CLT_PIN = A5; // Engine Temp. MID32 RPM [attachInterrupt]
const uint8_t RPM_SNS_PIN = 2;  //  old:10 MID6 RPM [attachInterrupt]
const uint8_t SPD_SNS_PIN = 3;  //  MID12 Speed sensor hub [attachInterrupt]
const uint8_t ECU_SGN_PIN = 19; //  ECU signal
//
// Display dim pins
const uint8_t DIM_PIN_VAL = A10; // MID7 input Dim of display
const uint8_t DIM_PIN_OUT = 46; // output dim of display
//
// Temperatures
const uint8_t TMP_PIN_OUT = A9;

/* Extras ...   ******/
//
// Alarm / Tone pin
const uint8_t ADT_ALR_PIN = 11;
//
// Alpine / Steering Wheel buttons
const uint8_t ALP_PIN_INP = A8;
const uint8_t ALP_PIN_OUT = 53;
const uint8_t ALP_PIN_VOL = 14;

/*********************** Global Vars ***********************/
//
//  volatile Vehicle time travel
volatile float CUR_VTT = 0;
/*
#include <SerialDebug.h>
#define DEBUG true
*/
//
//
//#include <SoftwareSerial.h>

//
// Includes Libraries
//#include <SPI.h>



#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
/* Todo
#include <Wire.h>
*/

#include <LiquidCrystal.h>

//
// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(32, 33, 34, 35, 36, 37);

//
// Menu cursor
int cursorMenu = 0;
//
// Global interval
const int SNS_INTERVAL_TIME_LOW = 150; // Low sensor interval
const int SNS_INTERVAL_TIME_MID = 2000; // Mid sensor inter
int showerCounter = 0;

//
//
#include "lib/TimeAmp.h"

//
// Amplitude interval setupEngine
//      between loops
TimeAmp ampInt(/* min */5, /* low */10, /* mid */50, /* sec */100, /* big */200, /* max */ 1000); // TODO need tests
//
// Main Sensor handler
#include "lib/MainFunc.h"
//
//
#include "lib/LcdChar.h"
//
// Engine sensors
#include "lib/CarSens.h"

CarSens carSens;

//
// Adding sensors
#include "lib/SensInit.h"

//
// Config class
StrButtonsSony sensStr(ALP_PIN_INP, ALP_PIN_OUT, ALP_PIN_VOL);
//
// Adding menu source
#include "lib/MidMenu.h"
//
//
#include "lib/EepRom.h"

//
// Data storage
EepRom eepRom;

//
// Change state of shutdown "press to save"
#define SHUTDOWN_SAVE_STATE LOW
//
// Add library
#include "lib/ShutDw.h"

//
// Shutdown constructor
MidShutdown shutDown(SAV_PIN_CTR, SAV_PIN_DTC, BTN_PIN_UP, ADT_ALR_PIN);

//
//
static void playWelcomeScreen();


//
// Setup the code...
void setup() {
    //
    // Shutdown setupEngine
    shutDown.setup();
    //
    // Turn display off
    lcd.noDisplay();
    //
    // Change timer 3
    setupUseTimer3();
    //
    // Debug serial
    Serial.begin(9600);
    //
    //
    eepRom.setup();
    //
    //
//    carSens.setTimeAmp(ampInt);
    //
    // Engine sensors pin mode input
    carSens.setupEngine(SPD_SNS_PIN, RPM_SNS_PIN, ECU_SGN_PIN, ENG_CLT_PIN);

    setupTemperature();
    //
    // Display back-light handler
    setupBackLight(DIM_PIN_VAL, DIM_PIN_OUT);
    //
    // Initializes the interface to the LCD screen
    lcd.begin(16, 2);
    lcd.clear();
    //
    // main setupEngine
    setupMain();
    //
    // Adding custom characters to LCD
    setLcdBaseChar();
    lcd.display();
    //
    // Show welcome from car
//    playWelcomeScreen();
    //
    // Set MID menu
    setupMenu();
    //
    // Setup SPI lib
    sensStr.setup();
    //
    // Restore data
    eepRom.loadCurrentData();

//    TTL_TLH = 12.34;

}


void loop() {

#if defined(EEP__INJ_SER)
    if (Serial.available()) {
        int val = Serial.parseInt();

        if (val != 0) {

            float value = val * 0.01;

            int vl[2];
            Serial.print(" Insert value is \t ");
            Serial.println(vl[0]);
            Serial.println(",");
            Serial.println(vl[1]);
//            Serial.print("Sum between two is \t");
//            Serial.println(TTL_TLH + CUR_TLH);

            separateFloat(value, vl);

            Serial.print("Saving value \t ");
            Serial.println(value);


            eepRom.WireEepRomWriteByte(1, vl[0]);
            eepRom.WireEepRomWriteByte(2, vl[1]);

            delay(1000);

            int ts1, ts2;

            ts1 = eepRom.WireEepRomRead(1);
            ts2 = eepRom.WireEepRomRead(2);


//            TTL_TLH = eepRom.loadTravelConsumption();
            Serial.print("Restoring value \t ");
            Serial.println(ts1);
            Serial.println(",");
            Serial.println(ts2);

            float ttl = restoreFloat(int(ts1), int(ts2));


            Serial.println(ttl);

        }

    }
#endif

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
    // Sensors
    sensorsInit();
    //
    // Reads buttons from steering
    sensStr.listenButtons();
    //
    // Simulate resistance in radio
    sensStr.sendRadioButtons();
    //
    // Listener shutdown
    shutDown.listener();
    //
    //  Read main buttons
    readButtons(BTN_PIN_UP, BTN_PIN_DW);
    //
    // Handle navigation
    navigateMenu();
    //
    //  Switch to shutdown menu
    shutDown.cursor(cursorMenu);
    //
    // Switch menu from cursor
    switch (cursorMenu) {
        //
        // Main / first menu
        case 1:
            displayTotalCons();
            displayOutTmp();
            displayInsTmp();
            break;
        case 11:
            displayEngRPM();
            displayCarKMH();
            displayCarECU();
            displayCarDST();
            break;

        case 12:
            displayTest();
            break;
        case 4:
            displayAverage();
            break;
            //
            // Travel menu
        case 2:
            displayDistance();
            break;
        case 3:
            displayConsumption();
            break;
        case MidShutdown::MENU_SHUTDOWN:
            shutDown.display();
            break;
    }
}

/**
 * Welcome screen ...
 */
static void playWelcomeScreen() {
    lcd.setCursor(0, 0);
    lcd.print("Welcome to Astra");
    //
    // Test tone
    tone(ADT_ALR_PIN, 400, 100);
    delay(1000);
    lcd.setCursor(0, 1);
    lcd.print("Nice driving! ");
    tone(ADT_ALR_PIN, 400, 200);
    delay(500);
    tone(ADT_ALR_PIN, 600, 300);
    lcd.print(":)");
    delay(1500);
    lcd.clear();
}


