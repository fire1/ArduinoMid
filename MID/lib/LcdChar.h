//
// Created by Angel Zaprianov on 30.6.2016 г..
//
#ifndef ARDUINOMID_LCDCHAR_H
#define ARDUINOMID_LCDCHAR_H

#include <USBAPI.h>

//
// LCD Character handler
void setupLcdChar() {
//
// Smile face
    byte CHR_SML[8] =  {
            0b00000,
            0b00000,
            0b00010,
            0b01001,
            0b00001,
            0b01001,
            0b00010,
            0b00000
    };

//
// Temperature celsius
    byte CHR_CLS[8] = {
            0b00010,
            0b00101,
            0b00010,
            0b01000,
            0b10100,
            0b10000,
            0b10100,
            0b01000
    };
//
// Menu empty marker
    byte CHR_MNC1[8] = {
            0b00000,
            0b11111,
            0b10001,
            0b10001,
            0b10001,
            0b11111,
            0b00000,
            0b00000
    };
//
// Menu active marker
    byte CHR_MNC2[8] = {
            0b00000,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b11111,
            0b00000,
            0b00000
    };
//
// Menu gas marker
    byte CHR_CONS[8] = {
            0b01101,
            0b10101,
            0b11110,
            0b10111,
            0b11111,
            0b11100,
            0b11110,
            0b00000
    };
    //
    // pomp1
    byte Pomp1[8] = {
            0b00000,
            0b00011,
            0b00010,
            0b00011,
            0b00011,
            0b00011,
            0b00011,
            0b00000
    };
    //
    // pomp 2
    byte Pomp2[8] = {
            0b00000,
            0b11000,
            0b01100,
            0b01010,
            0b11010,
            0b11100,
            0b11000,
            0b11100
    };
    //
    // Fi diameter
    byte Diameter[8] = {
            0b00000,
            0b01101,
            0b10010,
            0b10101,
            0b01001,
            0b10110,
            0b00000,
            0b00000
    };

    byte instCons1[8] = {
            0b00000,
            0b00000,
            0b10000,
            0b00010,
            0b10101,
            0b10101,
            0b10101,
            0b00000
    };
    byte instCons2[8] = {
            0b00000,
            0b00000,
            0b00000,
            0b01000,
            0b10000,
            0b01000,
            0b10000,
            0b00000
    };

    byte allCons1[8] = {
            0b00000,
            0b00000,
            0b00000,
            0b01001,
            0b10101,
            0b11101,
            0b10101,
            0b00000
    };
    byte allCons2[8] = {
            0b00000,
            0b00000,
            0b00000,
            0b01000,
            0b01000,
            0b01000,
            0b01000,
            0b00000
    };

    byte dspSpeed1[8] = {
            0b00000,
            0b00000,
            0b00000,
            0b01001,
            0b10010,
            0b01011,
            0b10010,
            0b00000
    };

    byte dspSpeed2[8] = {
            0b00000,
            0b00000,
            0b00000,
            0b00001,
            0b10011,
            0b00101,
            0b00011,
            0b00000
    };

    byte dspRpm1[8] = {
            0b00000,
            0b00000,
            0b00000,
            0b01000,
            0b10101,
            0b11001,
            0b10101,
            0b00000
    };

    lcd.createChar(9, CHR_SML);
    lcd.createChar(1, CHR_CLS);
    lcd.createChar(2, CHR_MNC1);
    lcd.createChar(3, CHR_MNC2);
    lcd.createChar(4, CHR_CONS);
    lcd.createChar(5, instCons1);
    lcd.createChar(6, instCons2);
    lcd.createChar(7, allCons1);
    lcd.createChar(8, allCons2);
//    lcd.createChar(9, dspSpeed1);
//    lcd.createChar(10, dspSpeed2);
//    lcd.createChar(11, dspRpm1);
}

#endif //ARDUINOMID_LCDCHAR_H
