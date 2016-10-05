/*
    Steering wheel buttons to Sony Alpine

            resistance  voltage
---------------------------------------------------
src             2200      4.1
att             4400      3.47
off             35        4.98
vol down        23850     1.47
vol up          17000     1.85
seek down       12000     2.25
seek up         8800      2.65
back            6660      3
front           33940     1.13
bottom          48800     0.84
---------------------------------------------------
 Sony Voltages                                  
 */


#include <SPI.h>


const int sonySrc = 41; //4.1v
const int sonyAtt = 34;
const int sonyOff = 49;
const int sonyVolDw = 14;
const int sonyVolUp = 18;
const int sonySeekDw = 26;
const int sonySeekUp = 22;
const int sonyBack = 29;
const int sonyFront = 11;
const int sonyBottom = 8;
//
// Output for Sony Alpine emulation
const int sonyPin = A0;
const int sonyShiftPin = A2;
const int alpinePin = 3;


void resetStrPins() {

    int val = 130;

}


void setupAlphine(int pinTargetSteering) {
    pinMode(pinTargetSteering, INPUT);

    pinMode(7, OUTPUT);


    SPI.begin();

    SPI.transfer(0); // command
    SPI.transfer(0); // value


}


float getVoltage(float volt) {
    return 255 * (volt / 3.80);
}

/**
        Handle Steering wheel media buttons
 */
void sensStr() {
//    if (isSensorReadLow()) {
    int readingSteeringButton = analogRead(ALP_PIN_INP);
//    Serial.print("\n MultiVolan: ");
//    Serial.print(readingSteeringButton);
//    Serial.print("\n ");

    int steeringButtonPress = 0;
/*
                resistance	voltage
src      	    2200	4.1
att	            4400	3.47
off	            35	4.98
vol down	    23850	1.47
vol up	        17000	1.85
seek down	    12000	2.25
seek up	        8800	2.65
back	        6660	3
front	        33940	1.13
bottom	        48800	0.84
*/


    if (readingSteeringButton > 250) {
        SPI.transfer(0);
    }


    if (Serial.available()) {
//        mySerial.write(Serial.read());
//        sendCode(Serial.read());
//        analogWrite(ALP_PIN_OUT, getVoltage(Serial.read()));
    }

    //
    // Zero button
    if (readingSteeringButton > 25 && readingSteeringButton < 30) {
        Serial.print("Zero button");
        SPI.transfer(11);

    }

    //
    // Volume up
    if (readingSteeringButton > 14 && readingSteeringButton < 20) {
        SPI.transfer(21);

        if (ampInt.isMid()) {
            Serial.print("Volume Up");
            Serial.print("\t value:");
            Serial.print(getVoltage(1.85));
            Serial.print("\n");
        }
    }

    //
    // Volume down
    if (readingSteeringButton > 5 && readingSteeringButton < 14) {
        SPI.transfer(141);
        if (ampInt.isMid()) {
            Serial.print("Volume Down");
            Serial.print("\t value:");
            Serial.print(getVoltage(1.37));
            Serial.print("\n");
        }
    }


    //
    // Right arrow / seek up
    if (readingSteeringButton > 40 && readingSteeringButton < 50) {

        analogWrite(ALP_PIN_SKP, 0);
        if (ampInt.isMid()) {
            Serial.print("Seek Up");
            Serial.print("\t value:");
            Serial.print(getVoltage(2.55));
            Serial.print("\n");
        }
    }

    //
    // Left arrow / seek down
    if (readingSteeringButton > 70 && readingSteeringButton < 80) {

        analogWrite(ALP_PIN_SKD, 0);
        if (ampInt.isMid()) {
            Serial.print("Seek Down");
            Serial.print("\t value:");
            Serial.print(getVoltage(2.15));
            Serial.print("\n");
        }
    }


    //
    // Back button
    if (readingSteeringButton > 130 && readingSteeringButton < 140) {

        analogWrite(ALP_PIN_BCK, 0);
        if (ampInt.isMid()) {
            Serial.print("Back Button");
            Serial.print("\t value:");
            Serial.print(getVoltage(2.95));
            Serial.print("\n");
        }
    }


}
