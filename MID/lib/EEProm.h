//#include <USBAPI.h>

//
// Created by Angel Zaprianov on 28.6.2016 г..
//
// Since EEPROM have live time will use external-sourcecode ...
// http://www.hobbytronics.co.uk/arduino-external-eeprom

// http://playground.arduino.cc/Main/LibraryForI2CEEPROM
#include <Wire.h>
#include "MainFunc.h"

#define EEP_ROM_ADDRESS 0x50    //Address of 24LC256 eeprom chip


//
//  Mega2560
// 	20 (SDA), 21 (SCL)
//
// Saves Addresses in EEP Rom
const int EEP_ADR_FTK = 0; // Fuel tank Astra G -  52 liter 14 gallons
const int EEP_ADR_TTD = 1; // Total Travel distance
const int EEP_ADR_TRD = 2; // Trip distance
const int EEP_ADR_TR2 = 3; // Trip distance
const int EEP_ADR_TTT = 4; // Total Trip Time
const int EEP_ADR_TER = 4; // Time Engine Run
const int EEP_ADR_TRS = 5; // Tires size
const int EEP_ADR_RMS = 6; // Rims Size
const int EEP_ADR_TC1 = 7; // Gas tank size
const int EEP_ADR_TC2 = 8; // Travel Consumption
//
// We’re using a 256kbit eeprom which is actually 32kbytes of space.
//      262,144 bits / 8 bits in a byte = 32,768 bytes.
//      That’s 62 times the Arduino’s built-in storage!




//
//
class EepRom {

public:
    void setup() { Wire.begin(); };

    void saveFuelTankLevel(unsigned int value = 0) {
        WireEepRomWriteByte(EEP_ADR_FTK, value);
    };

    int loadFuelTankLevel() {
        WireEepRomRead(EEP_ADR_FTK);
    };

    void saveTravelDistance(unsigned int value = 0) {
        WireEepRomWriteByte(EEP_ADR_TTD, value / 4);
    }

    int long loadTravelDistance() {
        return WireEepRomRead(EEP_ADR_TTD) * 4;
    }

    /**
     *  Saves travel consumption
     */
    void saveTravelConsumption(float value = 0) {
        int val[1];

        separateFloat(value, val);

        WireEepRomWriteByte(EEP_ADR_TC1, val[0]);
        WireEepRomWriteByte(EEP_ADR_TC2, val[1]);
    }

    /**
     *  Loads travel consumption
     */
    float loadTravelConsumption() {

        int va1 = WireEepRomRead(EEP_ADR_TC1);
        int va2 = WireEepRomRead(EEP_ADR_TC2);

        return restoreFloat(va1, va2);
    }

    void saveTripDistance(unsigned int value = 0) {
        WireEepRomWriteByte(EEP_ADR_TRD, value / 4);
    }

    int long loadTripDistance(unsigned int value = 0) {
        return WireEepRomRead(EEP_ADR_TRD) * 4;
    }

    void saveCurrentData();

    void loadCurrentData();

private:
    byte noElem = 12;
    unsigned int baseAddress = 0;

    void WireEepRomWriteByte(uint16_t theMemoryAddress, int u8Byte) {

        Wire.beginTransmission(EEP_ROM_ADDRESS);
        Wire.write((theMemoryAddress >> 8) & 0xFF);
        Wire.write((theMemoryAddress >> 0) & 0xFF);
        Wire.write(u8Byte);
        Wire.endTransmission();
        delay(6);

    }


    uint8_t WireEepRomRead(uint16_t theMemoryAddress) {
        uint8_t u8retVal = 0;
        Wire.beginTransmission(EEP_ROM_ADDRESS);
        Wire.write((theMemoryAddress >> 8) & 0xFF);
        Wire.write((theMemoryAddress >> 0) & 0xFF);
        Wire.endTransmission();
        delay(5);
        Wire.requestFrom(EEP_ROM_ADDRESS, 1);
        u8retVal = Wire.read();
        return u8retVal;
    }

//    template<class T>
//    int EEPROM_writeAnything(int ee, const T &value) {
//        const byte *p = (const byte *) (const void *) &value;
//        int i;
//        for (i = 0; i < sizeof(value); i++)
//            WireEepRomWriteByte(ee++, *p++);
//        return i;
//    }
//
//    template<class T>
//    int EEPROM_readAnything(int ee, T &value) {
//        byte *p = (byte *) (void *) &value;
//        int i;
//        for (i = 0; i < sizeof(value); i++) {
//            *p++ = WireEepRomRead(ee++);
//        }
//        return i;
//    }


};


