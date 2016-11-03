//
// Created by Angel Zaprianov on 19.10.2016 г..
//
#include <Arduino.h>
#include "TimeAmp.h"

#define MID_CAR_SENS_VERSION 0.1

#ifndef ARDUINOMID_ENGSENS_H
#define ARDUINOMID_ENGSENS_H
//
// City Speed alarm
#define VSS_ALARM_CITY_SPEED  62 // km
#define VSS_ALARM_VWAY_SPEED  102 // km
#define VSS_ALARM_HWAY_SPEED  142 // km
#define VSS_ALARM_ENABLED // Comment to disable speeding alarms
//
// Sensor correctors
#define ECU_CORRECTION 1.8
#define VSS_CORRECTION 3.847232 // original value is 3.609344 my tires are smaller
#define RPM_CORRECTION 32.767 // RPM OBD PID: 16,383.75 [*2] || [old: 32.8]
//
// Best 15636.44, 14952.25, 15736.44,
#define DST_CORRECTION 15167.81 // 16093.44  - ~6% = 15127.8336 [lower tire profile]
#define TRS_CORRECTION 0 // 0.064444 a proximity  6(~6)%
//
//#define VSD_SENS_DEBUG;
#define SCREEN_DEF_LIGHT 22
#define SCREEN_GO_TO_DEF 15

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
    TimeAmp *_amp;

private:

    bool _isEngineSens = false;
    bool _isVehicleSens = false;
    //
    // Divider for averages
    unsigned long LOOP_SENS_INDEX = 0;

    //
    // Engine temperature pin
    uint8_t pinTemp;
    uint8_t pinLpgTank;

    int
    //
    // Human Results
            CUR_VSS, CUR_RPM, CUR_ECU;
    //
    // Distance container
    unsigned long int CUR_VDS;
    //
    // LPG tank
    int CUR_LTK;
    //
    // Travel time
    unsigned long CUR_VTT;

    unsigned long indexLpgTank = 0;
    int long containerLpgTank = 0;

    //
    // Speeding alarms
    int speedAlarmCursor = 1;

    /**
     * Handles speeding alarms
     */
    void speedingAlarms();

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
 * Handle display dim
 */
    int long lastReadValueDim = 0;


protected:

    /**
      * Setup RPM
     */
    void setupRpmSens(uint8_t pinTarget) {
        pinMode(pinTarget, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt (pinTarget), EngSens_catchRpmHits, FALLING);
    }

    /**
      * Setup VSS
     */
    void setupVssSens(uint8_t pinTarget) {
        pinMode(pinTarget, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt (pinTarget), EngSens_catchVssHits, FALLING);
    }

    /**
      * Setup Ecu
     */
    void setupEcuSens(uint8_t pinTarget) {
        pinMode(pinTarget, INPUT_PULLUP);
        attachInterrupt(digitalPinToInterrupt(pinTarget), EngSens_catchEcuHits, FALLING);
    }


    void sensVss();

    void sensRpm();

    void sensEcu();

    void sensDim();

    void sensTnk();

    void sensAvr();

    void sensCrm();

public:

    CarSens(TimeAmp *ampInt);

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

    /**
     * Setup engine
     */
    void setupEngine(uint8_t pinVss, uint8_t pinRpm, uint8_t pinEcu, uint8_t pinTmp) {
        setupRpmSens(pinRpm);
        setupVssSens(pinVss);
        setupEcuSens(pinEcu);
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
     *
     */
    void setupLpg(uint8_t pinTank) {
        pinMode(pinTank, INPUT);
        pinLpgTank = pinTank;
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
     * Gets engine temperature
     */
    int getEngTmp() {
        return (analogRead(pinTemp) / 4 - 41);
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
        return (int) map(CUR_LTK, 785, 860, 0, 100);
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

        if (_amp->isMid()) {
            Serial.print("Time running: ");
            Serial.println(tmSec);
        }

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
     *  Listen sensors
     */
    void listener() {
        if (_amp->isSens()) {
            //
            // Counting sens loops
            LOOP_SENS_INDEX += 1;
        }


        //
        // No Interrupts
        cli();
        sensVss();
        sensRpm();
        sensEcu();
        // Interrupts
        //
        sei();
        sensTnk();
        sensAvr();
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
        // Sens display dim
        if (_amp->isMin()) {
            sensDim();
        }

        if (CUR_RPM > 500) {
            _isEngineSens = true;
        }
    }

};

/***********************************************************************************************
 *                                                                                             *
 *                                   CPP part of file                                          *
 *                                                                                             *
 ***********************************************************************************************/
CarSens::CarSens(TimeAmp *ampInt) {
    _amp = ampInt;
}


void EngSens_catchVssHits() {
    vssHitsCount++;
}

void EngSens_catchRpmHits() {
    rpmHitsCount++;
}

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

    if (_amp->isSec() && CUR_VSS > VSS_ALARM_CITY_SPEED && speedAlarmCursor == ENABLE_SPEED_CT) {
        tone(ADT_ALR_PIN, 4000, 500);
    }

    if (_amp->isSec() && CUR_VSS > VSS_ALARM_VWAY_SPEED && speedAlarmCursor == ENABLE_SPEED_VW) {
        tone(ADT_ALR_PIN, 4000, 500);
    }

    if (_amp->isSec() && CUR_VSS > VSS_ALARM_HWAY_SPEED && speedAlarmCursor == ENABLE_SPEED_HW) {
        tone(ADT_ALR_PIN, 4000, 500);
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

    if (_amp->isSens()) {
        indexLpgTank++;
        int lpgTankLevel = analogRead(pinLpgTank);

        if (lpgTankLevel > 500) {
            containerLpgTank += lpgTankLevel;
            CUR_LTK = int(containerLpgTank / indexLpgTank);
        }
    }

    if (_amp->isMinute()) {
        containerLpgTank = containerLpgTank / 3;
        indexLpgTank = indexLpgTank / 3;
    }


}

/**
 *  Average sens
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

int CarSens::getMxmVss() {
    return maxReachedSpeed;
}

/**
 *
 */
void CarSens::sensCrm() {

    if (_amp->isSecond()) {
        unsigned long currentTimeTrip = millis();
        CUR_VDS = currentTimeTrip / 1000;
        /*
        //
        // Pass records from mil sec
        if (currentTimeTrip < 1) {
            currentTimeTrip = currentTimeTrip * -1;
        }
        //
        // Time detector bay removing last record
        timeTravelTrip = timeTravelTrip + (currentTimeTrip - lastRecordTravelTimeTrip);
        //
        //
        lastRecordTravelTimeTrip = currentTimeTrip;
        CUR_VDS = timeTravelTrip / 1000;*/
    }
}

#endif //ARDUINOMID_ENGSENS_H
