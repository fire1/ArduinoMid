#include <USBAPI.h>

//
// Created by Angel Zaprianov on 28.6.2016 г..
//
// Since EEPROM have live time will use external-sourcecode ...
// http://www.hobbytronics.co.uk/arduino-external-eeprom
//
// Data container
//#include <EEPROM.h>
//#include <Wire.h>   // External I2C EEPROM to Arduino
//
// Saves Addresses in EEP Rom
const int EEP_ADR_FTK = 0; // Fuel tank Astra G -  52 liter 14 gallons
const int EEP_ADR_TT1 = 1; // Total Travel distance
const int EEP_ADR_TT2 = 2; // Total Travel distance
const int EEP_ADR_TR1 = 2; // Trip distance
const int EEP_ADR_TR2 = 3; // Trip distance
const int EEP_ADR_TTT = 4; // Total Trip Time
const int EEP_ADR_TER = 4; // Time Engine Run
const int EEP_ADR_TRS = 5; // Tires size
const int EEP_ADR_RMS = 6; // Rims Size
const int EEP_ADR_GTS = 7; // Gas tank size
const int EEP_ADR_GTL = 8; // Gas tank current liters
//
//
byte readEEPROM(int deviceaddress, unsigned int eeaddress )
{
  byte rdata = 0xFF;

  Wire.beginTransmission(deviceaddress);
  Wire.send((int)(eeaddress >> 8));   // MSB
  Wire.send((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();

  Wire.requestFrom(deviceaddress,1);

  if (Wire.available()) rdata = Wire.receive();

  return rdata;
}
//
//
void writeEEPROM(int deviceaddress, unsigned int eeaddress, byte data )
{
  Wire.beginTransmission(deviceaddress);
  Wire.send((int)(eeaddress >> 8));   // MSB
  Wire.send((int)(eeaddress & 0xFF)); // LSB
  Wire.send(data);
  Wire.endTransmission();

  delay(5);
}

/**
 * Saves fuel tank level
 */
void saveFuelTankLevel (unsigned int value = 0)
{
  EEPROM.write (EEP_ADR_FTK, value);
}
/**
 * Loads saved fuel level
 */
int loadFuelTankLevel ()
{
  return EEPROM.read (EEP_ADR_FTK);
}
/**
 * Save total travel distance
 */
void saveTravelDistance (unsigned int value = 0)
{
  EEPROM.write (EEP_ADR_TTD, value / 4);
}
/**
 * Loads travel distance
 */
void loadTravelDistance ()
{
  return EEPROM.read (EEP_ADR_TTD) * 4;
}
/**
 *
 */
void saveTripDistance (unsigned int value = 0)
{
  EEPROM.write (EEP_ADR_TRD, value / 4);
}