void EepRom::saveCurrentData() {

    saveTravelConsumption(TTL_TLH + CUR_TLH);

/*
    double dataInt[noElem] = {TTL_TLH + CUR_TLH,};

    unsigned int n = 0;
    //
    // write data to eepRom
    for (int i = 0; i <= noElem - 1; i++) {
        n = EEPROM_writeAnything((i * 4) + baseAddress, dataInt[i]);
    }
*/

}


void EepRom::loadCurrentData() {


    TTL_TLH = loadTravelConsumption();
    /*
    unsigned int n = 0;
    // read data back
    for (int i = 0; i <= noElem - 1; i++) {
        double val;
        int address = (i * 4) + baseAddress;
        n = EEPROM_readAnything(address, val);
        address +=n;
        Serial.print("Restored ");
        Serial.println(val);
    }

    // read data back in separate temporary buffer
    double scratch[countof(testInt)];
    int n = EEPROM_readAnything(baseAddr, scratch);
    for (int i = 0; i < countof(scratch); i++)
        Serial.println(scratch[i]);
        */



}

/*
void writeBigEEPROM(unsigned int deviceaddress, double  *str_data) {
    // Work out length of data
    char str_len = 0;
    do { str_len++; } while (str_data[str_len]);

    // Write out data several times consecutively starting at address 0
    for (i = 0; i < WRITE_CNT; i++) writeEEPROM(deviceaddress, i * str_len, str_data);
}


void writeEEPROM(int deviceaddress, unsigned int eeaddress, double  *data) {

    // Uses Page Write for 24LC256
    // Allows for 64 byte page boundary
    // Splits string into max 16 byte writes
    unsigned char i = 0, counter = 0;
    unsigned int address;
    unsigned int page_space;
    unsigned int page = 0;
    unsigned int num_writes;
    unsigned int data_len = 0;
    unsigned char first_write_size;
    unsigned char last_write_size;
    unsigned char write_size;

    // Calculate length of data
    do { data_len++; } while (data[data_len]);

    // Calculate space available in first page
    page_space = int(((eeaddress / 64) + 1) * 64) - eeaddress;

    // Calculate first write size
    if (page_space > 16) {
        first_write_size = page_space - ((page_space / 16) * 16);
        if (first_write_size == 0) first_write_size = 16;
    }
    else
        first_write_size = page_space;

    // calculate size of last write
    if (data_len > first_write_size)
        last_write_size = (data_len - first_write_size) % 16;

    // Calculate how many writes we need
    if (data_len > first_write_size)
        num_writes = ((data_len - first_write_size) / 16) + 2;
    else
        num_writes = 1;

    i = 0;
    address = eeaddress;
    for (page = 0; page < num_writes; page++) {
        if (page == 0) write_size = first_write_size;
        else if (page == (num_writes - 1)) write_size = last_write_size;
        else write_size = 16;

        Wire.beginTransmission(deviceaddress);
        Wire.write((int) ((address) >> 8));   // MSB
        Wire.write((int) ((address) & 0xFF)); // LSB
        counter = 0;
        do {
            Wire.write((byte) data[i]);
            i++;
            counter++;
        } while ((data[i]) && (counter < write_size));
        Wire.endTransmission();
        address += write_size;   // Increment address for next write

        delay(6);  // needs 5ms for page write
    }
}

void readEEPROM(int deviceaddress, unsigned int eeaddress, unsigned char *data, unsigned int num_chars) {
    unsigned char i = 0;
    Wire.beginTransmission(deviceaddress);
    Wire.write((int) (eeaddress >> 8));   // MSB
    Wire.write((int) (eeaddress & 0xFF)); // LSB
    Wire.endTransmission();

    Wire.requestFrom(deviceaddress, num_chars);

    while (Wire.available()) data[i++] = Wire.read();

}*/
