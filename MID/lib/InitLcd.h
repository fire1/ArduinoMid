//
// Created by Angel Zaprianov on 3.4.2017 г..
//

#ifndef ARDUINOMID_SCREENS_H
#define ARDUINOMID_SCREENS_H

#include "InitObj.h"
#include "LcdPwr.h"


#if SCREEN == 162 || !defined(SCREEN)

#include <LiquidCrystal.h>
#include "displays/Lcd16x2.h"

//
// This class somehow fixes unexpected
// reboot from Mid plug dim value. (in my case)
//#include <Firmata.h>
Menu16x2 midMenu;
//
//
MenuBase menuBase(btnMenu, midMenu);
//
// Resolving power pins from Mega2560's socket
LcdPwr lcdPwr(DSP_PIN_GD1, DSP_PIN_VCC, DSP_PIN_LDK, DSP_PIN_LDA, DSP_PIN_WR);
//
// Creates an LC object. Parameters: (rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(32, 33, 34, 35, 36, 37);
//
//
Lcd16x2 lcdMenu(lcd, btnMenu, menuBase,/* &carGames, */shutDown);

#elif SCREEN == 24064

/*
     analogWrite(26, 0); // GND
    analogWrite(27, 0); // GND
    analogWrite(29, 255); // 5V
    analogWrite(45, 0); // fs
    digitalWrite(30, HIGH); // RD
    analogWrite(46, 0); // back light
    analogWrite(47, 255); // back light
 */

#include <U8g2lib.h>
#include "displays/Lcd240x64.h"

Menu240x60 midMenu;

MenuBase menuBase(btnMenu, midMenu);
/*
1   |   GND   |   power GND
2   |   GND   |   Power GND
3   |   VCC   |   Power positive
4   |   V0    |   Negative Voltage (LCD contrast)
5   |   WR    |   Write Signal                          |
6   |   RD    |   Read Signal
7   |   CE    |   Chip Enable Signal
8   |   CD    |   H: Data L:  Instruction Code          |       DC
9   |   NC    |   No connection
10  |   RST   |   Reset Signal Input
11  /   18    |   DB0..DB7 Data Bus Line
19  |   FS    |   Font selection
20  |   VEE   |   Negative Voltage Supply
21  |   LED_A |   Blacklight Anode (+5V)
22  |   LED_K |   Blacklight cathode (0v)
 */
LcdPwr lcdPwr = LcdPwr(DSP_PIN_GD1, DSP_PIN_VCC, DSP_PIN_LDK, DSP_PIN_LDA, DSP_PIN_RD, DSP_PIN_FS, DSP_PIN_GD2);
//
// Check https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for display setup
U8G2_T6963_240X64_2_8080 // Screen constructor
        u8g2(U8G2_R0,
             DSP_PIN_DT1,
             DSP_PIN_DT2,
             DSP_PIN_DT3,
             DSP_PIN_DT4,
             DSP_PIN_DT5,
             DSP_PIN_DT6,
             DSP_PIN_DT7,
             DSP_PIN_DT8,
        /*WR*/ DSP_PIN_WR, /*CE*/ DSP_PIN_CE, /*dc8*/DSP_PIN_CD, /*RST*/ DSP_PIN_RST);
//U8G2_T6963_240X64_2_8080 u8g2(U8G2_R0, 8, 9, 10, 11, 4, 5, 6, 7,/*WR*/ 14, /*CE*/ 16, /*dc8*/17, /*RST*/ 18); // Connect RD with +5V, FS0 and FS1 with GND
Lcd240x62 lcdMenu(u8g2, btnMenu, menuBase, shutDown);
#endif

//
// Event method set
void menuChangeEvent(MenuChangeEvent changed) {
    midMenu.menuChanged(changed);
}

void menuUseEvent(MenuUseEvent used) {
    Serial.print(F(" Stage 1 free RAM (menuUseEvent): "));
    Serial.println(getFreeRam());
    usedMenu.used = used.item.getName();

#if SCREEN != 162
    usedMenu.back = used.item.getBack()->getName();
    usedMenu.last = used.item.getAfter()->getAfter()->getName();
    usedMenu.down = used.item.getRight()->getName();
    usedMenu.used = used.item.getName();
    usedMenu.next = used.item.getAfter()->getName();
#endif
//
//    Serial.print(F(" Stage 2 free RAM (menuUseEvent):"));
//    Serial.println(getFreeRam());
}


#endif //ARDUINOMID_SCREENS_H
