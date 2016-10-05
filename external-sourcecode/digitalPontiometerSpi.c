/*
The circuit:
  * CS  (1)     - to digital pin 53     (SS pin)
  * SI  (3)     - to digital pin 51     (MOSI pin)
  * CLK (2)     - to digital pin 52     (SCK pin)
  * */
// inslude the SPI library:
#include <SPI.h>

// set pin 10 as the slave select for the digital pot:
const int slaveSelectPin = 53;
//int channel = 0;
//int pot = 0;
//int level = 0;

void setup() {
    Serial.begin(9600);
// set the slaveSelectPin as an output:
    pinMode (slaveSelectPin, OUTPUT);
//  pinMode (SPICCLOCK, OUTPUT);//Needed to be defined?
//  pinMode (SLAVESELECT,OUTPUT); //same as above?
// initialize SPI:
    SPI.begin();
    SPI.setDataMode(SPI_MODE0);//SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3
    SPI.setBitOrder(MSBFIRST); // LSBFIRST or MSBFIRST

}

void loop() {
    delay(10);
    digitalWrite(slaveSelectPin,LOW);
    SPI.transfer(B10001);
    SPI.transfer(byte(128));
    digitalWrite(slaveSelectPin,HIGH);
}
