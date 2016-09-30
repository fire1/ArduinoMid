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

void setupAlphine(int pinTargetSteering, int pinTargetAlphine) {
    pinMode(pinTargetSteering, INPUT);
    pinMode(pinTargetAlphine, OUTPUT);
    analogWrite(pinTargetAlphine, 255);
}



float getVoltage(float volt) {
    return 255 * (volt / 5);
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
        analogWrite(ALP_PIN_OUT, 255);
    }

    //
    // Zero button
    if (readingSteeringButton > 25 && readingSteeringButton < 30) {
        Serial.print("Zero button");
        analogWrite(ALP_PIN_OUT, getVoltage(4.1));

    }

    //
    // Volume up
    if (readingSteeringButton > 14 && readingSteeringButton < 20) {
        analogWrite(ALP_PIN_OUT, getVoltage(1.85));
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
        analogWrite(ALP_PIN_OUT, getVoltage(1.47));

        if (ampInt.isMid()) {
            Serial.print("Volume Down");
            Serial.print("\t value:");
            Serial.print(getVoltage(1.47));
            Serial.print("\n");
        }
    }


    //
    // Right arrow / seek up
    if (readingSteeringButton > 40 && readingSteeringButton < 50) {
        // seek down 2.25
        analogWrite(ALP_PIN_OUT, getVoltage(2.65));
        if (ampInt.isMid()) {
            Serial.print("Seek Up");
            Serial.print("\t value:");
            Serial.print(getVoltage(2.65));
            Serial.print("\n");
        }
    }

    //
    // Left arrow / seek down
    if (readingSteeringButton > 70 && readingSteeringButton < 80) {
        // seek up 2.65
        analogWrite(ALP_PIN_OUT, getVoltage(2.25));

        if (ampInt.isMid()) {
            Serial.print("Seek Down");
            Serial.print("\t value:");
            Serial.print(getVoltage(2.25));
            Serial.print("\n");
        }
    }


    //
    // Back button
    if (readingSteeringButton > 130 && readingSteeringButton < 140) {
        steeringButtonPress = 6;
        analogWrite(ALP_PIN_OUT, getVoltage(3));
        if (ampInt.isMid()) {
            Serial.print("Back Button");
            Serial.print("\t value:");
            Serial.print(getVoltage(3));
            Serial.print("\n");
        }
    }


}
