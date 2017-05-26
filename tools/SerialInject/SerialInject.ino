#include <Arduino.h>

void setup() {


    // put your setup code here, to run once:
    pinMode(50, OUTPUT);
    pinMode(13, OUTPUT);
    Serial.begin(115200);
}

void loop() {


    String srlAllData;
    String srlStrName;
    String srlOutputs;
    //
    // Default message
    srlOutputs = F(" None ... Sorry! ");
    //
    // Execute command from serial
    if (Serial.available() > 0) {
        float saveTemp;

        srlStrName = Serial.readStringUntil('=');
        //
        // So ... C++ is static language...
        //      is not good idea to re-set dynamically

        if (srlStrName == "p50") {
            // trip distance
            saveTemp = Serial.readStringUntil('\n').toInt();


            if (saveTemp > 0) {
                digitalWrite(50, HIGH);
            } else {
                digitalWrite(50, LOW);
            }

            srlOutputs = F("PinSet ");
            srlOutputs += saveTemp;


        }


        //
        // Show command information to human
        Serial.print(F("\n\n==============================================================\n"));
        Serial.print(F("\n \t\t  [MID $]> Affected value of "));
        Serial.print(srlOutputs);
        Serial.print(F("\n\n\n==============================================================\n\n"));
        //
        // Remove containers
        srlStrName = "";
        srlAllData = ""; // Clear recieved buffer
        srlOutputs = "";
    }
}