//
// Created by Angel Zaprianov on 19.10.2016 г..
//
#include <Arduino.h>
#include "IntAmp.h"
//
#ifndef ARDUINO_MID_CAR_SENS_H
#define ARDUINO_MID_CAR_SENS_H


//
//

#define DEBUG_ENG_TEMP
// Version of MID plug driver
#define MID_CAR_SENS_VERSION 0.1
//
// Show information from consumption
//#define DEBUG_CONS_INFO
//
// Speeding alarm
#define VSS_ALARM_CITY_SPEED  61 // km
#define VSS_ALARM_VWAY_SPEED  101 // km
#define VSS_ALARM_HWAY_SPEED  141 // km
#define VSS_ALARM_ENABLED // Comment to disable speeding alarms
//
// --------------------------------------------------------------------------------------------------------------------
// ABOUT ECU signal
//      (actuated consumption signal)
//
//  On the GM hot film MAFs, you can also tap into the onboard computer data stream
//  with a scan tool to read the MAF sensor output in "grams per second" (GPS).
//  The reading might go from 3 to 5 GPS at idle up to 100 to 240 GPS at wide open throttle and 5000(+) RPM.
//  The frequency range varies from 30 to 150 Hz, with 30 Hz being average for idle and 150 Hz for wide open throttle.
//  Using as frequency MAF. Wide open throttle test reaches ~244 gps.
// --------------------------------------------------------------------------------------------------------------------
//
//
// ECU Consumption signal mul by *10
// next 3815
#define ENG_CORRECTION 7.6        //  Divider pure voltage
#define ECU_CORRECTION 346      //  <sens:200> 168          || <sens:150> 224           || <sens:100> 336      || <sens:50> 648
#define VSS_CORRECTION 3.767    //  <sens:200> 3.835232     || <sens:150> 5             || <sens:100> 7.670464 || <sens:50> 15.340928
#define RPM_CORRECTION 33.767   //  <sens:200> 33.767       || <sens:150> 50            || <sens:100> 67.534   || <sens:50> 135.068
#define DST_CORRECTION 15541.11 //  <sens:200> 15260.11     || <sens:150> 20266.66      || <sens:100> 30400    || <sens:50> 60791.24
//  DST
// ===============
// cur test +40 = 15240.11
// Best 15197.81,15636.44, 14952.25, 15736.44,
//
// VSS
// ===============
// 3.835232
//
#define TRS_CORRECTION 0 // 0.064444 a proximity  6(~6)%
//
//#define VSD_SENS_DEBUG;
#define SCREEN_DEF_LIGHT 22
#define SCREEN_GO_TO_DEF 15


#define DEFAULT_FUEL_STATE 1
/**************************/
/* GASOLINE ENGINE CONFIG */
/**************************/
// [CONFIRMED] For gas car use 3355 (1/14.7/730*3600)*10000
#define FUEL_BNZ_IFC 3355
#define FUEL_BNZ_CNS 107310 // 14.7*730*10

/************************/
/* LPG ENGINE CONFIG    */
/************************/
//LPG mass/volume is 520-580gr/ltr depending on propane/butane mix

// LPG/air ratio:
// 15.8:1 if 50/50 propane/butate is used
// 15:1 if 100 propane is used
// 15.4 if 60/40 propane/butane is used
// experiments shows that something in middle should be used eg. 15.4:1 :)

// [TEST PROGRESS] For lpg(summer >20C) car use 4412 (1/15.4/540*3600)*10000
#define FUEL_LPG_IFC 4329
#define FUEL_LPG_CNS 83160  // 15.4*540*10 = 83160
#define LPG_SWTC_PIN 7
/************************/
/* DIESEL ENGINE CONFIG */
/************************/
// [NOT TESTED] For diesel car use ??? (1/??/830*3600)*10000
//#define FUEL_DSL_CNS ????
//#define DSL_MAF_CNST ???   // ??*830*10
#define FUEL_ADJUST 1
/************************/
/*        ENGINE CONFIG */
/************************/
// https://en.wikipedia.org/wiki/Engine_displacement
// displacement = 0.7854.. X bore2 X stroke X Numb of Cylinders
//  Chevy bore is 4.312 in, and the stroke is 3.65 in, therefore the displacement for this eight-cylinder engine is:
//          3.1416/4 × (4.312 in)2 × 3.65 in × 8 = 426.4 cu in
/* Example
X18XE1  - 1796 cm3 = 17.96 dl
 */
#define ENGINE_DSP  17.96   // engine displacement in dL
#define CONS_TGL_VSS  3       // speed from which we toggle to fuel/hour (km/h)
#ifndef SWITCH_FUEL_ON_STATE
#define SWITCH_FUEL_ON_STATE LOW
#endif
struct Fuel {
    int ifc;
    long cns;
};

