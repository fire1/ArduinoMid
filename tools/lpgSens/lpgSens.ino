

#include <Arduino.h>


const uint8_t pinLpgDat = A1;     //  [brown]     Switch DATA     Tank fuel level     /// A8
const uint8_t pinLpgClc = A2;     //  [blue]      Switch button   Fuel switcher       /// A9

#define LPG_SENS_MESSAGE_LENGTH 8


struct LpgStram {
    String head;
    String data;
};

//Note:
//        Not all pins on the Mega and Mega 2560 support change interrupts,
//so only the following can be used for RX:
// 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69


uint8_t shiftListenIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i) {
        digitalWrite(clockPin, HIGH);
        if (bitOrder == LSBFIRST)
            value |= digitalRead(dataPin) << i;
        else
            value |= digitalRead(dataPin) << (7 - i);
        digitalWrite(clockPin, LOW);
    }
    return value;
}

void setup() {
    Serial.begin(250000);
//    portOne.begin(9600);
    Serial.println("Start LPG listening .... ");
};

boolean startClockState = false;
boolean startDataState = false;
int indexReceiving = 0;
uint8_t receivedBuffer, receivingData;

char bit_string[8];


void loop() {

//    portOne.listen();
//    while (portOne.available() > 0) {
//        //
//        //
//        Serial.print("Receiving serial ... ");
//        Serial.write(portOne.read());
//        Serial.println("\n");
//    }



    if (digitalRead(pinLpgClc) == LOW) {
        digitalWrite(13, HIGH);
    } else {
        digitalWrite(13, LOW);
    }


    if (digitalRead(pinLpgClc) == LOW && !startClockState) {
        startClockState = true;
    }

    if (digitalRead(pinLpgDat) == LOW && startClockState && startDataState == 0) {
        startDataState = true;
    }


    if (indexReceiving < LPG_SENS_MESSAGE_LENGTH && digitalRead(pinLpgDat) && startDataState) {
        receivedBuffer |= (digitalRead(pinLpgClc) & 0x01) << indexReceiving;
//        receivedBuffer |= digitalRead(pinLpgDat) << indexReceiving;
//        bit_string[indexReceiving] |= digitalRead(pinLpgDat) << indexReceiving;

//        Serial.print(digitalRead(pinLpgDat), HEX);
        indexReceiving++;
    }


    if (indexReceiving > LPG_SENS_MESSAGE_LENGTH) {
        receivingData = receivedBuffer;
        receivedBuffer = 0;
        indexReceiving = 0;
        startClockState = false;
    }


    if (receivingData && receivingData < 255) {

//        char bit_string[] = "01001000"; // a single byte in string representation, visible value 48Hex
//        char dest_char = 0; // the output, 'H' -- must initialize all bits to 0
//        for (int source_bit_pos = 8; source_bit_pos >= 0; source_bit_pos--) // start from rightmost position
//        {
//            int the_bit = bit_string[source_bit_pos] - '0'; // convert from char representation '0', '1' to 0, 1
//            dest_char |= the_bit << (7 - source_bit_pos);
//        }

        Serial.print(receivingData, BIN);
        Serial.print("\t\t\t\t");
//        Serial.write(((receivingData >> 8) & 0xff));
//        Serial.print(receivingData, HEX);
        receivingData = 0;
        Serial.print("\n");
    }

};
