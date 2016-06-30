//
// Created by Angel Zaprianov on 30.6.2016 г..
//
#ifndef ARDUINOMID_LCDCHAR_H
#define ARDUINOMID_LCDCHAR_H
//
// LCD Character handler
void setupLcdChar()
{
//
// Smile face
  byte CHR_SML[8] = {
      B00000,
      B10001,
      B00000,
      B00000,
      B10001,
      B01110,
      B00000
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

  lcd.createChar (0, CHR_SML);
  lcd.createChar (1, CHR_CLS);
  lcd.createChar (2, CHR_MNC1);
  lcd.createChar (3, CHR_MNC2);
}
#endif //ARDUINOMID_LCDCHAR_H