//
// Car's gears ratio
#define CAR_GEAR_G1  3.308
#define CAR_GEAR_G2  2.13
#define CAR_GEAR_G3  1.483
#define CAR_GEAR_G4  1.139
#define CAR_GEAR_G5  0.949
#define CAR_GEAR_G6  0.816
//
// Additional temperature sensor
//      [Dallas Temperature temperature sensor]
#ifndef INSIDE_TEMPERATURE_DS // Comment this to disable (check for same definition in main *.ino file )
//
// Inside temperature [very cheep temperature sensor]
// additional mounted temperature sensor from DallasTemperature
#define INSIDE_TEMPERATURE_DS
#endif
#define  DEBUG_TEMPERATURE_OU
//#define  DEBUG_TEMPERATURE_IN


//
// External Temperature
#define EXT_TMP_VSS  5
#define EXT_TMP_RST  10000
#define EXT_TMP_MSR  10 // measured temperature
#define EXT_TMP_MVL  328 // measured value of temperature

#if defined(INSIDE_TEMPERATURE_DS)

#include <OneWire.h>
#include <DallasTemperature.h>
// Data wire is plugged into pin A7 on the Arduino
#define ONE_WIRE_BUS 7
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature temperatureSensors(&oneWire);
#endif

//
// Using Arduino API for Attach interrupt
static void EngSens_catchRpmHits();

static void EngSens_catchVssHits();

static void EngSens_catchEcuHits();

//
// Hint counters
unsigned volatile int vssHitsCount, rpmHitsCount, ecuHitsCount;

/**
 * Car's sensing class
 */
class CarSens {

    //
    // Take a pointer to time amplitude instance
    IntAmp *_amp;

private:

    //
    // bool for read sensor at first loop
    bool isInitTemperature = 1;
    //
    // temperature pin container
    uint8_t pinTmpOut;
    //
    // Temperatures
    float CUR_OUT_TMP = 0; // Outside temperature
#if defined(INSIDE_TEMPERATURE_DS)
    float CUR_INS_TMP = 0; // Inside temperature /Dallas Temperature Sensors/
#endif
    //
    // Gears resolver constants
    const float CAR_GEAR_Dia = 616;
    const float CAR_GEAR_Pi = 3.14;

    unsigned long sensDeltaCnsOldTime,
            CONS_DELTA_TIME,
            TTL_FL_CNS,  // Consumed fuel
            TTL_FL_WST;  // Waste fuel

    bool _isEngineSens = false;
    bool _isVehicleSens = false;
    //
    // Divider for averages
    unsigned long LOOP_SENS_INDEX = 0;

    //
    // Engine temperature pin
    uint8_t pinTemp;


    int
    //
    // Human Results
            CUR_VSS, CUR_RPM, CUR_ECU, CUR_ENT;
    //
    // Distance container
    unsigned long int CUR_VDS;
    //
    // LPG tank
    int CUR_LTK;
    //
    // Detect fuel switch
    static unsigned long lastFuelStateSwitched; // Record time when is switched
    static int FUEL_STATE;
    static uint8_t pinSwitchFuel, pinLpgTank;

    int FUEL_INST_CONS;
    Fuel FUEL_PARAM_DEF, FUEL_PARAM_ADT;
    //
    // Fuel consumption variables
    unsigned long FL_CNS_DEF, FL_CNS_ADT, FL_WST_DEF, FL_WST_ADT;


    //
    unsigned long CUR_VTT;// Travel time

    unsigned long indexLpgTank = 0;
    int long containerLpgTank = 0;

    float FUEL_AVRG_INST_CONS;
    unsigned long collectionIfc, indexIfc;

    //
    // Speeding alarms
    int speedAlarmCursor = 1;

    //
    //
    int carGearNum = 0;

    /**
     * Handles speeding alarms
     */
    void speedingAlarms();

    /**
    *
    * @param value
    */
    void setConsumedFuel(long value);

    //
    // Screen back light vars
    static const int numReadingsDim = 10;
    int indexReadValDim = 0;
    int lastReadingsDim[numReadingsDim];
    int totalReadingDim = 0;
    int backLightLevel = 0; // Container of current level
    uint8_t pinScreenInput, pinScreenOutput;


    //
    // Car's average
    unsigned long averageAllVssValues = 0;
    unsigned long averageAllRpmValues;
    int long averageDivider = 0;
    bool initializeAverage = 0;
    //
    // Car's reached ...
    int maxReachedSpeed = 0;


    //
    // Car's Travel time
    unsigned long timeTravelTrip = 0;
    int long lastRecordTravelTimeTrip = 0;
    int vehicleStopped = LOW;

/**
 * Handle lcdDisplay dim
 */
    int long lastReadValueDim = 0;

