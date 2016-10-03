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


void resetStrPins() {

    int val = 130;
    analogWrite(ALP_PIN_SKD, val);
    analogWrite(ALP_PIN_SKP, val);
    analogWrite(ALP_PIN_BCK, val);
    analogWrite(ALP_PIN_VLD, val);
    analogWrite(ALP_PIN_VLP, val);
    analogWrite(ALP_PIN_ATT, val);
    analogWrite(ALP_PIN_SRC, val);
}


void setupAlphine(int pinTargetSteering) {
    pinMode(pinTargetSteering, INPUT);

    pinMode(ALP_PIN_SKD, OUTPUT);
    pinMode(ALP_PIN_SKP, OUTPUT);
    pinMode(ALP_PIN_BCK, OUTPUT);
    pinMode(ALP_PIN_VLD, OUTPUT);
    pinMode(ALP_PIN_VLP, OUTPUT);
    pinMode(ALP_PIN_ATT, OUTPUT);
    pinMode(ALP_PIN_SRC, OUTPUT);
    resetStrPins();


}

/*void sendCode(char *code) {
    const int alpCodeLength = 47;
    const int alpRate = 500; //microseconds
    const int alpInterval = 41; //milliseconds

    int alpPin = ALP_PIN_OUT;

    //Innit
    digitalWrite(alpPin, HIGH);
    delay(8); // 8 ms high init
    digitalWrite(alpPin, LOW);
    delayMicroseconds(4500); // 4.5 ms low init

    //Send command
    for (int i = 0; i <= alpCodeLength; i++) {
        if (code[i] == '1') {
            digitalWrite(alpPin, HIGH);
        } else {
            digitalWrite(alpPin, LOW);
        }
        delayMicroseconds(alpRate);
        digitalWrite(alpPin, LOW);
        delayMicroseconds(alpRate);
    }
    digitalWrite(alpPin, LOW);
    delay(41); //41ms interval between commands
}*/

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
        resetStrPins();
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
        analogWrite(ALP_PIN_ATT, 0);

    }

    //
    // Volume up
    if (readingSteeringButton > 14 && readingSteeringButton < 20) {
        digitalWrite(ALP_PIN_VLP, 0);

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
        analogWrite(ALP_PIN_VLD, 0);
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
