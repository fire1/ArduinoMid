//
// Created by Angel Zaprianov on 8.2.2017 г..
//

#ifndef ARDUINOMID_LCD240X64_H
#define ARDUINOMID_LCD240X64_H

#include <Arduino.h>
#include <U8g2lib.h>

class Lcd240x62 {

private:
    unsigned int aniIndex;
    void setTextMode(void);

    void setCharMode(void);


public:
    void aniHrzChar( u8g2_uint_t x, u8g2_uint_t y, const char *str);

};


void Lcd240x62::aniHrzChar( u8g2_uint_t x, u8g2_uint_t y, const char *str) {
    lcd.drawUTF8(aniIndex * 3, 36, str);
}

void Lcd240x62::setTextMode(void) {
    //
    // u8g2_font_4x6_tf, u8g2_font_5x7_tf
    lcd.setFont(u8g2_font_6x10_tf);
    lcd.setFontRefHeightExtendedText();
    lcd.setDrawColor(1);
    lcd.setFontPosTop();
    lcd.setFontDirection(0);
}

void Lcd240x62::setCharMode(void) {
    lcd.drawStr(0, 0, "Unicode");
    // u8g_font_04b_03b
    lcd.setFont(u8g2_font_unifont_t_symbols);
    lcd.setFontPosTop();
}

#endif //ARDUINOMID_LCD240X64_H