    //
    // Digital read ECU
    uint8_t ecuPin;
    String ecuCollection;
    String ecuLine;

    //
    // Sense ECU signal
    void sensDre() {
        ecuCollection += digitalRead(ecuPin);
        if (_amp->isSens()) {
            ecuLine = ecuCollection;
            ecuCollection = "";
            Serial.println(ecuLine);
        }
    }


protected:
    /**
      * Setup RPM
      * @param pinTarget
      */
    void setupRpmSens(uint8_t pinTarget) {
        pinMode(pinTarget, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt (pinTarget), EngSens_catchRpmHits, FALLING);
    }

    /**
      * Setup VSS
      * @param pinTarget
      */
    void setupVssSens(uint8_t pinTarget) {
        pinMode(pinTarget, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt (pinTarget), EngSens_catchVssHits, FALLING);
    }

    /**
      * Setup Ecu
      * @param pinTarget
      */
    void setupEcuSens(uint8_t pinTarget) {
        pinMode(pinTarget, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(pinTarget), EngSens_catchEcuHits, FALLING);
    }

    /**
     * Gets calculate constant for instant consumption
     * @return int
     */
    int getIfcFuelVal() {
        if (getFuelState() == 0) return FUEL_PARAM_DEF.ifc;
        if (getFuelState() == 1) return FUEL_PARAM_ADT.ifc;
    }

    /**
     * Gets calculated constant for consumption
     * @return
     */
    long getCnsFuelVal() {
        if (getFuelState() == 0) return FUEL_PARAM_DEF.cns;
        if (getFuelState() == 1) return FUEL_PARAM_ADT.cns;
    }


    void sensVss();

    void sensRpm();

    void sensEcu();

    void sensDim();

    void sensTnk();

    void sensAvr();

    void sensCrm();

    void sensCns();

    void sensIfc();

    void sensEnt();

    void sensTmp();

    void sensDlt();

public:

    CarSens(IntAmp *ampInt);

    //
    // Speeding alarm modes
    const int DISABLE_SPEED_AL = 0, ENABLE_SPEED_CT = 1, ENABLE_SPEED_VW = 2, ENABLE_SPEED_HW = 3;

    void speedingAlarmsUp();

    void speedingAlarmsDw();

    /**
     * Clear peak
     */
    void clearBaseData() {
        CUR_VSS = 0, CUR_RPM = 0, CUR_ECU = 0;
    }

    int getGear(int CarSpeed, int Rpm);

    /**
     * Setup engine
      * @param pinVss
      * @param pinRpm
      * @param pinEcu
      * @param pinTmp
      */
    void setupEngine(uint8_t pinVss, uint8_t pinRpm, uint8_t pinEcu, uint8_t pinTmp) {
        setupRpmSens(pinRpm);
        setupVssSens(pinVss);
        setupEcuSens(pinEcu);
        //
        // Pass ecu pin
        ecuPin = pinEcu;
        //
        // Engine temperature
        pinMode(pinTmp, INPUT);
        pinTemp = pinTmp;
    }

    bool isRunEng() {
        return _isEngineSens;
    }

    bool isRunDst() {
        return _isVehicleSens;
    }

    /**
     * Setup additional fuel line
      * @param pinTank
      * @param pinSwitch
      */
    void setupAdtFuel(uint8_t pinTank, uint8_t pinSwitch) {
        pinMode(pinTank, INPUT);
        pinMode(pinSwitch, INPUT);
        //
        //
        CarSens::pinSwitchFuel = pinSwitch;
        CarSens::pinLpgTank = pinTank;
    }


    /**
    * Setup screen pins
    */
    void setupScreen(uint8_t pinInputInstrumentValue, uint8_t pinOutputDisplayContrast) {

        pinScreenInput = pinInputInstrumentValue;
        pinScreenOutput = pinOutputDisplayContrast;

        pinMode(pinScreenInput, INPUT);
        pinMode(pinScreenOutput, OUTPUT);
        //
        // Set default value
        analogWrite(pinScreenOutput, SCREEN_DEF_LIGHT);
        //
        // Sens dim level at setupEngine
        sensDim();

    }

    /**
     * Setups fuel lines to listen
      * @param defFuel
      * @param adtFuel
      */
    void setupFuel(Fuel defFuel, Fuel adtFuel = {0, 0}) {
        FUEL_PARAM_DEF = defFuel;
        //
        // Optional
        if (adtFuel.cns > 0 && adtFuel.ifc > 0) {
            FUEL_PARAM_ADT = adtFuel;
        }
    }

