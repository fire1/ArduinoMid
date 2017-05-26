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
#include "AmpTime.h"

//#define STR_DEBUG
//#define STR_WHL_SEND_A
#define STR_WHL_SEND_B

//
// Creates test with maximum send value
#define ADR_DIG_POD B10001
//
// pull up Resistance ~ 220 ohm
#define STR_READ_RESIST_B

//
// Uncomment to send resistance values from terminal
//#define STR_INJ_SRL true
class WhlSens {


    AmpTime *amp;

private:

    int currentStateButton;
    int lastStateButton = 0;
    uint8_t pinSteering, pinDigPotCntr, pinOutRelay;
    //
    // Used for shortcuts ...
    boolean isDisabled = 0;


    void _setDigitalPot(uint8_t resistance);

    void _setCurrentState(int currentButton);

    void setButtonStateParser(int currentState);

public:
    WhlSens(AmpTime &timeAmp) : amp(&timeAmp) {

    }

    //
    // Define buttons values
    static constexpr uint8_t STR_BTN_NON = 0;
    static constexpr uint8_t STR_BTN_VLD = 1;
    static constexpr uint8_t STR_BTN_VLU = 2;
    static constexpr uint8_t STR_BTN_SKU = 3;
    static constexpr uint8_t STR_BTN_SKD = 4;
    static constexpr uint8_t STR_BTN_BCK = 5;
    static constexpr uint8_t STR_BTN_ATT = 6;
    static constexpr uint8_t STR_BTN_MNT = 7;


    int getAnalogReadButtons();

    void setup(uint8_t pinTargetSteering, uint8_t pinDigitalPod, uint8_t pinRelay);

    void listener();

    void sendRadioButtons();

    int getCurrentState();

    void disable() {
        isDisabled = 1;
    }

    void enable() {
        isDisabled = 0;
    }

    boolean isDisable() {
        return isDisabled;
    }

    void shutdownMode(void);

};

/***********************************************************************************************
 *                                                                                             *
 *                                   CPP part of file                                          *
 *                                                                                             *
 ***********************************************************************************************/




/**
 * Sets current button press
 */
void WhlSens::_setCurrentState(int currentButton) {
    currentStateButton = currentButton;

#if defined(STR_DEBUG)
    if (amp->isSecond()) {
        Serial.print("WHL Button detect ");
        Serial.println(currentStateButton);
    }
#endif
}

/**
 *  GEts current pressed button
 */
int WhlSens::getCurrentState() {
    return currentStateButton;
}

/*
 * Setup Steering Wheel to Sony audio
 */
void WhlSens::setup(uint8_t pinTargetSteering, uint8_t pinDigitalPod, uint8_t pinRelay) {


    pinSteering = pinTargetSteering;
    pinDigPotCntr = pinDigitalPod;
    pinOutRelay = pinRelay;


    pinMode(pinSteering, INPUT);
    pinMode(pinDigPotCntr, OUTPUT);
    pinMode(pinOutRelay, OUTPUT);
    digitalWrite(pinOutRelay, LOW);
//  pinMode (SPICCLOCK, OUTPUT);//Needed to be defined?
//  pinMode (SLAVESELECT,OUTPUT); //same as above?
    //
    // initialize SPI:
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);//SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3
    SPI.setBitOrder(MSBFIRST); // LSBFIRST or MSBFIRST

}


void WhlSens::setButtonStateParser(int currentState) {

    if (currentState == STR_BTN_VLU) _setDigitalPot(95);// Volume up
    if (currentState == STR_BTN_VLD) _setDigitalPot(115);// Volume down
    if (currentState == STR_BTN_ATT) _setDigitalPot(225); // Zero
    if (currentState == STR_BTN_MNT) _setDigitalPot(155);// Mute
    if (currentState == STR_BTN_SKU) _setDigitalPot(45);// seek up
    if (currentState == STR_BTN_SKD) _setDigitalPot(65);// seek down
    if (currentState == STR_BTN_BCK) _setDigitalPot(15);// back button
    if (currentState == STR_BTN_NON) _setDigitalPot(0);// return to default

}

/**
 * Send value to dig-pot
 * @param  resistanceValue
 */
void WhlSens::_setDigitalPot(uint8_t resistanceValue) {
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
void WhlSens::listener() {

    int readingSteeringButton = getAnalogReadButtons();
    //
    // Testing method

#if defined(STR_INJ_SRL)
    if (Serial.available()) {
        int val = Serial.parseInt();
        Serial.println(val);
        if (val != 0) {
            digitalWrite(pinDigPotCntr, LOW);
            delay(30);
            digitalWrite(pinOutRelay, LOW);
            SPI.transfer(B10001); // 17
            SPI.transfer(val);
            digitalWrite(pinDigPotCntr, HIGH);
            delay(30);
            digitalWrite(pinOutRelay, HIGH);
        }
    }
#endif

#if defined(STR_DEBUG)
    if (amp->isMid()) {
        Serial.print("Steering button: ");
        Serial.println(readingSteeringButton);
    }
#endif
    _setCurrentState(STR_BTN_NON);
    //
    // Volume down
    if (readingSteeringButton > 200 && readingSteeringButton < 330) {
        _setCurrentState(STR_BTN_VLD);
    }
    //
    // Volume up
    if (readingSteeringButton > 330 && readingSteeringButton < 499) {
        _setCurrentState(STR_BTN_VLU);
    }
    //
    // Zero button
    if (readingSteeringButton > 500 && readingSteeringButton < 599) {
        _setCurrentState(STR_BTN_ATT);
//        if (amp->isMin()) {
//            _setCurrentState(STR_BTN_MNT);
//        }
        // TODO long press 155 volume press button
    }
    //
    // Right arrow / seek up
    if (readingSteeringButton > 600 && readingSteeringButton < 699) {
        _setCurrentState(STR_BTN_SKU);
    }
    //
    // Left arrow / seek down
    if (readingSteeringButton > 700 && readingSteeringButton < 810) {
        _setCurrentState(STR_BTN_SKD);
    }
    //
    // Back button
    if (readingSteeringButton > 810 && readingSteeringButton < 900) {
        _setCurrentState(STR_BTN_BCK);
    }
    //
    // Simulate resistance in radio
    sendRadioButtons();
}

/**
 * Gets reading from steering buttons
 */
int WhlSens::getAnalogReadButtons() {
    return analogRead(pinSteering);
}

/**
 * Sends commands to radio
 */
void WhlSens::sendRadioButtons() {

    int currentState = getCurrentState();

    //
    // Disable commands to radio
    if (isDisable()) {
        return;
    }
#if defined(STR_DEBUG)
    if (amp->isSecond()) {
        Serial.print("WHL Current State ");
        Serial.println(currentState);
    }
#endif
    //
    // When is not none state
    if (currentState != STR_BTN_NON) {

        digitalWrite(50, HIGH);

        digitalWrite(pinDigPotCntr, LOW);

        setButtonStateParser(currentState);
        delay(5); // Some separation fix
//
        // Open resistance to pot
        digitalWrite(pinDigPotCntr, HIGH);


        lastStateButton = currentState;

    } else {
        digitalWrite(50, LOW);
    }


}

/**
 * Change dig pod to shutdown mode
 */
void WhlSens::shutdownMode(void) {
    digitalWrite(pinDigPotCntr, LOW);
    analogWrite(pinOutRelay, HIGH);
    delay(5);
    _setDigitalPot(1);
    delay(5);
    analogWrite(pinDigPotCntr, LOW);

}

#endif