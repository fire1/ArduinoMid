#include <Arduino.h>
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
//double AirFuelRatio = 14.70;  // константа расхода 14,7 воздуха к 1 литра бензина, у дизеля своя, у газа своя
double FuelDensityGramsPerLiter = 750.0;   // константа - грамм бензина в 1 литре бензина
double AirFuelRatio = 14.70;  // константа расхода 14,7 воздуха к 1 литра бензина, у дизеля своя, у газа своя

//
// MID plug pins definition over Arduino
//
// Define button pins for steering controller
const int BTN_PIN_UP = 8;
const int BTN_PIN_DW = 9;
//
// Engine pins
const int RPM_SNS_PIN = 2;    //  old:10 MID6 RPM [attachInterrupt]
const int SPD_SNS_PIN = 3;    // MID12 Speed sensor hub [attachInterrupt]
const int ECU_SGN_PIN = 19; // ECU signal
//
// Display dim pins
const int DIM_PIN_VAL = A10; // MID7 input Dim of display
const int DIM_PIN_OUT = 46; // output dim of display
//
// Temperatures
const int TMP_PIN_OUT = A9;

/* Extras ...   ******/
//
// Alarm / Tone pin
const int ADT_ALR_PIN = 11;
//
// Alpine / Steering Wheel buttons
const int ALP_PIN_INP = A8;
const int ALP_PIN_SKD = A7; //SEEK DOWN 	-	1
const int ALP_PIN_SKP = A1;
//SEEK UP		-	2
const int ALP_PIN_BCK = A2;
//BACK		- 	3
const int ALP_PIN_VLD = A3;
//VOL DW		-	4
const int ALP_PIN_VLP = A4;
//VOL UP		-	5
const int ALP_PIN_ATT = A5;
// Att 		-	5
const int ALP_PIN_SRC = A6;// Src		-	6



const int SAVE_PROTECT = A0; // 	-	1



/*
#include <SerialDebug.h>
#define DEBUG true
*/
//
//
//#include <SoftwareSerial.h>

//
// Includes Libraries

#include <LiquidCrystal.h>

//#include <SPI.h>



#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
/* Todo
#include <Wire.h>
*/

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

#include <MenuBackend.h>


#include "lib/InjData.h"

//
//
#include "lib/TimeAmp.h"

//
// Amplitude interval setup
TimeAmp ampInt(2, 50, 1, 10, 100);
//
// Main Sensor handler
#include "lib/MainFunc.h"
//
//
#include "lib/LcdChar.h"
//
// Adding menu source
#include "lib/MidMenu.h"
//
// Adding sensors
#include "lib/SensInit.h"

//
// Serial inject with max length 80 characters
InjData serialInject(80);
//
//
#include "lib/EepRom.h"

//
// Data storage
EepRom eepRom;

//
//
static void playWelcomeScreen();

//
// Setup the code...
void setup() {
    //
    // Turn display off
    lcd.noDisplay();

//    sei();
//#if defined(TCCR2) && defined(WGM20)
//
//    sbi(TCCR0A, WGM01);
//    sbi(TCCR0A, WGM00);
//    sbi(TCCR0B, CS01);
//    sbi(TCCR0B, CS00);
//    sbi(TIMSK0, TOIE0);
//
//    // set timer 1 prescale factor to 64
//    sbi(TCCR1B, CS11);
//    sbi(TCCR1B, CS10);
//    // put timer 1 in 8-bit phase correct pwm mode
//    sbi(TCCR1A, WGM10);
//    // set timer 3 prescale factor to 64
//    sbi(TCCR3B, CS22);
//    // configure timer 3 for phase correct pwm (8-bit)
//    sbi(TCCR3A, WGM20);
//
//#else
//
//#warning Timer 2 not finished (may not be present on this CPU)
//
//#endif
//    sei();

    // set timer 3 prescale factor to 64
    sbi(TCCR3B, CS22);
    // configure timer 3 for phase correct pwm (8-bit)
    sbi(TCCR3A, WGM20);
    //
    // Debug serial
    Serial.begin(9600);
    //
    //
    eepRom.setup();
    //
    //

    //
    // Engine sensors pin mode input
    setupRpmSens(RPM_SNS_PIN); // Engine RPM
    setupVssSens(SPD_SNS_PIN);    // Vehicle Speed Sensor
    setupEcuSens(ECU_SGN_PIN); // Signal from engine ECU  

    //
    // Multi steering handle
    setupAlphine(ALP_PIN_INP);

    //
    // Display back-light handler
    setupBackLight(DIM_PIN_VAL, DIM_PIN_OUT);
    //
    // Initializes the interface to the LCD screen
    lcd.begin(16, 2);
    lcd.clear();
    //
    // main setup
    setupMain();
    //
    // Adding custom characters to LCD
    setupLcdChar();
    lcd.display();


    //
    // Show welcome from car
//    playWelcomeScreen();

    //
    // Set MID menu
    setupMenu();

    //
    // Setup save protection  input
    pinMode(SAVE_PROTECT, INPUT);



    //
    // Restore data
    eepRom.loadCurrentData();

//    TTL_TLH = 6.24;
}

int long saveProtectInit = 0;


void loop() {

    //
    // Inject data from serial command
//    serialInject.listenerSerial();

    //
    //
    ampInt.listener();

    if (ampInt.isBig()) {
        Serial.print("\n Detected save pin value: ");
        Serial.println(analogRead(SAVE_PROTECT));
    }

    //
    // last 515
    if (analogRead(SAVE_PROTECT) < 820 && saveProtectInit == 0 || /* Only first initialization will run */
        analogRead(SAVE_PROTECT) < 820 && saveProtectInit + MILLIS_PER_MN < millis()) { /* next record after a minute */
//        eepRom.saveCurrentData();
        //
        // Show message

        Serial.print("\n\n");
        Serial.print("Data was recorded in EepRom !!! ********************************* ");
        Serial.print("\n\n");


        //
        // Close first initialization
        saveProtectInit = millis();
    } else {
        // TODO
    }

    //
    // Check recorded consumption
    if (ampInt.isMid()) {
        Serial.print("Consumption distance record  ");
        Serial.print(TTL_TLH);
        Serial.print("\n");
    }

    //
    // Sensors
    sensorsInit();
    //
    //  Read main buttons
    readButtons(BTN_PIN_UP, BTN_PIN_DW);
    //
    // Handle navigation
    navigateMenu();
    getTravelDistanceMeters();
    //
    // Switch menu from cursor
    switch (cursorMenu) {
        //
        // Main / first menu
        case 1:
            displayOutTmp();
            displayEngRPM();
            displayCarKMH();
            displayCarECU();
            break;
        case 5:

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