    /**
     * Setup temperature
      * @param pinOutsideTemperature
      */
    void setupTemperature(uint8_t pinOutsideTemperature) {
        pinTmpOut = pinOutsideTemperature;
        //
        // Setup outside pin
        pinMode(pinOutsideTemperature, INPUT);
        //
        // Setup inside pin
#if defined(INSIDE_TEMPERATURE_DS)
        temperatureSensors.begin();
#endif
    }

    float getTmpOut() {
        return CUR_OUT_TMP;
    }

    float getTmpIns() {
        return CUR_INS_TMP;
    }

    /**
     * Gets instant fuel consumption
     */
    int getIfc() {
        return FUEL_INST_CONS;
    }

    /**
     * Gets Instant fuel consumption average value
     */
    float getIfcAvr() {
        return FUEL_AVRG_INST_CONS;
    }

    /**
     * Gets default fuel line consumption
     */
    float getDefFuelCns() {
        return float(FL_CNS_DEF * 0.00001);
    }

    /**
     * Gets additional fuel line consumption
     */
    float getAdtFuelCns() {
        return float(FL_CNS_ADT * 0.00001);
    }

    /**
     * Gets fuel state  used
     */
    float getCurFuelCns() {
        if (getFuelState() == 0) return float(FL_CNS_DEF * 0.00001);
        if (getFuelState() == 1) return float(FL_CNS_ADT * 0.00001);
    }

    /*
     * Gets current fuel state
     */
    int getFuelState() {
        return CarSens::FUEL_STATE;
    }

    /**
     * Gets engine temperature
     */
    int getEngTmp() {
        // resistor 5.6k  to GND || pull-up resistor 4.2k from MID plug pin 31
        return CUR_ENT; // output must be 1:1 so... 95°C gives reading of 195
    }

    /**
     * Gets current Vss
     */
    int getVss() {
        return CUR_VSS;
    }

    /**
     * Gets current Rpm
     */
    int getRpm() {
        return CUR_RPM;
    }

    /**
     * Gets current Ecu
     */
    int getEcu() {
        return CUR_ECU;
    }

    int getTnkLpg() {
        return CUR_LTK;
    }

    int getTnkLpgPer() {

        return (int) CUR_LTK;/* map(CUR_LTK, 10, 100, 0, 100)*/
    }

    int getTnkBnz() {

        return 0;
    }

    int getTnkBnzPer() {
        return 0;
    }

    /**
     *  Gets travel time
     */
    int long getTime() {
        return CUR_VTT;
    }

    /**
     *  Gets Human time
     */
    char *getHTm() {

        char *dspTime = new char[6] /* 11 = len of clock time + 1 char for \0*/;

        unsigned long tmSec;
        int tmMin, tmHrs;

        tmSec = millis() / 1000;
        tmMin = int(tmSec / 60);
        tmHrs = tmMin / 60;

//        if (_amp->isMid()) {
//            Serial.print("Time running: ");
//            Serial.println(tmSec);
//        }

//        char dspTime[5];
        sprintf(dspTime, "%02d:%02d", tmHrs, tmMin);

//        strcpy(ch, dspTime);

        return dspTime;
    }

    /**
     *  Gets current Distance
     */
    float getDst() {
        /* my tires are smaller then original ... so my number must be lower, original must be / 16093.44 // one mile */
        float km = CUR_VDS / (DST_CORRECTION + TRS_CORRECTION);

        if (km <= 0) {
            km = 0;
        }
        return km;
    }

    long getTfc() {
        return TTL_FL_CNS;
    }

    /**
     * Gets Average Vss
     */
    int getAvrVss();

    /**
     * Gets Average Rpm
     */
    int getAvrRpm();

    /**
     * Gets maximum car speed
     */
    int getMxmVss();

    /**
     * Fuel pin listener
     */
    static void listenFuelSwitch();

    /**
     *  Listen sensors
     */
    void listener() {
        if (_amp->isSens()) {
            //
            // Counting sens loops
            LOOP_SENS_INDEX += 1;
        }


        // Important!
        // No Interrupts
        cli();
        sensVss();
        sensRpm();
        sensEcu();
        CarSens::listenFuelSwitch();
        // Interrupts
        //
        sei();
        //
        // Other
        sensTnk();
        sensAvr();
        sensEnt();
        sensTmp();

        //
        // Consumption
        sensDlt();
        sensCns();
        sensIfc();

        //
        // Test Ecu
//        sensDre();
        //
        // I don't know way but this is a fix ... ?
        // Only like this way base vars are initialized every single loop
        //
        if (_amp->isSens()) {
            int foo;
            foo = getEcu();
            foo = getRpm();
            foo = getVss();
            foo = getAvrVss();
            foo = getAvrRpm();
        }

        int vss = getVss();
        //
        // Detect time
        if (vss < 1) {
            vehicleStopped = HIGH;
        } else {
            vehicleStopped = LOW;
        }
        //
        // Car Run time
        // sensCrm();
        //
        // Sens lcdDisplay dim
        if (_amp->isMin()) {
            sensDim();
        }
        //
        // Mark engine on
        if (CUR_RPM > 500) {
            _isEngineSens = true;
        }
    }

};

