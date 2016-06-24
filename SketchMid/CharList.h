//
// Created by fire1 on 6/13/2016.
//

#ifndef ARDUINOMID_CHARLIST_H
#define ARDUINOMID_CHARLIST_H

void initChars() {
     byte smiley[8] = {
  B00000,
  B10001,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
};

byte clelsi[8] = {
  0b00010,
  0b00101,
  0b00010,
  0b01000,
  0b10100,
  0b10000,
  0b10100,
  0b01000
};

byte menuCurA[8] = {
  0b00000,
  0b11111,
  0b10001,
  0b10001,
  0b10001,
  0b11111,
  0b00000,
  0b00000
};

byte menuCurB[8] = {
  0b00000,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b11111,
  0b00000,
  0b00000
};


     lcd.createChar(0, smiley);
     lcd.createChar(1, clelsi);
     lcd.createChar(2, menuCurA);
     lcd.createChar(3, menuCurB);
}

#endif //ARDUINOMID_CHARLIST_H
