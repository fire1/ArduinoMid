//
// Created by Angel Zaprianov on 5/10/2016.
//


#ifndef ARDUINO_MID_STR_SENS_H
#define ARDUINO_MID_STR_SENS_H

#include <SPI.h>


//
// Creates test with maximum send value
#define TST_DIG_POD 256 // Test full range resistance of digital potentiometer
#define ADR_DIG_POD B10001
//
// Uncomment to send resistance values from terminal
//#define STR_ACC_SER true

/**
 * Class converts Opel||Vauxhall steering wheel to Sony remote control
 */
class StrButtonsSony {

private:

    bool isButtonPressActive = 0;

    int testIndex = 0;
    uint8_t pinSteering, pinDigitalOut, pinOutVoltage;


    int digitalPotWrite(int resistance);

    int currentStateButton;
    int lastStateButton = 0;

    void setCurrentState(int currentButton);

public:

    const int STR_BTN_NON = 0;
    const int STR_BTN_VLD = 1;
    const int STR_BTN_VLU = 2;
    const int STR_BTN_SKU = 3;
    const int STR_BTN_SKD = 4;
    const int STR_BTN_BCK = 5;
    const int STR_BTN_ATT = 6;

    int getCurrentState();

    StrButtonsSony(uint8_t pinTargetSteering, uint8_t pinDigitalPod, uint8_t pinVoltage) {
        pinSteering = pinTargetSteering;
        pinDigitalOut = pinDigitalPod;
        pinOutVoltage = pinVoltage;

    }

    void setup();

    void listenButtons();

    void sendRadioButtons();

};

/***********************************************************************************************
 *                                                                                             *
 *                                   CPP part of file                                          *
 *                                                                                             *
 ***********************************************************************************************/


/**
 * Sets current button press
 */
void StrButtonsSony::setCurrentState(int currentButton) {
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
// initialize SPI:
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);//SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3
    SPI.setBitOrder(MSBFIRST); // LSBFIRST or MSBFIRST

}

/**
 * Send command to pod
 */
int StrButtonsSony::digitalPotWrite(int resistanceValue) {
    SPI.transfer(B10001); // 17
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

    int readingSteeringButton = analogRead(pinSteering);
    //
    // Testing method

#if defined(STR_ACC_SER)
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

    //
    // Default value  for sony whe Steering wheel is not used
    if (readingSteeringButton > 250 && isButtonPressActive == 0) {
        setCurrentState(STR_BTN_NON);
        //
        // Lock digital pot
        digitalWrite(pinDigitalOut, HIGH);
        digitalWrite(pinOutVoltage, HIGH);
        //
        // Do not enter in here next loop
        isButtonPressActive = 1;
    }

//    if (ampInt.isSec()) {
//        Serial.println(readingSteeringButton);
//    }

    //
    // Volume up
    if (readingSteeringButton > 9 && readingSteeringButton < 20) {
        setCurrentState(STR_BTN_VLU);
//        digitalPotWrite(95);
    }
    //
    // Volume down
    if (readingSteeringButton >= 0 && readingSteeringButton < 9) {
        setCurrentState(STR_BTN_VLD);
//        digitalPotWrite(115);
    }
    //
    // Zero button
    if (readingSteeringButton > 20 && readingSteeringButton < 30) {
        setCurrentState(STR_BTN_ATT);

        //
        // TODO long press 155 volume press button
//        digitalPotWrite(225); // moda
    }
    //
    // Right arrow / seek up
    if (readingSteeringButton > 35 && readingSteeringButton < 50) {
        setCurrentState(STR_BTN_SKU);
//        digitalPotWrite(45);
    }
    //
    // Left arrow / seek down
    if (readingSteeringButton > 60 && readingSteeringButton < 80) {
        setCurrentState(STR_BTN_SKD);
//        digitalPotWrite(65);
    }
    //
    // Back button
    if (readingSteeringButton > 130 && readingSteeringButton < 160) {
        setCurrentState(STR_BTN_BCK);
//        digitalPotWrite(15);
        // TODO long press 225 MODE
        // 5 - off
        //
    }

}

/**
 * Sends commands to radio
 */
void StrButtonsSony::sendRadioButtons() {

    int currentState = getCurrentState();

    //
    // Determinate button is pressed
    if (currentState != STR_BTN_NON && lastStateButton != currentState) {
        digitalWrite(pinDigitalOut, LOW);
        delay(20);
        digitalWrite(pinOutVoltage, LOW);
        lastStateButton = currentState;
        isButtonPressActive = 0;
    }


    switch (currentState) {

        case STR_BTN_VLU: // Volume up
            digitalPotWrite(95);
            break;

        case STR_BTN_VLD: // Volume up
            digitalPotWrite(115);
            break;

        case STR_BTN_ATT: // Zero
            digitalPotWrite(225);
            break;

        case STR_BTN_SKU:
            digitalPotWrite(45);
            break;

        case STR_BTN_SKD:
            digitalPotWrite(65);
            break;

        case STR_BTN_BCK:
            digitalPotWrite(15);
            break;
    }


    if (currentState == STR_BTN_NON && lastStateButton == currentState) {
        lastStateButton = currentState;
    }

}

#endif