/***********************************************************************************************
 * ########################################################################################### *
 * ########################################################################################### *
 *                                                                                             *
 *                                   CPP part of file                                          *
 *                                                                                             *
 * ########################################################################################### *
 * ########################################################################################### *
 ***********************************************************************************************/



/**
 * Construct class
  * @param ampInt
  */
CarSens::CarSens(IntAmp *ampInt) {
    _amp = ampInt;
}

//
// Fuel switch detection variables
int CarSens::FUEL_STATE = DEFAULT_FUEL_STATE;
unsigned long CarSens::lastFuelStateSwitched = 0;
uint8_t CarSens::pinSwitchFuel, CarSens::pinLpgTank;

/**
 * Interrupt function Vss
 */
void EngSens_catchVssHits() {
    vssHitsCount++;
}

/**
 * Interrupt function Rpm
 */
void EngSens_catchRpmHits() {
    rpmHitsCount++;
}

/**
 * Interrupt function Ecu
 */
void EngSens_catchEcuHits() {
    ecuHitsCount++;
}


/*******************************************************************
 * Detect Vss
 */
void CarSens::sensVss() {

    if (_amp->isSens()) {

        //
        // Pass vss to global
        CUR_VSS = int(vssHitsCount / (VSS_CORRECTION + TRS_CORRECTION));
        CUR_VDS = vssHitsCount + CUR_VDS;

//
// debug info
#if defined(VSS_SENS_DEBUG) || defined(GLOBAL_SENS_DEBUG)
        Serial.print("\n");
        Serial.print(" vss count:  \t");
        Serial.print(vssHitsCount);
        Serial.print(" vss is:  \t");
        Serial.print(vssHitsCount * VssCorrection);
        Serial.print("\n");
#endif
        //
        // Reset pulse counter
        vssHitsCount = 0;
    }
    //
    // Alarms
    speedingAlarms();
    //
    // Distance
#if defined(VSD_SENS_DEBUG) || defined(GLOBAL_SENS_DEBUG)
    if (_amp->isBig()) {
        Serial.print("Counted VSD is: ");
        Serial.println(CUR_VDS);
    }
#endif

}

/*******************************************************************
 * Detect RPMs
 */
void CarSens::sensRpm() {

    if (_amp->isSens()) {
        //
        // Pass rpm to global
        CUR_RPM = int(rpmHitsCount * RPM_CORRECTION);

//
// debug info
#if defined(RPM_SENS_DEBUG) || defined(GLOBAL_SENS_DEBUG)
        Serial.print("\n");
        Serial.print(" RPM count:  \t");
        Serial.print(rpmHitsCount);
        Serial.print(" RPM is:  \t");
        Serial.print(rpmHitsCount * RpmCorrection);
        Serial.print("\n");
#endif
        //
        // Clear value
        rpmHitsCount = 0;
    }
}

/*******************************************************************
* Detect Ecu
*/
void CarSens::sensEcu() {

    if (_amp->isSens()) {
        //
        // Pass ecu to global
        CUR_ECU = int(ecuHitsCount * ECU_CORRECTION);
//
// debug info
#if defined(ECU_SENS_DEBUG) || defined(GLOBAL_SENS_DEBUG)
        Serial.print("\n");
        Serial.print(" ecu count:  \t");
        Serial.print(ecuHitsCount);
        Serial.print(" ecu is:  \t");
        Serial.print(ecuHitsCount * EcuCorrection);
        Serial.print("\n");
#endif

        ecuHitsCount = 0;
    }

}

/*******************************************************************
* Speed Alarms
*/
void CarSens::speedingAlarms() {
#if defined(VSS_ALARM_ENABLED)
    //
    // Alarm speeding at city

    if (speedAlarmCursor < DISABLE_SPEED_AL) {
        speedAlarmCursor = ENABLE_SPEED_HW;
    }

    if (_amp->is5Seconds() && CUR_VSS > VSS_ALARM_CITY_SPEED && speedAlarmCursor == ENABLE_SPEED_CT) {
        tone(ADT_ALR_PIN, 4000, 200);
    }

    if (_amp->is10Seconds() && CUR_VSS > VSS_ALARM_VWAY_SPEED && speedAlarmCursor == ENABLE_SPEED_VW) {
        tone(ADT_ALR_PIN, 4000, 200);
    }

    if (_amp->isMinute() && CUR_VSS > VSS_ALARM_HWAY_SPEED && speedAlarmCursor == ENABLE_SPEED_HW) {
        tone(ADT_ALR_PIN, 4000, 200);
    }

    if (speedAlarmCursor > ENABLE_SPEED_HW) {
        speedAlarmCursor = DISABLE_SPEED_AL;
    }


#endif
}

