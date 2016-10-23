/**
 *
 *  Created by Angel Zaprianov on 5/10/2016.
 *
 *  Class converts Opel||Vauxhall steering wheel to Sony remote control.
 *     + This class support SPI communication to digital potentiometer.
 *     + 50k digital potentiometer [MCP41050] is used to simulate
 *          Alphine remote controller.
 *     + 10k pull-up resistor from 5v Supplying voltage is used to
 *          determinate steering wheel button.
 *     + Some of buttons are out of resistant range if used digit pot.
 *
 *
 */

#ifndef ARDUINO_MID_STR_SENS_H
#define ARDUINO_MID_STR_SENS_H



#include <SPI.h>

//#define STR_DEBUG
#define STR_TYPE_A
//#define STR_TYPE_B

//
// Creates test with maximum send value
#define ADR_DIG_POD B10001
// pull up Resistance 10k - After test is not good ...
//#define STR_READ_RESIST_A
//
// pull up Resistance ~ 220 ohm
#define STR_READ_RESIST_B
//
// Uncomment to send resistance values from terminal
//#define STR_INJ_SRL true


class StrButtonsSony {

private:
    bool isButtonPressActive = 0;
    uint8_t pinSteering, pinDigitalOut, pinOutVoltage;
    int currentStateButton;
    int lastStateButton = 0;
    int closeStateButton = 0;

    //
    // Used for shortcuts ...
    boolean isDisabled = 0;

    void _setDigitalPot(int resistance);

    void _setCurrentState(int currentButton);

    void _parseButtonState(int currentState);

public:

    //
    // Define buttons values
    static constexpr int STR_BTN_NON = 0;
    static constexpr int STR_BTN_VLD = 1;
    static constexpr int STR_BTN_VLU = 2;
    static constexpr int STR_BTN_SKU = 3;
    static constexpr int STR_BTN_SKD = 4;
    static constexpr int STR_BTN_BCK = 5;
    static constexpr int STR_BTN_ATT = 6;


    /**
     *  Constrictor of StrButtonsSony class
     */
    StrButtonsSony(uint8_t pinTargetSteering, uint8_t pinDigitalPod, uint8_t pinVoltage) {
        pinSteering = pinTargetSteering;
        pinDigitalOut = pinDigitalPod;
        pinOutVoltage = pinVoltage;

    }

    int getAnalogReadButtons();

    void setup();

    void listenButtons();

    void sendRadioButtons();

    int getCurrentState();

    void disable() {
        isDisabled = 1;
    }

    void enable() {
        isDisabled = 0;
    }
};

/***********************************************************************************************
 *                                                                                             *
 *                                   CPP part of file                                          *
 *                                                                                             *
 ***********************************************************************************************/


/**
 * Sets current button press
 */
void StrButtonsSony::_setCurrentState(int currentButton) {
    currentStateButton = currentButton;
}

/**
 *  GEts current pressed button
 */
int StrButtonsSony::getCurrentState() {
    return currentStateButton;
}

/*
 * Setup Steering Wheel to Sony audio
 */
void StrButtonsSony::setup() {
    pinMode(pinSteering, INPUT);
    pinMode(pinDigitalOut, OUTPUT);
    pinMode(pinOutVoltage, OUTPUT);
    digitalWrite(pinOutVoltage, HIGH);
//  pinMode (SPICCLOCK, OUTPUT);//Needed to be defined?
//  pinMode (SLAVESELECT,OUTPUT); //same as above?
    //
    // initialize SPI:
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);//SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3
    SPI.setBitOrder(MSBFIRST); // LSBFIRST or MSBFIRST

}


void StrButtonsSony::_parseButtonState(int currentState) {

    if (currentState == STR_BTN_VLU) _setDigitalPot(95);// Volume up
    if (currentState == STR_BTN_VLD) _setDigitalPot(115);// Volume down
    if (currentState == STR_BTN_ATT) _setDigitalPot(225); // Zero
    if (currentState == STR_BTN_SKU) _setDigitalPot(45);// seek up
    if (currentState == STR_BTN_SKD) _setDigitalPot(65);// seek down
    if (currentState == STR_BTN_BCK) _setDigitalPot(15);// back button
    if (currentState == STR_BTN_NON) _setDigitalPot(0);// return to default
}


/**
 * Send command to pod
 */
void StrButtonsSony::_setDigitalPot(int resistanceValue) {
    SPI.transfer(ADR_DIG_POD); // 17
    SPI.transfer(resistanceValue);
}

