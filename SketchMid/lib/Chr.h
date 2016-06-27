//
// Created by fire1 on 6/13/2016.
//

#ifndef ARDUINOMID_Chr_H
#define ARDUINOMID_Chr_H

//
// Smile
byte CHR_SML[8] = {
        B00000,
        B10001,
        B00000,
        B00000,
        B10001,
        B01110,
        B00000,
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


#endif //ARDUINOMID_CHARLIST_H
