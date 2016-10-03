//#include <USBAPI.h>

//
// Created by Angel Zaprianov on 28.6.2016 г..
//
// Since EEPROM have live time will use external-sourcecode ...
// http://www.hobbytronics.co.uk/arduino-external-eeprom

// http://playground.arduino.cc/Main/LibraryForI2CEEPROM
#include <Wire.h>

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
const int EEP_ADR_GTS = 7; // Gas tank size
const int EEP_ADR_GTL = 8; // Travel Consumption
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

    void saveTravelConsumption(float value = 0) {
        if (value < 1) {
            return;
        }

        WireEepRomWriteByte(EEP_ADR_GTL, int(value * 10));
    }

    float loadTravelConsumption() {
        int val = WireEepRomRead(EEP_ADR_GTL);
        if (val > 100) {
            val = val / 10;
        }
        return val;
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

    void WireEepRomWriteByte(uint16_t theMemoryAddress, uint8_t u8Byte) {

        Wire.beginTransmission(EEP_ROM_ADDRESS);
        Wire.write((theMemoryAddress >> 8) & 0xFF);
        Wire.write((theMemoryAddress >> 0) & 0xFF);
        Wire.write(u8Byte);
        Wire.endTransmission();
        delay(2);

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

    template<class T>
    int EEPROM_writeAnything(int ee, const T &value) {
        const byte *p = (const byte *) (const void *) &value;
        int i;
        for (i = 0; i < sizeof(value); i++)
            WireEepRomWriteByte(ee++, *p++);
        return i;
    }

    template<class T>
    int EEPROM_readAnything(int ee, T &value) {
        byte *p = (byte *) (void *) &value;
        int i;
        for (i = 0; i < sizeof(value); i++) {
            *p++ = WireEepRomRead(ee++);
        }
        return i;
    }


};


void EepRom::saveCurrentData() {

/*
    double dataInt[noElem] = {TTL_TLH + CUR_TLH,};

    unsigned int n = 0;
    //
    // write data to eepRom
    for (int i = 0; i <= noElem - 1; i++) {
        n = EEPROM_writeAnything((i * 4) + baseAddress, dataInt[i]);
    }
*/
    saveTravelConsumption(TTL_TLH + CUR_TLH);
}


void EepRom::loadCurrentData() {

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
    TTL_TLH = loadTravelConsumption();
}

