//#include <USBAPI.h>

//
// Created by Angel Zaprianov on 28.6.2016 г..
//
// Since EEPROM have live time will use external-sourcecode ...
// http://www.hobbytronics.co.uk/arduino-external-eeprom
//
// Data container
//#include <EEPROM.h>
#include <I2C_eeprom.h>
//
// http://playground.arduino.cc/Main/LibraryForI2CEEPROM

//#define EE24LC01MAXBYTES
//#define DEVICEADDRESS (0x50)



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
const int EEP_ADR_TC2 = 8; // Gas tank current liters
//
// We’re using a 256kbit eeprom which is actually 32kbytes of space.
//      262,144 bits / 8 bits in a byte = 32,768 bytes.
//      That’s 62 times the Arduino’s built-in storage!
I2C_eeprom diskStorage((0x50), 32768);

//
//
class EepRom {

public:
    void setup() { diskStorage.begin(); };

    void saveFuelTankLevel(unsigned int value = 0) {
        diskStorage.writeByte(EEP_ADR_FTK, value);
    };

    int loadFuelTankLevel() {
        return diskStorage.readByte(EEP_ADR_FTK);
    };

    void saveTravelDistance(unsigned int value = 0) {
        diskStorage.writeByte(EEP_ADR_TTD, value / 4);
    }

    int long loadTravelDistance() {
        return diskStorage.readByte(EEP_ADR_TTD) * 4;
    }

    void saveTripDistance(unsigned int value = 0) {
        diskStorage.writeByte(EEP_ADR_TRD, value / 4);
    }

    int long loadTripDistance(unsigned int value = 0) {
        return diskStorage.readByte(EEP_ADR_TRD) * 4;
    }

private:

};
