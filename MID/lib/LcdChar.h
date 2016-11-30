//
// Created by Angel Zaprianov on 30.6.2016 г..
//
#ifndef ARDUINOMID_LCDCHAR_H
#define ARDUINOMID_LCDCHAR_H

#include <USBAPI.h>

//
// LCD Character handler
void setLcdBaseChar() {
//
// Temperature celsius
    byte chrCelsius[8] = {
            0b01000,
            0b10100,
            0b01000,
            0b00010,
            0b00100,
            0b00100,
            0b00010,
            0b00000
    };

    //
    // Check
    byte chrCheck[8] = {
            0b00000,
            0b00000,
            0b00000,
            0b00000,
            0b00001,
            0b01010,
            0b00100,
            0b00000
    };
    //
    // Menu gas marker
    byte chrGasMark[8] = {
            0b00000,
            0b01110,
            0b01011,
            0b01010,
            0b01111,
            0b01111,
            0b01110,
            0b11111
    };


    byte chrInstCons1[8] = {
            0b00000,
            0b00000,
            0b10000,
            0b00010,
            0b10101,
            0b10101,
            0b10101,
            0b00000
    };
    byte chrInstCons2[8] = {
            0b00000,
            0b00000,
            0b00000,
            0b01000,
            0b10000,
            0b01000,
            0b10000,
            0b00000
    };

    byte chrAllCons1[8] = {
            0b00000,
            0b00000,
            0b00000,
            0b01001,
            0b10101,
            0b11101,
            0b10101,
            0b00000
    };
    byte chrAllCons2[8] = {
            0b00000,
            0b00000,
            0b00000,
            0b01000,
            0b01000,
            0b01000,
            0b01000,
            0b00000
    };


    byte chrBertone[8] = {
            0b11000,
            0b01000,
            0b01100,
            0b01010,
            0b01010,
            0b01010,
            0b00100,
            0b00000
    };


    lcd.createChar(1, chrCelsius);
    lcd.createChar(2, chrBertone);
    lcd.createChar(3, chrCheck);
    lcd.createChar(4, chrGasMark);
    lcd.createChar(5, chrInstCons1);
    lcd.createChar(6, chrInstCons2);
    lcd.createChar(7, chrAllCons1);
    lcd.createChar(8, chrAllCons2);

}

#endif //ARDUINOMID_LCDCHAR_H
