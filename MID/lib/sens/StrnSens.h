//
// Created by Angel Zaprianov on 5/10/2016.
//


#ifndef ARDUINO_MID_STR_SENS_H
#define ARDUINO_MID_STR_SENS_H

int isButtonPressActive = 0;

//
// Creates test with maximum send value
#define TST_DIG_POD 256
#define ADR_DIG_POD B10001

#include <SPI.h>

class StrButtonsSony {

private:

    bool isButtonPressActive = 0;

    int testIndex = 0;
    uint8_t pinSteering, pinDigitalOut, pinOutVoltage;

    void testDigitalPod();

    int digitalPotWrite(int value, float voltage);


public:
    StrButtonsSony(uint8_t pinTargetSteering, uint8_t pinDigitalPod, uint8_t pinVoltage) {
        pinSteering = pinTargetSteering;
        pinDigitalOut = pinDigitalPod;
        pinOutVoltage = pinVoltage;
    }

    void setup();

    void listenButtons();

};

float getResistens(int valueStep) {

    int podK = 50; // 100k/50k

    return ((podK * 10 ^ 3) * (256 - valueStep) / 256 - 125);
}

/**
 * Send command to pod
 */
int StrButtonsSony::digitalPotWrite(int value, float voltage) {
    digitalWrite(pinDigitalOut, LOW);
    //send in the address and value via SPI:
    SPI.transfer(ADR_DIG_POD);
    SPI.transfer(value);
    int volts = int(255 * (voltage / 5));

    analogWrite(pinOutVoltage, volts);
    // take the SS pin high to de-select the chip
    digitalWrite(pinDigitalOut, HIGH);
    Serial.print(" Sending value to plot: ");

    Serial.print(volts);
    Serial.print("  \t");
    Serial.println(value);
}

void StrButtonsSony::testDigitalPod() {

#if defined(TST_DIG_POD)

    // adjust high and low resistance of potentiometer
    // adjust Highest Resistance .
//    digitalPotWrite(0x00);
//    delay(1000);
//
//    // adjust  wiper in the  Mid point  .
//    digitalPotWrite(0x80);
//    delay(1000);
//
//    // adjust Lowest Resistance .
//    digitalPotWrite(0xFF);
//    delay(1000);


    digitalWrite(pinDigitalOut, LOW);
    SPI.transfer(B10001); // 17
    SPI.transfer(testIndex);
    digitalWrite(pinDigitalOut, HIGH);
    //
    // Show value
    Serial.print("\n Test of digital pod is: ");
    Serial.println(testIndex);

    testIndex++;
    delay(100);
    if (testIndex >= TST_DIG_POD) {
        testIndex = 0;
    }

#endif
}


void StrButtonsSony::setup() {
    pinMode(pinSteering, INPUT);
    pinMode(pinDigitalOut, OUTPUT);
//  pinMode (SPICCLOCK, OUTPUT);//Needed to be defined?
//  pinMode (SLAVESELECT,OUTPUT); //same as above?
// initialize SPI:
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);//SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3
    SPI.setBitOrder(MSBFIRST); // LSBFIRST or MSBFIRST

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

//    StrButtonsSony::testDigitalPod();


    if (ampInt.isBig()) {
        Serial.print("Value reading steering: ");
        Serial.println(isButtonPressActive);
    }

//
    if (readingSteeringButton > 250 && isButtonPressActive == 0) {
        digitalPotWrite(0, 5);
        isButtonPressActive = 1;


    }

    //
    // Zero button
    if (readingSteeringButton > 25 && readingSteeringButton < 30) {
        Serial.print("Zero button");
        isButtonPressActive = 0;
    }

    if (ampInt.isMid()) {
        Serial.println(readingSteeringButton);
    }
    //
    // Volume up
    if (readingSteeringButton > 5 && readingSteeringButton < 14) {

        digitalPotWrite(byte(94), 1.85);


        if (ampInt.isMid()) {
            Serial.print("Volume Up");
            Serial.print("\t value:");
//            Serial.print(getVoltage(1.85));
            Serial.print("\n");
        }
        isButtonPressActive = 0;
    }

    //
    // Volume down
    if (readingSteeringButton > 0 && readingSteeringButton < 5) {
        digitalWrite(pinDigitalOut, LOW);
        SPI.transfer(B10001);
        SPI.transfer(byte(167));
        digitalWrite(pinDigitalOut, HIGH);
        if (ampInt.isMid()) {
            Serial.print("Volume Down");
            Serial.print("\t value:");
//            Serial.print(getVoltage(1.37));
            Serial.print("\n");
        }
        isButtonPressActive = 0;
    }


    //
    // Right arrow / seek up
    if (readingSteeringButton > 40 && readingSteeringButton < 50) {

        SPI.transfer(131);
        if (ampInt.isMid()) {
            Serial.print("Seek Up");
            Serial.print("\t value:");
//            Serial.print(getVoltage(2.55));
            Serial.print("\n");
        }
        isButtonPressActive = 0;
    }

    //
    // Left arrow / seek down
    if (readingSteeringButton > 70 && readingSteeringButton < 80) {
        digitalWrite(pinDigitalOut, LOW);
        SPI.transfer(B10001);
        SPI.transfer(byte(60));
        digitalWrite(pinDigitalOut, HIGH);
        if (ampInt.isMid()) {
            Serial.print("Seek Down");
            Serial.print("\t value:");
//            Serial.print(getVoltage(2.15));
            Serial.print("\n");
        }
        isButtonPressActive = 0;
    }


    //
    // Back button
    if (readingSteeringButton > 130 && readingSteeringButton < 140) {
//src
//        digitalWrite(pinDigitalOut,LOW);
//        SPI.transfer(B10001);
//        SPI.transfer(byte(118));
//        digitalWrite(pinDigitalOut,HIGH);


        SPI.transfer(200);
        if (ampInt.isMid()) {
            Serial.print("Back Button");
            Serial.print("\t value:");
//            Serial.print(getVoltage(2.95));
            Serial.print("\n");
        }
        isButtonPressActive = 0;
    }


}


#endif