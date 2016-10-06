
// include the SPI library:
#include <SPI.h>
#define potInputPin 0


// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 10;
int potValue;


void setup() {
    // start serial port at 9600 bits per second
    Serial.begin(9600);

    // clear the serial port buffer
    Serial.flush();
// set the slaveSelectPin as an output:
    pinMode (slaveSelectPin, OUTPUT);
    // initialize SPI:
    SPI.begin();
}

int digitalPotWrite(int command, int value) {
    // take the SS pin low to select the chip:
    digitalWrite(slaveSelectPin,LOW);
    //  send in the value via SPI:
    SPI.transfer(command);
    SPI.transfer(value);
    // take the SS pin high to de-select the chip:
    digitalWrite(slaveSelectPin,HIGH);
}

void printNum(int Value) {
    int Thous = char(Value/1000);
    Serial.write(Thous+48);
    Value = Value - Thous * 1000;
    int Huns = char(Value/100);
    Serial.write(Huns+48);
    Value = Value - Huns * 100;
    int Tens = char(Value/10);
    Serial.write(Tens+48);
    Value = Value - Tens * 10;
    int Ones = char(Value);
    Serial.write(Ones+48);
}

void loop() {
    // change the resistance from min to max:
    for (int level = 0; level < 255; level = level + 1) {
        digitalPotWrite(0, level);
        Serial.print("Pot Level:  ");
        printNum(level);
        Serial.print("\t");
        delay(100);
        potValue = analogRead(potInputPin);
        Serial.print("Pot Value:  ");
        printNum(potValue);
        Serial.print("\n");
    }

    // wait a second at the top:
    Serial.print("\n");
    delay(100);

    // change the resistance from max to min:
    for (int level = 0; level < 255; level = level + 1) {
        digitalPotWrite(0, 255 - level);
        Serial.print("Pot Level:  ");
        printNum(255 - level);
        Serial.print("\t");
        delay(100);
        potValue = analogRead(potInputPin);
        Serial.print("Pot Value:  ");
        printNum(potValue);
        Serial.print("\n");
    }
    delay(30000);
}