/**
 * Alarm cursor changer UP
 */
void CarSens::speedingAlarmsUp() {
    speedAlarmCursor++;
}

/**
 * Alarm cursor changer Down
 */
void CarSens::speedingAlarmsDw() {
    speedAlarmCursor--;
}

/**
 *  Sensing Screen DIM
 */
void CarSens::sensDim() {

    int defaultActive = 0;

    totalReadingDim = totalReadingDim - lastReadingsDim[indexReadValDim];
    // read from the sensor:
    lastReadingsDim[indexReadValDim] = (int) map(analogRead(pinScreenInput), 0, 1023, 0, 255);
    // add the reading to the total:
    totalReadingDim = totalReadingDim + lastReadingsDim[indexReadValDim];
    // advance to the next position in the array:
    indexReadValDim = indexReadValDim + 1;


    backLightLevel = totalReadingDim / numReadingsDim;


    if (indexReadValDim >= numReadingsDim) {
        // ...wrap around to the beginning:
        indexReadValDim = 0;
    }


    if (backLightLevel < SCREEN_GO_TO_DEF) {
        backLightLevel = SCREEN_DEF_LIGHT;
        defaultActive = 1;
    } else {
        defaultActive = 0;
    }

    if (lastReadValueDim != backLightLevel && backLightLevel > 0) {
        lastReadValueDim = backLightLevel;

        if (defaultActive == 0) {
            backLightLevel = backLightLevel - SCREEN_DEF_LIGHT;
        }
        analogWrite(pinScreenOutput, backLightLevel);
    }
}

/**
 * Car tank/s sens
 */
void CarSens::sensTnk() {

    //
    // LPG tank
    //      Full tank reading        805
    //      Empty tank reading       ---
    if (_amp->isMax()) {
        indexLpgTank++;
        int lpgTankLevel = analogRead(CarSens::pinLpgTank);
        Serial.print("Tank level: ");
        Serial.println(lpgTankLevel);
        lpgTankLevel = int(lpgTankLevel - 805);


        Serial.print("after tank level: ");
        Serial.println(lpgTankLevel);

        if (lpgTankLevel > 0) {
            containerLpgTank = containerLpgTank + lpgTankLevel;
            CUR_LTK = int(containerLpgTank / indexLpgTank);
            CUR_LTK = lpgTankLevel;

        }
    }
    if (_amp->isMinute()) {
        containerLpgTank = 3;
        indexLpgTank = CUR_LTK * 3;
    }
}

/**
 *  Engine temperature
 */
void CarSens::sensEnt() {
    if (_amp->isSens()) {
        int val = analogRead(pinTemp);
        CUR_ENT = int(val / ENG_CORRECTION);

#ifdef DEBUG_ENG_TEMP

        Serial.print("Engine temperature: ");
        Serial.print(val);
        Serial.print(" / result:");
        Serial.println(CUR_ENT);
#endif
    }
}

/**
 *  Average speed & revolution
 *      and also maximum speed
 */
void CarSens::sensAvr() {

    int vss = getVss();
    int rpm = getRpm();

    //
    // Start average collection after run
    if (!initializeAverage && vss > 1) {
        initializeAverage = 1;
    }

    //
    // Check is initialize Average
    if (_amp->isSens() && initializeAverage) {
        averageAllVssValues += vss;
        if (rpm > 0) {
            averageAllRpmValues += rpm;
        }
        averageDivider += 1;
        //
        //  Resolve maximum speed reached
        if (maxReachedSpeed < vss) {
            maxReachedSpeed = vss;
        }
    }

}

/**
 * Gets Average Vss
 */
int CarSens::getAvrVss() {
    return int(averageAllVssValues / averageDivider);
}

/**
 * Gets Average Rpm
 */
int CarSens::getAvrRpm() {
    return int(averageAllRpmValues / averageDivider);
}

/**
 * Max reached speed
 */
int CarSens::getMxmVss() {
    return maxReachedSpeed;
}

/**
 * Temperature sensing
 */
