//
// Created by Angel Zaprianov on 30.6.2016 г..
//
#ifndef ARDUINOM_MID_LCD_CHAR_H
#define ARDUINOM_MID_LCD_CHAR_H

#include <Arduino.h>
#include <LiquidCrystal.h>


//
// LCD Character handler
void setLcdBaseChar(LiquidCrystal *lcd) {
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
    byte chrAlert[8] = {
            0b00000,
            0b01110,
            0b11011,
            0b11011,
            0b11111,
            0b11011,
            0b01110,
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
    byte kilometers[8] = {
            0b10100,
            0b11000,
            0b10100,
            0b00000,
            0b01010,
            0b10101,
            0b10101,
            0b00000
    };

    byte snowflake[8] = {
            0b00000,
            0b10101,
            0b01110,
            0b11011,
            0b01110,
            0b10101,
            0b00000,
            0b00000
    };

    byte number100[8] = {
            0b00100,
            0b01010,
            0b00100,
            0b00000,
            0b00100,
            0b01010,
            0b00100,
            0b01110
    };

    lcd->createChar(0, number100);
    lcd->createChar(1, chrCelsius);
    lcd->createChar(2, kilometers);
//    lcd->createChar(3, chrAlert);
    lcd->createChar(3, number100);
    lcd->createChar(4, chrGasMark);
    lcd->createChar(5, chrInstCons1);
    lcd->createChar(6, chrInstCons2);
    lcd->createChar(7, chrAllCons1);
    lcd->createChar(8, chrAllCons2);
//    lcd->createChar(9, snowflake);

}

#endif //ARDUINOMID_LCDCHAR_H
