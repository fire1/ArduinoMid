

#include <Arduino.h>


const uint8_t pinLpgDat = A1;     //  [brown]     Switch DATA     Tank fuel level     /// A8
const uint8_t pinLpgClc = A2;     //  [blue]      Switch button   Fuel switcher       /// A9

#define LPG_SENS_MESSAGE_LENGTH 8


#define LPG_SENS_MESSAGE_HEADER 111111101100000011111100


struct LpgStram {
    String head;
    String data;
};


boolean is_lpg_switch(String buffer) {

}


void setup() {
    Serial.begin(250000);

    Serial.println("Start LPG listening .... ");
};

boolean startReceiving = false;
int indexReceiving = 0;
uint8_t receivedBuffer, receivingData;

char bit_string[8];


void loop() {


    if (digitalRead(pinLpgClc) == LOW && !startReceiving) {
        startReceiving = true;
    }


    if (indexReceiving < LPG_SENS_MESSAGE_LENGTH && startReceiving) {
        receivedBuffer |= (digitalRead(pinLpgDat) & 0x01) << indexReceiving;
//        receivedBuffer |= digitalRead(pinLpgDat) << indexReceiving;
        bit_string[indexReceiving] |= digitalRead(pinLpgDat) << indexReceiving;

//        Serial.print(digitalRead(pinLpgDat), HEX);
        indexReceiving++;
    }



    if (indexReceiving > LPG_SENS_MESSAGE_LENGTH) {
        receivingData = receivedBuffer;
        receivedBuffer = 0;
        indexReceiving = 0;
        startReceiving = 0;
    }


    if (receivingData && receivingData < 255) {

//        char bit_string[] = "01001000"; // a single byte in string representation, visible value 48Hex
        char dest_char = 0; // the output, 'H' -- must initialize all bits to 0
        for (int source_bit_pos = 8; source_bit_pos >= 0; source_bit_pos--) // start from rightmost position
        {
            int the_bit = bit_string[source_bit_pos] - '0'; // convert from char representation '0', '1' to 0, 1
            dest_char |= the_bit << (7 - source_bit_pos);
        }

        Serial.print(receivingData, BIN);
        Serial.print("\t");
//        Serial.write(((receivingData >> 8) & 0xff));
        Serial.print(receivingData, HEX);
        receivingData = 0;
        Serial.print("\n");
    }

};