void CarSens::sensTmp() {

/*******************************     DS    temperature sensor ******************************************/
#if defined(DEBUG_TEMPERATURE_IN)
    if (ampInt.isBig()) {
        temperatureSensors.requestTemperatures();
        Serial.print("Dallas temperature: \t");
        Serial.println(temperatureSensors.getTempCByIndex(0)); // Why "byIndex"?
    }
#endif

    //
    // Read inside temperature
#if defined(INSIDE_TEMPERATURE_DS)
    if (_amp->isBig()) {
        temperatureSensors.requestTemperatures();
        CUR_INS_TMP = temperatureSensors.getTempCByIndex(0);
    }
#endif
//    temperatureSensors.requestTemperatures();
//    temperatureSensors.getTempCByIndex(0);


    /******************************* Car's temperature sensor ******************************************/
    float temperatureC;
    /**
     * About GM Temperature sensor
     *      Temperature range to [°C]: 250
     *      Resistance [Ohm]: 5000
     * https://www.hackster.io/Marcazzan_M/how-easy-is-it-to-use-a-thermistor-e39321
     */



    float RT, VR, ln, TX, T0, voltage;
    //
    // Init on first loop the when is big amplitude
    if (isInitTemperature || _amp->isBig()) {

        T0 = EXT_TMP_MSR + 273.15;

        int reading = analogRead(TMP_PIN_OUT);              // Acquisition analog value of VRT
        voltage = (5.00 / 1023.00) * reading;      // Conversion to voltage
        VR = EXT_TMP_VSS - voltage;
        RT = voltage / (VR / EXT_TMP_RST);


        ln = log(RT / EXT_TMP_RST * 10000 /* 10000 pull-up R */);
        TX = (1 / ((ln / EXT_TMP_MVL) + (1 / T0))); // Temperature from thermistor
        temperatureC = TX * 0.01 + 3; // + 3 maybe


#if defined(DEBUG_TEMPERATURE_OU)
        if (_amp->isMid()) {
            Serial.print("Read Temp  value: ");
            Serial.print(reading);
            Serial.print("  | volts: ");
            Serial.print(voltage);
            Serial.print(" | calculation:");
            Serial.println(temperatureC);
        }
#endif

        //
        // Close first loop
        isInitTemperature = 0;

        //
        // Pass value to global
        CUR_OUT_TMP = temperatureC;
    }

}

/**
 * Detect delta time for consumption
 */
void CarSens::sensDlt() {
    // time elapsed
    if (_amp->isSens()) {
        unsigned long time_now;
        time_now = millis();
        CONS_DELTA_TIME = time_now - sensDeltaCnsOldTime;
        sensDeltaCnsOldTime = time_now;
    }
}

/**
 * Trip consumption
 */
void CarSens::sensCns() {

    // add MAF result to trip
    // we want fuel used in ÂµL
    // maf gives grams of air/s
    // divide by 100 because our MAF return is not divided!
    // divide by 14.7 (a/f ratio) to have grams of fuel/s
    // divide by 730 to have L/s
    // mul by 1000000 to have ÂµL/s

    // Formula:
    //      MPG = (14.7 * 6.17 * 454 * VSS * 0.621371) / (3600 * MAF / 100)
    //  We using short way:
    //      LKM =  MAF * time /  <fuel sum>
    // Since we want only consumption VSS is skipped

    if (_amp->isSens()) {
        long deltaFuel = 0;
        Serial.println(getCnsFuelVal());
        if (CUR_ECU > 0) {
            deltaFuel = (CUR_ECU * FUEL_ADJUST * CONS_DELTA_TIME) / getCnsFuelVal();
            // Direct correction in constant
//            deltaFuel = deltaFuel * 2; // Don't know way but need to be mul by 2
        }

        setConsumedFuel(deltaFuel);


//        //
//        // Deprecated from here ....        ----        >>>>>>
//        TTL_FL_CNS += deltaFuel;
//        //
//        //code to accumlate fuel wasted while idling
//        if (CUR_VSS == 0) {//car not moving
//            TTL_FL_WST += deltaFuel;
//        }
//        //
//        // Convert to float
////        CRT_LPG = float(TTL_FL_CNS * 0.00001);// L/h, comes from the /10000*100
//        float consumedFuel = float(TTL_FL_CNS * 0.00001);// L/h, comes from the /10000*100
////        setConsumedFuel(consumedFuel);
    }


}

/**
 * Instance Fuel Consumption
 */