/*
 *          SONY buttons value
 *
                resistance	voltage
        src      	    2200	4.1
        att	            4400	3.47
        off	            35	    4.98
        vol down	    23850	1.47
        vol up	        17000	1.85
        seek down	    12000	2.25
        seek up	        8800	2.65
        back	        6660	3
        front	        33940	1.13
        bottom	        48800	0.84
*/
void StrButtonsSony::listenButtons() {

    int readingSteeringButton = getAnalogReadButtons();
    //
    // Testing method

#if defined(STR_INJ_SRL)
    if (Serial.available()) {
        int val = Serial.parseInt();
        Serial.println(val);
        if (val != 0) {
            digitalWrite(pinDigitalOut, LOW);
            delay(30);
            digitalWrite(pinOutVoltage, LOW);
            SPI.transfer(B10001); // 17
            SPI.transfer(val);
            digitalWrite(pinDigitalOut, HIGH);
            delay(30);
            digitalWrite(pinOutVoltage, HIGH);
        }
    }
#endif

#if defined(STR_DEBUG)
    if (ampInt.isBig()) {
        Serial.print("Steering button: ");
        Serial.println(readingSteeringButton);
    }
#endif


#if defined(STR_READ_A)
    //
    // Default value  for sony when Steering wheel is not used
    if (readingSteeringButton > 250 && isButtonPressActive == 0) {
        _setCurrentState(STR_BTN_NON);
        //
        // Do not enter in here next loop
        isButtonPressActive = 1;
    }
    //
    // Volume up
    if (readingSteeringButton > 9 && readingSteeringButton < 20) {
        _setCurrentState(STR_BTN_VLU);
    }
    //
    // Volume down
    if (readingSteeringButton >= 0 && readingSteeringButton < 9) {
        _setCurrentState(STR_BTN_VLD);
    }
    //
    // Zero button
    if (readingSteeringButton > 20 && readingSteeringButton < 30) {
        _setCurrentState(STR_BTN_ATT);
        // TODO long press 155 volume press button
    }
    //
    // Right arrow / seek up
    if (readingSteeringButton > 35 && readingSteeringButton < 50) {
        _setCurrentState(STR_BTN_SKU);
    }
    //
    // Left arrow / seek down
    if (readingSteeringButton > 60 && readingSteeringButton < 80) {
        _setCurrentState(STR_BTN_SKD);
    }
    //
    // Back button
    if (readingSteeringButton > 130 && readingSteeringButton < 160) {
        _setCurrentState(STR_BTN_BCK);
    }

#endif

#if defined(STR_READ_RESIST_B)
    //
    // Default value  for sony when Steering wheel is not used
    if (readingSteeringButton > 900 && readingSteeringButton < 999 && isButtonPressActive == 0) {
        _setCurrentState(STR_BTN_NON);
        //
        // Do not enter in here next loop
        isButtonPressActive = 1;
    }
    //
    // Volume up
    if (readingSteeringButton > 400 && readingSteeringButton < 499) {
        _setCurrentState(STR_BTN_VLU);
    }
    //
    // Volume down
    if (readingSteeringButton > 200 && readingSteeringButton < 299) {
        _setCurrentState(STR_BTN_VLD);
    }
    //
    // Zero button
    if (readingSteeringButton > 500 && readingSteeringButton < 590) {
        _setCurrentState(STR_BTN_ATT);
        // TODO long press 155 volume press button
    }
    //
    // Right arrow / seek up
    if (readingSteeringButton > 600 && readingSteeringButton < 699) {
        _setCurrentState(STR_BTN_SKU);
    }
    //
    // Left arrow / seek down
    if (readingSteeringButton > 700 && readingSteeringButton < 799) {
        _setCurrentState(STR_BTN_SKD);
    }
    //
    // Back button
    if (readingSteeringButton > 800 && readingSteeringButton < 899) {
        _setCurrentState(STR_BTN_BCK);
    }

#endif
}

/**
 * Gets reading from steering buttons
 */
int StrButtonsSony::getAnalogReadButtons() {
    return analogRead(pinSteering);
}

/**
 * Sends commands to radio
 */
void StrButtonsSony::sendRadioButtons() {

    int currentState = getCurrentState();


    if (isDisabled) {
        return;
    }

#if defined(STR_TYPE_A)
    //
    // Determinate button is pressed
    if (lastStateButton != currentState) {
        digitalWrite(pinOutVoltage, LOW);
        delay(10);
        if (currentState == getCurrentState()) {
            digitalWrite(pinDigitalOut, LOW);
            lastStateButton = currentState;
            isButtonPressActive = 0;
            _parseButtonState(currentState);
        } else {
            digitalWrite(pinOutVoltage, HIGH);
        }
    }

    //
    //
    if (isButtonPressActive == 0 && closeStateButton != currentState || currentState == STR_BTN_NON) {
        //
        // Lock digital pot
        digitalWrite(pinDigitalOut, HIGH);
        closeStateButton = currentState;

    }

    if (isButtonPressActive == 1) {
        digitalWrite(pinOutVoltage, HIGH);
    }
#endif


#if defined(STR_TYPE_B)
    //
    // When is not none state
    if (currentState != STR_BTN_NON) {
        //
        // Open resistance to pot
        digitalWrite(pinOutVoltage, LOW);
        digitalWrite(pinDigitalOut, LOW);
        //
        // Check is still pressing and need to change state
        if (ampInt.isLow() && currentState == getCurrentState() && currentState != lastStateButton) {
            lastStateButton = currentState;
            _parseButtonState(currentState);
            digitalWrite(pinDigitalOut, HIGH);
        } else {
            //
            // Close state
            digitalWrite(pinDigitalOut, HIGH);
        }
        //
        // When button is returned to none
    } else if (currentState != lastStateButton) {
        digitalWrite(pinOutVoltage, HIGH);
        digitalWrite(pinDigitalOut, HIGH);
        lastStateButton = currentState;
    }

#endif

}

#endif