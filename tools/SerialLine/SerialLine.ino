#include <Arduino.h>
#include <SoftwareSerial.h>


const uint8_t pinLpgDat = A5;     //  [brown]     Switch DATA     Tank fuel level     /// A8
const uint8_t LPG_CLC_PIN = A4;     //  [blue]      Switch button   Fuel switcher       /// A9

SoftwareSerial portOne(pinLpgDat, LPG_CLC_PIN);

void setup() {
    Serial.begin(250000);

    // set the data rate for the ReceiveOnlySoftwareSerial port
    portOne.begin(9600);
}

void loop() // run over and over
{
    portOne.listen();
    Serial.println("Data from port one:");
    // while there is data coming in, read it
    // and send to the hardware serial port:
    while (portOne.available() > 0) {
        int inByte = portOne.read();
        Serial.print(inByte);
    }
}