void CarSens::sensIfc() {
    long cons;
    char decs[16];
    unsigned long delta_dist;



    // divide MAF by 100 because our function return MAF*100
    // but multiply by 100 for double digits precision
    // divide MAF by 14.7 air/fuel ratio to have g of fuel/s
    // divide by 730 (g/L at 15°C) according to Canadian Gov to have L/s
    // multiply by 3600 to get litre per hour
    // formula: (3600 * MAF) / (14.7 * 730 * VSS)
    // = maf*0.3355/vss L/km
    // mul by 100 to have L/100km

    float maf = CUR_ECU;

    if (_amp->isSens()) {

        delta_dist = ((CUR_VSS * 100) * CONS_DELTA_TIME); //

        // if maf is 0 it will just output 0
        if (CUR_VSS < CONS_TGL_VSS) {
            cons = long(
                    long(maf * getIfcFuelVal() / 2) / 1000 * 0.001);  // L/h, do not use float so mul first then divide
        } else {
            cons = long(maf * getIfcFuelVal() / 2) / delta_dist; // L/100kmh, 100 comes from the /10000*100
        }
        // pass
        // Current Instance consumption
        if (cons > 99) {
            cons = 99;
        }
        FUEL_INST_CONS = (int) cons;
        //
        // Average consumption for 5 seconds
        indexIfc++;
        // Comes from missing 200 milliseconds between read intervals
        collectionIfc += (cons  /** *  MILLIS_SENS*/);
        //
        // Average instance fuel consumption for 5 sec
        FUEL_AVRG_INST_CONS = (collectionIfc / indexIfc);//
    }

    // Average IFC for 5 sec
    // Keep last value as 1:3 rate
    if (_amp->isMinute()) {
        indexIfc = 3;
        collectionIfc = (unsigned long) FUEL_AVRG_INST_CONS * 3;
    }

#if defined(DEBUG_CONS_INFO) || defined(GLOBAL_SENS_DEBUG)
    if (_amp->isMax()) {

        Serial.print("\n\n Fuel Cons  | INS: ");
        Serial.print(FUEL_INST_CONS * 0.001);
        Serial.print(" || TTL: ");
        Serial.print(TTL_FL_CNS);
        Serial.print(" || ECU: ");
        Serial.print(CUR_ECU);

        Serial.print("\n\n ");
    }
#endif
}

/**
 * Car gear
 * TODO Needs testing
 */
int CarSens::getGear(int CarSpeed, int Rpm) {
    float FinalG, Ratio, Diff;

    FinalG = 3.706;
    if (CarSpeed != 0) {

        Ratio = (Rpm * CAR_GEAR_Pi * CAR_GEAR_Dia * 60) / (CarSpeed * FinalG * 1000000);

        carGearNum = 7;

        if ((-0.1 < Ratio - CAR_GEAR_G1) and (Ratio - CAR_GEAR_G1 < 0.1)) carGearNum = 1;
        if ((-0.1 < Ratio - CAR_GEAR_G2) and (Ratio - CAR_GEAR_G2 < 0.1)) carGearNum = 2;
        if ((-0.1 < Ratio - CAR_GEAR_G3) and (Ratio - CAR_GEAR_G3 < 0.1)) carGearNum = 3;
        if ((-0.1 < Ratio - CAR_GEAR_G4) and (Ratio - CAR_GEAR_G4 < 0.1)) carGearNum = 4;
        if ((-0.1 < Ratio - CAR_GEAR_G5) and (Ratio - CAR_GEAR_G5 < 0.1)) carGearNum = 5;
        if ((-0.1 < Ratio - CAR_GEAR_G6) and (Ratio - CAR_GEAR_G6 < 0.1)) carGearNum = 6;
    } else carGearNum = 0;

    return carGearNum;
}

/**
 * Sets Fuel consumed by engine
 */
void CarSens::setConsumedFuel(long value) {

    //
    // Recording wasted fuel
    if (CUR_VSS > CONS_TGL_VSS) {
        if (getFuelState() == 0) {
            FL_WST_DEF = FL_WST_DEF + value;
        } else {
            FL_WST_ADT = FL_WST_ADT + value;
        }
    }

    //
    //  Recording used fuel
    if (getFuelState() == 0) {
        FL_CNS_DEF = FL_CNS_DEF + value;
    } else {
        FL_CNS_ADT = FL_CNS_ADT + value;
    }

}

unsigned long dumpFuelSwitchCnt = 0;
unsigned long dumpFuelSwitchLvl = 0;
unsigned long dumpFuelSwitchSwt = 0;

/**
 * Detector of fuel switch
 */
void CarSens::listenFuelSwitch() {

    dumpFuelSwitchCnt++;

    if (!digitalRead(CarSens::pinSwitchFuel)) {
        dumpFuelSwitchSwt++;
    }

    if (!digitalRead(CarSens::pinLpgTank)) {
        dumpFuelSwitchLvl++;
    }

    //
    // Wait for event
    if (digitalRead(CarSens::pinSwitchFuel) == SWITCH_FUEL_ON_STATE) {
        //
        // Get current read time
        unsigned long currentListenSwitchTime = millis();
        if (CarSens::lastFuelStateSwitched + 1000 > currentListenSwitchTime) {
            //
            // Protecting Duplicated reads ...
            CarSens::lastFuelStateSwitched = currentListenSwitchTime;
            //
            // Changing fuel state
            if (CarSens::FUEL_STATE == 1) {
                CarSens::FUEL_STATE = 0;
            } else
                CarSens::FUEL_STATE = 1;
        }
    }

}

//ARDUINO_MID_CAR_SENS_H
#endif


