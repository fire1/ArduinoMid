#include <Arduino.h>


#define BitVal(data, y) ( (data>>y) & 1)

boolean serial2Low = false;
boolean Serial2High = true;
uint8_t serial2Index;
uint16_t serial2Length;
unsigned long serial2Timing;
//char dataBuff[] = {};
uint32_t dataBuff = 0;

void captureCommunication() {


    int receive = digitalRead(17);

    if (receive == LOW) {
        serial2Low = true;
    }

    if (receive == HIGH && serial2Low && !Serial2High) {
        serial2Timing = millis();
        Serial2High = true;
        serial2Low = false;
    }

    if (receive == LOW && Serial2High) {
        serial2Length = (uint8_t) (millis() - serial2Timing); //if it is low, end the time
        Serial2High = false;
        serial2Low = false;

        if (serial2Length <= 3) {
            dataBuff |= 0 << serial2Index;
//            dataBuff[serial2Index] = 0;
            serial2Index++;
        }

        if (serial2Length > 3 && serial2Length <= 6) {
            dataBuff |= 1 << serial2Index;
//            dataBuff[serial2Index] = 1;
            serial2Index++;
        }

        //
        // Todo test
        if (serial2Length > 10) {
            serial2Low = false;
            Serial2High = false;
        }


    }
    // LPG
    // 2221 2201
    // 101000000001

    // BNZ
    // 1501 1521 C01 1C00 201
    // 110000000001

    if (serial2Length > 10 && dataBuff > 0) {
        Serial.print(F("BIN "));
        Serial.print(dataBuff, BIN);
        Serial.print(" / total:  ");
        Serial.print(serial2Index);
        Serial.print(" / detects ");
        Serial.print(BitVal(x, 2));

        Serial.println();
        dataBuff = 0;
        serial2Index = 0;
    }


}


void setup() {
    Serial.begin(115200);

    Serial.print("Beginning .....");
}

void loop() {
    captureCommunication();
}