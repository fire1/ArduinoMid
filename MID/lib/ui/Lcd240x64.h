//
// Created by Angel Zaprianov on 8.2.2017 г..
//
#ifndef ARDUINO_MID_LCD240X64_H
#define ARDUINO_MID_LCD240X64_H
//#include "CarGames.h"
#include <Arduino.h>
#include <U8g2lib.h>
#include "../AptService.h"

#ifndef _U8G2LIB_HH
// Some IDE syntax mishmash fixer
#include "../../libraries/U8g2/src/U8g2lib.h"

#endif

#include "Menu240x64.h"
#include "../../MID.h"
#include "../MainFunc.h"
#include "../ShutDw.h"
#include "../CarState.h"
#include "graphics/240x64-logo.h"
#include "graphics/gLcd-icons.h"
#include "fonts/OpelFontU8g.h"

#define DEFAULT_FONT u8g_font_opel_ic_13
#define GUIDANCE_FONT u8g_font_opel_ic_12
#define LCD_ROW_1 16
#define LCD_ROW_2 27
#define LCD_ROW_3 39
#define LCD_ROW_4 50

#define LCD_COL_L10 5
#define LCD_COL_L11 28
#define LCD_COL_L12 39
#define LCD_COL_L21 49
#define LCD_COL_L22 56
#define LCD_COL_L23 75

#define LCD_CNR 120 // Center of the screen
#define LCD_CNR_1 125 // Center of the screen
#define LCD_CNR_2 145 // Center of the screen

#define LCD_COL_R11 154
#define LCD_COL_R12 175
#define LCD_COL_R21 195
#define LCD_COL_R22 215
#define LCD_COL_R23 230

#define LCD_PADDING  5

#define LCD_ICO_HIGH 2
#define LCD_ENTRY_FRAME 2
const uint16_t INIT_TIME = 7000;


class Lcd240x62 : virtual public LcdUiInterface {

    U8G2 *lcd;

    //
    // Define base objects
    AmpTime *amp;
    CarSens *car;
    EepRom *eep;
    CarState *stt;
    WhlSens *whl;
    MenuBase *mbs;
    ShutDw *sdw;
    MenuBtn *btn;
    AptService *rtc;

    boolean animateFast = false;
    boolean animateUltra = false;
    boolean initializeDraw = false;
    boolean changedFont = false;
    char instantCons[3];

    const uint8_t *fontSelect;
    uint8_t lastValue;
//
// Drawing counter
    uint8_t drawIndex = 0;
    uint8_t drawEntry = 0;
    uint8_t tripActive = 1;
    uint8_t tripReset = 0;
    uint8_t wordWidth = 0;
    uint8_t offsetInf = 0;
    uint8_t sizeOfInf = 0;
    char valueCursor = 1;
    char valueComparator = 1;
    //
    // from 14 to 64
    uint8_t graphTest[10] = {54, 20, 48, 14, 64, 46, 18, 35, 15, 48};
    const uint8_t *currentFont = DEFAULT_FONT;

    uint8_t headerCursorDemo = 0;

public:
/**
 *
 * @param _lcd
 * @param _btn
 * @param _mbs
 * @param _sdw
 * @param _rtc
 */
    Lcd240x62(U8G2 &_lcd, MenuBtn &_btn, MenuBase &_mbs, ShutDw &_sdw, AptService &_rtc) :
            lcd(&_lcd), btn(&_btn), mbs(&_mbs), amp(_btn.passAmp()), car(_btn.passCar()), eep(_btn.passEep()),
            whl(_btn.passWhl()), stt(_btn.passStt()), sdw(&_sdw), rtc(&_rtc) {
    }


/**
 * Mid's intro
 */
    void intro(void) {
        //
        // Test tone
        tone(TONE_ADT_PIN, 400, 20);
        if (/*car->getRpm() < 699*/ false) {
            lcd->clearBuffer();
            lcd->firstPage();
            do {
                lcd->drawXBMP(0, 2, 240, 60, OpelLogoBits);
            } while (lcd->nextPage());
            delay(500);
            lcd->clearBuffer();

            lcd->firstPage();
            do {
                lcd->drawXBMP(0, 2, 240, 60, BertoneBits);
            } while (lcd->nextPage());
            delay(2550);
        }
        lcd->clear();


        tone(TONE_ADT_PIN, 800, 10);
    }

    void begin(void) {
        lcd->begin();
        useDefaultMode();
//        lcd->setAutoPageClear(1);
    }


/**
 *
 * @param col
 * @param row
 * @param before
 * @param tag
 */
    void showTag(uint8_t col, uint8_t row, const char *before, uint8_t tag, uint8_t padding = LCD_PADDING) {
        wordWidth = lcd->getStrWidth(before);
        lcd->drawStr(col + wordWidth + padding, row, getMsg(tag));
    }

/**
 *
 * @param col
 * @param row
 * @param before
 * @param data
 * @param padding
 */
    void showDin(uint8_t col, uint8_t row, const char *before, const char *data, uint8_t padding = LCD_PADDING) {
        wordWidth = lcd->getStrWidth(before);
        lcd->drawStr(col + padding + wordWidth, row, data);
    }

    /****************************************************************


/**
 * Draws shutdown begin for trip save
 */
    void drawShutdownBegin() {

    }

/**
 * Draws shutdown begin for trip save
 */
    void drawShutdownShort() {
        lcd->setCursor(15, LCD_ROW_1);
        lcd->print(this->getMsg(72));
        lcd->setCursor(15, LCD_ROW_2);
        lcd->print(this->getMsg(1));
    }

/**
 * Draws countdown time for saving trip
 */
    void drawShutdownCount(char sec[2]) {

    }

    /**
     *
     * @param text
     */
    void drawInfo(const __FlashStringHelper *text) {

        lcd->drawBox(0, LCD_ROW_4 - 1, 240, 14);
        lcd->setCursor(LCD_COL_L10, LCD_ROW_4);
        lcd->print(getMsg(106));
        lcd->setFont(GUIDANCE_FONT);
        lcd->setDrawColor(0);
        lcd->print(text);
        lcd->setFont(this->currentFont);
    }

    /**
     *
     * @param text
     */
    void drawInfoMsg(uint8_t offset) {

        lcd->drawBox(0, LCD_ROW_4 - 1, 240, 14);
        lcd->setCursor(LCD_COL_L10, LCD_ROW_4);
        lcd->setFont(DEFAULT_FONT);
        lcd->print(getMsg(106));
        lcd->setFont(GUIDANCE_FONT);
        lcd->setDrawColor(0);
        lcd->print(getMsg(offset));
        lcd->setFont(this->currentFont);
        lcd->setDrawColor(0);
    }

/**
 *
 * @param offsets
 * @param size Starting index  1
 */
    void drawInfoMsgMul(const uint8_t offsets[], uint8_t size) {
        sizeOfInf = size;
        drawInfoMsg(offsets[offsetInf]);
    }

    void listenInfoMsg() {
        if (amp->is2Seconds()) {
            offsetInf++;
        }
        if (offsetInf >= sizeOfInf)
            offsetInf = 0;
    }


/**
 *
 * @param icon
 */
    void drawHeaderIcon(uint8_t icon) {
        const char *ch = getMsg(icon);
        lcd->print(ch);
    }


    boolean isIconPulsing() {
        return drawIndex % 2 == 0;
    }

/**
 *
 * @param time
 * @return
 */
    boolean isIconDemo(const uint16_t time) {
        if (millis() < (INIT_TIME - time)) {
            return true;
        }
        return false;
    }

/**
 * Generate header part of screen menu
 * @param title
 */
    void showHeader(const char *title) {
        lcd->enableUTF8Print();
        lcd->drawLine(0, 13, 120, 13);
        lcd->drawLine(121, 12, 131, 2);
        lcd->drawLine(131, 2, 240, 2);
        lcd->setFont(u8g_font_opel_ic_12);
        lcd->setCursor(20, 1);
        lcd->print(title);



        //
        //  Clock in this place
        lcd->setCursor(131, 2);

#ifdef USE_CLOCK_MODULE
        lcd->print(rtc->getClock());
#endif


        //
        // Warnings
        lcd->setCursor(168, 2);
        if (car->getEngTmp() > ENGINE_OVERHEAT && isIconPulsing() || isIconDemo(200)) {
            drawHeaderIcon(75); // overheat
        }
        if (car->getRpm() > 5600 && isIconPulsing() || isIconDemo(800)) {
            drawHeaderIcon(80); // engine
        }
        if (stt->getVoltage() < 12 && car->isRunEng() && isIconPulsing() || isIconDemo(1200)) {
            drawHeaderIcon(104); // battery
        }
        if (stt->getLiveOil() && car->isRunEng() && isIconPulsing() || isIconDemo(1800)) {
            drawHeaderIcon(102); // battery
        }
        if (car->isEmgBreak()) { // emergency breaking
            drawHeaderIcon(73);
        }

        //
        // Media Futures
        lcd->setCursor(110, 2);
//        if (flashDrl) {
//            lcd->print(getMsg(69));
//        }
        uint8_t whlState = whl->getCurrentState();
        if (whlState == WhlSens::STR_BTN_VLU) {
            lcd->print(getMsg(90));
        } else if (whlState == WhlSens::STR_BTN_VLD) {
            lcd->print(getMsg(95));
        } else if (whlState != WhlSens::STR_BTN_NON) {
            lcd->print(getMsg(116));
        }


        //
        // Title icon
        lcd->setCursor(1, 1);
        if (stt->isAlert() && !stt->isWinter() || millis() < INIT_TIME && drawIndex % 2 == 0) {
            lcd->print(getMsg(84));
        } else if (millis() > INIT_TIME) {
            if (stt->isWinter()) {
                if (drawIndex % 2 == 0)lcd->print(getMsg(88));
            } else if (car->getTmpOut() < 0) {
                lcd->print(getMsg(88));
            } else if (car->getTmpOut() < 36) {
                (isAndroidConnected) ? lcd->print(getMsg(83)) : lcd->print(getMsg(86));
            } else if (car->getTmpOut() < 40) {
                lcd->print(getMsg(89));
            } else if (car->getTmpOut() > 40) {
                if (drawIndex % 2 == 0) lcd->print(getMsg(89));
            } else if (millis() > MILLIS_PER_HR * 3) {
                lcd->print(getMsg(81));
            } else if (millis() > MILLIS_PER_HR * 4) {
                if (drawIndex % 2 == 0)lcd->print(getMsg(81));
            }
        }

        lcd->setFont(u8g_font_opel_ic_13);
    }

/**
 * Icons for fuel switching BNZ
 */
    void showIconBnz(uint8_t x, uint8_t y) {
//        lcd->drawXBMP(x, y, 16, 16, bnz_16x16_bits);
        lcd->setCursor(x, y);
        lcd->print(getMsg(90));
    }

/**
 * Icons for fuel switching LPG
 */
    void showIconLpg(uint8_t x, uint8_t y) {
//        lcd->drawXBMP(x, y, 16, 16, lpg_16x16_bits);
        lcd->setCursor(x, y);
        lcd->print(getMsg(96));
    }


    void drawWarnMsg(uint8_t row1, uint8_t row2) {
        lcd->drawBox(80, LCD_ROW_3 - 1, 160, 26);
        lcd->setFont(u8g_font_opel_ic_12);
        lcd->setDrawColor(0);

        lcd->setCursor(85, LCD_ROW_3);
        lcd->print(this->getMsg(row1));
        lcd->setCursor(85, LCD_ROW_4);
        lcd->print(this->getMsg(row2));
        lcd->setFont(this->currentFont);
        lcd->setDrawColor(1);
    }


//
// Servicing screens
    void warnCoolant() override {
        if (amp->isRfr()) { //isRfr
            lcd->drawXBMP(0, 0, 85, 64, coolant_level_85x64_bits);
            lcd->setCursor(85, LCD_ROW_1);
            lcd->print(this->getMsg(49));
            drawWarnMsg(50, 51);
        }
    }

/**
 * Warning for battery with voltage
 * @param Voltage
 */
    void warnBattery(float Voltage) override {
        if (amp->isRfr()) { //isRfr
            lcd->drawXBMP(0, 0, 85, 64, low_battery_85x64_bits);
            lcd->setCursor(85, LCD_ROW_1);
            lcd->print(this->getMsg(43));
            lcd->setCursor(85, LCD_ROW_3);
            lcd->print(this->getMsg(44));
            lcd->print(getMsg(101));
            displayFloat(Voltage, char_3);
            lcd->print(char_3);
            lcd->print(F("V"));

            lcd->setCursor(85, LCD_ROW_4);
            lcd->print(this->getMsg(45));
        }
    }

    void warnMotorOil() override {
        if (amp->isRfr()) { //isRfr
            lcd->drawXBMP(0, 0, 85, 64, oil_level_85x64_bits);
            lcd->setCursor(85, LCD_ROW_1);
            lcd->print(this->getMsg(40));

            drawWarnMsg(41, 42);
        }
    }

    void warnLightsFront() override {
        if (amp->isRfr()) { //isRfr
            lcd->drawXBMP(0, 0, 85, 64, bulb_replacement_85x64_bits);

            lcd->setCursor(85, LCD_ROW_1);
            lcd->print(this->getMsg(58));

            drawWarnMsg(59, 60);
        }
    }


    void warnLightsBack() override {
        if (amp->isRfr()) { //isRfr
            lcd->drawXBMP(0, 0, 85, 64, bulb_replacement_85x64_bits);

            lcd->setCursor(85, LCD_ROW_1);
            lcd->print(this->getMsg(58));

            drawWarnMsg(59, 61);
        }
    }

    void warnBreakWare() override {
        if (amp->isRfr()) { //isRfr
            lcd->drawXBMP(0, 0, 85, 64, brake_ware_85x64_bits);

            lcd->setCursor(85, LCD_ROW_1);
            lcd->print(this->getMsg(46));

            drawWarnMsg(47, 48);
        }
    }

    void warnOverheat() override {
        if (amp->isRfr()) { //isRfr
            lcd->drawXBMP(0, 0, 85, 64, overheating_85x64_bits);

            lcd->setCursor(85, LCD_ROW_1);
            lcd->print(this->getMsg(52));

            drawWarnMsg(53, 54);
        }
    }

    void warnWinter() override {
        if (amp->isRfr()) { //isRfr
            lcd->drawXBMP(0, 0, 85, 64, winter_85x64_bits);

            lcd->setCursor(85, LCD_ROW_1);
            lcd->print(this->getMsg(37));

            drawWarnMsg(38, 39);
        }
    }

    void warnWasher() override {
        if (amp->isRfr()) { //isRfr
            lcd->drawXBMP(0, 0, 85, 64, washer_85x64_bits);

            lcd->setCursor(85, LCD_ROW_1);
            lcd->print(this->getMsg(55));

            drawWarnMsg(56, 57);
        }
    }

    void warnTmBelt() override {
        if (amp->isRfr()) { //isRfr

            lcd->setCursor(85, LCD_ROW_1);
            lcd->print(this->getMsg(62));

            drawWarnMsg(63, 64);
        }
    }


    /**
 *
 */
    void draWShutdownTripSave() override {

    }

/**
 *
 */
    void draWShutdownTripSkip() override {

    }

    /**
     *  changes the font to given
     */
    void setFont(const uint8_t *font) {
        this->currentFont = font;
        this->changedFont = true;
    }
    //
    // TODO reset to default font...

    void drawFont() {
        if (this->changedFont) {
            lcd->setFont(this->currentFont);
            lcd->setFontRefHeightExtendedText();
            lcd->setFontDirection(0);
            lcd->setDrawColor(1);
            lcd->setFontPosTop();
        } else {
            useDefaultMode();
        }
    }

    void handleDrawerEntry() {
        drawIndex++;
        if (MidCursorMenu == MENU_ENTRY) {
            drawEntry++;
        } else {
            drawEntry = 0;
        }
    }

    void handleDrawer() {
        drawIndex++;

        if (drawIndex > 11) {
            drawIndex = 0;
            initializeDraw = false;
        }
    }

    void makeDraw() {
        lcd->clearBuffer();
//        lcd->firstPage();
//        do {
        menus();
//        } while (lcd->nextPage());
        lcd->sendBuffer();
    }


/**
 * Draw graphic
 */
    void draw() override {
        cli(); // disable delay
        //
        //
        listenInfoMsg();
        //
        // Font handler
        drawFont();

        //
        // Ultra fast animation
        if (animateUltra) {
            if (amp->isMid()) {
//                Serial.println("Draw ultra ");
                makeDraw();
                handleDrawerEntry();
            }
        }

        //
        // Fast animation
        if (animateFast) {
            if (amp->isBig()) {
//                Serial.println("Draw big");
                makeDraw();
                handleDrawer();

            }
        }

        //
        // Slow / Normal animation
        if (!animateFast && !animateUltra) {
//            Serial.println("Draw slow ");
            if (amp->isRfr()) { //isRfr
                makeDraw();
                handleDrawer();
            }
        }
        sei(); //enable delay
    }

protected:
/**
 * NOT RECOMMENDED use
 * only for short time
 */
    inline void playUltra() {
        animateUltra = true;
        animateFast = false;
    }

    inline void playFast() {
        animateFast = true;
        animateUltra = false;
    }

    inline void playSlow() {
        animateFast = false;
        animateUltra = false;
    }

    void menus();

//    void aniHrzChar(u8g2_uint_t x, u8g2_uint_t y, const char *str) {
//        lcd->drawUTF8(aniIndex * 3, 36, str);
//    }

    void useDefaultMode() {
        lcd->enableUTF8Print();
        lcd->setFont(DEFAULT_FONT); // u8g2_font_unifont_t_cyrillic
        lcd->setFontRefHeightExtendedText();
        lcd->setFontDirection(0);
        lcd->setDrawColor(1);
        lcd->setFontPosTop();
    }


/**
 *
 * @param index of loop
 */
    void displayEntry() {
        //
        // TODO state error
        uint8_t subAnimateIndex = drawEntry & 4;

        //
        // Disable "switch screen" when showing screens
        // that not attached to menu structure.
        if (usedMenu.used == 0) {
            switch (drawEntry) {
                default:
                    drawEntry = 0;
                    break;
                case 0:
                    lcd->clearBuffer();
                    this->playUltra();
                    btn->useDebounceNormal();
                    break;
                case 1:
                    this->playSlow();
                    mbs->finishEntry();
                    btn->resetStates();
                    lcd->clear();
                    drawEntry = 0;
                    drawIndex = 0;
                    initializeDraw = true;

                    break;
            }
            return;
        }
        //
        // Shows menu structure
        switch (drawEntry) {
            default:
                drawEntry = 0;
                this->playUltra();

                break;
            case 0:
                lcd->clearBuffer();
                this->playUltra();
                mbs->startEntry();
                //
                // Reset button handler
                btn->setNavigationState(false);
                btn->useDebounceNormal();
                break;
            case 1:
            case 2:
            case 3:
                lcd->setCursor(108, 1);
                lcd->print(this->getMsg(2));
                lcd->drawLine(0, 12, lcd->getDisplayWidth(), 12);
                lcd->drawFrame(10, 12 + (drawEntry * 5), 230, 15);

                lcd->setCursor(LCD_COL_L11, 15);
                lcd->print(getMsg(getTitleMsgIndex(usedMenu.back)));
                lcd->setCursor(LCD_COL_L11, 30);
                lcd->print(getMsg(getTitleMsgIndex(usedMenu.used)));
                lcd->setCursor(LCD_COL_L11, 45);
                lcd->print(getMsg(getTitleMsgIndex(usedMenu.next)));

                break;
            case 4:
            case 5:
            case 6:
            case 7:
//
                lcd->drawFrame(10, 12 + LCD_ENTRY_FRAME + (3 * 5), 224, 15);
                lcd->setCursor(LCD_COL_L11, 15);
                lcd->print(getMsg(getTitleMsgIndex(usedMenu.back)));
                lcd->setCursor(LCD_COL_L11, 45);
                lcd->print(getMsg(getTitleMsgIndex(usedMenu.next)));

                lcd->setCursor(LCD_COL_L11, 30);
                lcd->print(getMsg(getTitleMsgIndex(usedMenu.used)));
                if (usedMenu.down > 0) {
                    lcd->print(getMsg(101));
                    lcd->print(getMsg(95));
                    lcd->print(getMsg(101));
                    lcd->print(getMsg(101));
                    lcd->print(getMsg(getTitleMsgIndex(usedMenu.down)));
                }
                break;
            case 8:
                this->playSlow();
                btn->resetStates();
                mbs->finishEntry();
                lcd->clear();
                drawEntry = 0;
                drawIndex = 0;
                initializeDraw = true;
                break;
        }
    }


private:
/**
 *
 * @param menu_t
 * @return
 */
    uint8_t getTitleMsgIndex(uint8_t menu_t) {

        //
        // General titles
        //

        if (menu_t == MENU_NAME_1) { // home
            return 11;
        }
        if (menu_t == MENU_NAME_2) { // trips
            return 12;
        }
        if (menu_t == MENU_NAME_4) { // service
            return 14;
        }

        //
        // Sub-titles
        //

        // Home
        if (menu_t == MENU_NAME_11) { // dash
            return 16;
        }
        if (menu_t == MENU_NAME_12) { // calibration
            return 17;
        }
        if (menu_t == MENU_NAME_13) { // about
            return 15;
        }

        // Trip
        if (menu_t == MENU_NAME_21) { // fuel
            return 13;
        }
        if (menu_t == MENU_NAME_22) { // race
            return 13;
        }

        if (menu_t == MENU_NAME_41) { // service
            return 99;
        }
        if (menu_t == MENU_NAME_42) { // service
            return 100;
        }
        //
        // Sub-sub-titles
        //

        if (menu_t == MENU_NAME_121) { // VSS
            return 27;
        }
        if (menu_t == MENU_NAME_122) { // RPM
            return 28;
        }
        if (menu_t == MENU_NAME_123) { // DST
            return 29;
        }
        if (menu_t == MENU_NAME_124) { // ECU
            return 30;
        }

        if (amp->isSec())
            Serial.println(F(" ERROR: Cannot resolve title index"));

    }


/**
 * Draws km as string
 * @param x
 * @param y
 */
    inline void showKm(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->setCursor(x, y);
        lcd->print(getMsg(69));
    }

/**
 * Draws average char
 * @param x
 * @param y
 */
    inline void showAverage(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->setCursor(x, y);
        lcd->print(getMsg(94));
    }

/**
 * Draws instant char
 * @param x
 * @param y
 */
    inline void showInstant(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->setCursor(x, y);
        lcd->print(getMsg(75));
    }


/**
 * Draws  L/100km
 * @param x
 * @param y
 */
    inline void showL100km(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->setCursor(x, y);
        lcd->print(getMsg(71));
    }

/**
 * Draws  L/100km
 * @param x
 * @param y
 */
    inline void showL100km(u8g2_uint_t x, u8g2_uint_t y, const char *parent) {
        uint8_t wd = lcd->getStrWidth(parent) + 5;
        lcd->setCursor(x + wd, y);
        lcd->print(getMsg(71));
    }

/**
 * Draws  celsius symbol
 * @param x
 * @param y
 */
    void showCels(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->setCursor(x + 1, y);
        lcd->print(getMsg(74));
    }

/**
 * Draws  celsius symbol
 * @param x
 * @param y
 */
    void showCels(u8g2_uint_t x, u8g2_uint_t y, const char *parent) {
        lcd->setCursor(x + lcd->getStrWidth(parent) + 1, y);
        lcd->print(getMsg(74));
    }

/**
 *
 */
    inline void showLiter(u8g2_uint_t x, u8g2_uint_t y) {
        lcd->setCursor(x, y);
        lcd->print(getMsg(68));
    }

    /**
     * Displays consumed fuel
     */
    void displayHomeConsumption() {

        lcd->setCursor(LCD_COL_L10 + 1, LCD_ROW_1);
        //
        // When have several fuel lines
#ifdef LPG_SWITCHING_DETECT
        if (car->getFuelState() == 0) {
            lcd->print(getMsg(90)); // petrol icon
        } else {
            lcd->print(getMsg(96)); // lpg icon
        }
#endif
//        lcd->print(getMsg(85)); // petrol station
        displayFloat(eep->getConsumedFuel(), char_3);
        lcd->drawStr(LCD_COL_L11, LCD_ROW_1, char_3);
        wordWidth = lcd->getStrWidth(char_3);

        lcd->setCursor(LCD_COL_L22, LCD_ROW_1);
        lcd->print(getMsg(68));
        lcd->print(getMsg(101));
        lcd->print(getMsg(101));




        //
        // Average consumption per 100km
        lcd->setCursor(LCD_COL_L10, LCD_ROW_2 - 2);
        lcd->print(getMsg(94));
        float consumption = eep->getAverageLitersPer100km();
        if (car->getDst() < 1 && eep->getTravelDistance() < 1) {
            lcd->setCursor(LCD_COL_L11, LCD_ROW_2);
            (consumption > SCREEN_CNS_SKIP) ? lcd->print(F("--.-")) : lcd->print(F("00.0"));
        } else {
            displayFloat(consumption, char_3);
            lcd->drawStr(LCD_COL_L11, LCD_ROW_2, char_3);
        }
        showL100km(LCD_COL_L22, LCD_ROW_2);
    }

    /**
     * Home center screen
     */
    void displayHomeCenter() {
#ifdef USE_BERTONE_LOGO
        lcd->drawXBMP(93, 15, 44, 48, bertone_small_bits);
#else
        lcd->drawXBMP(93, 19, 44, 36, opel_small_bits);
#endif
    }

/**
 * Display temperatures
 */
    void displayHomeTemperatures() {

        lcd->setCursor(LCD_COL_L10, LCD_ROW_3 - 1);
        lcd->print(getMsg(98));

        displayFloat(car->getTmpOut(), char_3);
        lcd->drawStr(LCD_COL_L11, LCD_ROW_3, char_3);
        this->showCels(LCD_COL_L22 + 2, LCD_ROW_3);

        //
        // if there no thermometer dump fuel wasted
        if (car->getTmpIns() < -99) {
            lcd->setCursor(LCD_COL_L10 - 1, LCD_ROW_4 + 1);
            lcd->print(getMsg(79));
            displayFloat(car->getCurFuelWasted(), char_3);
            lcd->drawStr(LCD_COL_L11, LCD_ROW_4, char_3);

            lcd->setCursor(LCD_COL_L22 + 2, LCD_ROW_4);
            lcd->print(getMsg(68));
        } else {
            lcd->setCursor(LCD_COL_L10, LCD_ROW_4 + 1);
            lcd->print(getMsg(97));
            //
            // Data
            displayFloat(car->getTmpIns(), char_3);
            lcd->drawStr(LCD_COL_L11, LCD_ROW_4, char_3);
            wordWidth = lcd->getStrWidth(char_3);
            this->showCels(LCD_COL_L22 + 2, LCD_ROW_4);
        }

    }

/**
 * Displays trip
 */
    void displayHomeTrip() {
        SavedData saved = eep->getData();
        //
        // Travel distance
        lcd->setCursor(LCD_COL_R11, LCD_ROW_1);
        lcd->print(getMsg(76));
        displayFloat(car->getDst() + saved.dist_trp, char_4);
        lcd->setCursor(LCD_COL_R12, LCD_ROW_1);
        lcd->print(char_4);
        lcd->print(getMsg(101));
        lcd->print(getMsg(69));
        //
        // Travel time
        car->getHTm(saved.time_trp, char_5);
        lcd->setCursor(LCD_COL_R11, LCD_ROW_2);
        lcd->print(getMsg(77));
        lcd->setCursor(LCD_COL_R12, LCD_ROW_2);
        lcd->print(char_5);
        lcd->print(getMsg(101));
        lcd->print(getMsg(70));
        //
        // Average speed
        showAverage(LCD_COL_R11, LCD_ROW_3);
        sprintf(char_2, "%02d", car->getAvrVss());
        lcd->setCursor(LCD_COL_R12, LCD_ROW_3);
        lcd->print(char_2);
        lcd->print(getMsg(101));
        lcd->print(getMsg(69));
        //
        // Instant cons per 100km
        showInstant(LCD_COL_R11, LCD_ROW_4);

        float dataFuel = (car->getFuelState() == 0) ? car->getDefFuelCns() : car->getAdtFuelCns();
        float result = ((dataFuel) * 100) / car->getDst();
        if (result > SCREEN_CNS_SKIP) {
            result = 0;
        }
        displayFloat(result, instantCons);


        lcd->setCursor(LCD_COL_R12, LCD_ROW_4);
        lcd->print(instantCons);
        showL100km(LCD_COL_R12, LCD_ROW_4, instantCons);

    }

/****************************************************************
 * Display Dashboard
 */

    void displayCarVss() {
        lcd->setCursor(0, LCD_ROW_1);
        lcd->print(F("KMH "));
        sprintf_P(char_3, pgm3 /*"%03d"*/, car->getVss());
        lcd->print(char_3);
    }

    void displayEngRpm() {
        lcd->setCursor(0, LCD_ROW_2);
        lcd->print(F("RPM "));
        sprintf_P(char_4, pgm4 /*"%04d"*/, car->getRpm());
        lcd->print(char_4);
    }

    void displayCarEcu() {
        lcd->setCursor(0, LCD_ROW_3);
        lcd->print(F("ECU "));
        sprintf_P(char_2, pgm4 /*"%04d"*/, car->getEcu());
        lcd->print(char_2);
    }

    void displayCarDst() {
        lcd->setCursor(0, LCD_ROW_4);
        lcd->print(F("DST "));
        displayFloat(eep->getTravelDistance(), char_3);
        lcd->print(char_3);
    }

    void displayCarOdm() {
        lcd->setCursor(LCD_COL_R12 - 10, LCD_ROW_1);
        lcd->print(F("ODO "));
        char vds[20];
        sprintf_P(vds, pgm5 /*"%lu"*/, car->getVdsDump());
        lcd->print(vds);
    }

    void displayCarEnt() {
        lcd->setCursor(LCD_COL_R12 - 10, LCD_ROW_2);
        lcd->print(F("ENG "));
        sprintf_P(char_3, pgm3/*"%03d"*/, car->getEngTmp());
        lcd->print(char_3);
        lcd->print(getMsg(101));
//        this->showCels(LCD_COL_R23, LCD_ROW_2, char_3);
    }

    void displayCarGrs() {
        lcd->setCursor(LCD_COL_R12 - 10, LCD_ROW_3);
        lcd->print(F("GRS "));
        sprintf_P(char_2, pgm6/*"%01d"*/, car->getGear());
        lcd->print(char_2);
    }

    void displayCarWrk() {
        lcd->setCursor(LCD_COL_R12 - 10, LCD_ROW_4);
        lcd->print(F("WRK "));
        sprintf_P(char_6, pgm4 /*"%04d"*/, eep->getWorkDistance());
        lcd->print(char_6);
        lcd->print(getMsg(69));
    }

    void displayCarBlb() {
        lcd->setCursor(LCD_COL_L22 + 10, LCD_ROW_1);
        lcd->print(F("BLB "));
        lcd->print(analogRead(STT_UL1_PIN));
        lcd->print(F("/"));
        lcd->print(analogRead(STT_UL2_PIN));
    }

    void displayCarPec() {
        lcd->setCursor(LCD_COL_L22 + 10, LCD_ROW_2);
        lcd->print(F("PEC "));
        lcd->print(eep->getPec());
    }

    /****************************************************************
 * About Fuel
 */
    void displayInfo() {
        lcd->setCursor(LCD_COL_L10, LCD_ROW_1);
        lcd->print(getMsg(20));
        lcd->setCursor(LCD_COL_L10, LCD_ROW_2);
        lcd->print(getMsg(21));

        lcd->setCursor(LCD_COL_L10, LCD_ROW_4);
        if (drawIndex < 5) {
            lcd->print(getMsg(22));

        } else {
            lcd->print(getMsg(23));
            lcd->print(getMsg(101));
            lcd->print(MID_VERSION);
        }
    }

    /**
     * Edit user information
     */
    void displayEdit() {
        lcd->enableUTF8Print();
        lcd->setCursor(LCD_COL_L10, LCD_ROW_2);
        if (drawIndex < 2)
            lcd->print(getMsg(24));
        else if (drawIndex > 2 && drawIndex < 6)
            lcd->print(getMsg(25));
        else if (drawIndex > 6 && drawIndex < 10)
            lcd->print(getMsg(26));

        drawInfoMsg(32);

    }

/****************************************************************
* Car Servicing
*/
    void displayCarState() {

        Diagnostic history = stt->getResult();


        if (history.brk) {
            lcd->setCursor(0, LCD_ROW_1);
            lcd->print(F(">"));
        }
        lcd->setCursor(LCD_COL_L10, LCD_ROW_1);
        lcd->print(getMsg(3));

        lcd->setCursor(LCD_COL_R12 - 20, LCD_ROW_1);
        if (stt->getLiveBrk())lcd->print((drawIndex < 5) ? getMsg(9) : getMsg(8)); else lcd->print(getMsg(7));

        // Coolant
        if (history.cnt) {
            lcd->setCursor(0, LCD_ROW_2);
            lcd->print(F(">"));
        }

        lcd->setCursor(LCD_COL_L10, LCD_ROW_2);
        lcd->print(getMsg(4));
        lcd->setCursor(LCD_COL_R12 - 20, LCD_ROW_2);
        if (stt->getLiveCnt()) lcd->print(getMsg(8)); else lcd->print(getMsg(7));


        // Window washer
        if (history.win) {
            lcd->setCursor(0, LCD_ROW_3);
            lcd->print(F(">"));
        }
        lcd->setCursor(LCD_COL_L10, LCD_ROW_3);
        lcd->print(getMsg(5));
        lcd->setCursor(LCD_COL_R12 - 20, LCD_ROW_3);
        if (stt->getLiveWin()) lcd->print(getMsg(8));
        else lcd->print(getMsg(7));


        // Oil level
        if (history.oil) {
            lcd->setCursor(0, LCD_ROW_4);
            lcd->print(F(">"));
        }
        lcd->setCursor(LCD_COL_L10, LCD_ROW_4);
        lcd->print(getMsg(6));

        lcd->setCursor(LCD_COL_R12 - 20, LCD_ROW_4);
        if (stt->getLiveOil()) lcd->print(getMsg(8));
        else lcd->print(getMsg(7));


        if (MidCursorMenu == 46) {
            lcd->print(getMsg(132));
            lcd->print(eep->getWorkDistance());
            lcd->write((uint8_t) 2);
            lcd->print(getMsg(101));
        }
    }

/****************************************************************
 * Trip graphic
 */
    void displayTripDetails() {
        //
        // Trip max rpm
        lcd->drawXBMP(4, LCD_ROW_1 + 2, 18, 18, gage_max_18x18_bits);
        sprintf(char_4, "%04d", car->getMxmRpm());
        lcd->drawStr(LCD_COL_L12, LCD_ROW_1, char_4);
        lcd->setCursor(LCD_COL_L22, LCD_ROW_1);
        lcd->print(F("rpm"));
        //
        // Trip max vss
        displayFloat(car->getMxmVss(), char_3);
        lcd->drawStr(LCD_COL_L12, LCD_ROW_2, char_3);
        showKm(LCD_COL_L22, LCD_ROW_2);
        //
        // Breaking time
        lcd->drawXBMP(LCD_COL_L11, LCD_ROW_3, 18, 10, car_brk_18x10_bits);
        displayFloat(car->getBreakTime(), char_3);
        lcd->drawStr(LCD_COL_L21, LCD_ROW_3, char_3);
        lcd->setCursor(LCD_COL_L23, LCD_ROW_3);
        lcd->print(F("min."));
        //
        // Avr RPM time
        lcd->drawXBMP(LCD_COL_L11 + 1, LCD_ROW_4, 18, 10, eng_avr_18x10_bits);
        sprintf(char_4, "%04d", car->getAvrRpm());
        lcd->drawStr(LCD_COL_L21, LCD_ROW_4, char_4);
        //
        // Travel fuel
        lcd->drawXBMP(LCD_CNR, LCD_ROW_1, 18, 18, fuel_18x18_bits);
        lcd->drawXBMP(LCD_COL_R11 + 1, LCD_ROW_1, 5, 8, drop_5x8_bits);
        displayFloat(car->getCurFuelCns(), char_3);
        lcd->drawStr(LCD_COL_R12, LCD_ROW_1, char_3);
        showLiter(LCD_COL_R22, LCD_ROW_1);
        //
        // Travel fuel waste
        lcd->drawXBMP(LCD_COL_R11, LCD_ROW_2, 6, 8, trash_6x8_bits);
        displayFloat(car->getCurFuelWasted(), char_3);
        lcd->drawStr(LCD_COL_R12, LCD_ROW_2, char_3);
        showLiter(LCD_COL_R22, LCD_ROW_2);
        //
        // Consumed fuel per 100km
        showAverage(LCD_COL_R11, LCD_ROW_3);
        displayFloat((car->getCurFuelCns() * 100) / (car->getDst()), char_3);
        lcd->drawStr(LCD_COL_R12, LCD_ROW_3, char_3);
        showL100km(LCD_COL_R22, LCD_ROW_3);


    }


    void buildInnerRowTrip(float carFuelConsumption, float dataFuel, float distance, uint8_t y) {
        displayFloat(dataFuel + carFuelConsumption, char_3);
        lcd->print(char_3);
        lcd->print(getMsg(68));
        lcd->print(getMsg(101));
        showAverage(LCD_COL_R11, y);
        lcd->setCursor(LCD_COL_R12, y);
        displayFloat(((dataFuel + car->getAdtFuelCns()) * 100) / distance, char_3);
        if (distance > 1) {
            lcd->print(char_3);
        } else {
            lcd->print(F("00.0"));
        }
        lcd->print(getMsg(68));
        lcd->print(F("  "));
    }

/**
 * Display trip row
 * */
    void buildRowTrip(const __FlashStringHelper *name, TripData data, uint8_t y) {
        float dst = car->getDst() + data.range;
        lcd->setCursor(LCD_COL_L10, y);
        lcd->print(name);
        displayFloat(dst, char_4);
        lcd->print(char_4);
        lcd->print(getMsg(69));
        lcd->setCursor(LCD_COL_L23 + 15, y);


#ifdef  DEFAULT_FUEL_USING
        if (DEFAULT_FUEL_USING == 1) {
            buildInnerRowTrip(car->getAdtFuelCns(), data.fuel, dst, y);
        } else {
            buildInnerRowTrip(car->getDefFuelCns(), data.fuel, dst, y);
        }
#else
        buildInnerRowTrip(car->getDefFuelCns(), data.fuel, dst, y);
#endif
    }

    void displayTrips() {
        this->buildRowTrip(F("T0:  "), eep->getTrip0(), LCD_ROW_1);
        this->buildRowTrip(F("T1:  "), eep->getTripA(), LCD_ROW_2);
        this->buildRowTrip(F("T2:  "), eep->getTripB(), LCD_ROW_3);
        this->buildRowTrip(F("T3:  "), eep->getTripC(), LCD_ROW_4);

        btn->setEditorState(true);

        if (drawIndex < 2 && initializeDraw) {
            //
            // Change values and speed of buttons
            btn->setValueControlled(valueCursor);

        }
        valueComparator = valueCursor;
        //
        // Manage section
        if (!btn->getNavigationState()) {
            //
            // Change speed of screen
            this->playUltra();
            signed char cursor = btn->getValueControlled();
            //
            // Detect button up / change cursor
            if (cursor > valueComparator && drawIndex % 4 == 0 && tripReset == 0) {
                valueCursor++;
                tripReset = 0;
            }

//            dump_dec("cursor", cursor);
//            dump_dec("value", valueComparator);
            // TODO: position of cursor for T1
            // Validate rest of trip
            boolean delete_trip = false;
            if (cursor < tripReset && drawIndex % 4 == 0) {

                if (tripReset == valueComparator) {
                    valueComparator = cursor;
                }

                if (cursor < valueComparator) {
                    Serial.println(F("NOTICE: Delete trip.... "));
                    tripReset = 0;
                    valueComparator = 0;
                    delete_trip = true;
                }
            }

            //
            // Un-activate delete
            if (tripReset != 0 && cursor > valueComparator) {
                if (cursor > valueComparator) {
                    Serial.println(F("NOTICE: Skip deleting ... "));
                    tripReset = 0;
                    valueComparator = 0;
                    delete_trip = false;
                    tripReset = 0;
                }
            }

            //
            // Detect button down / set trip to reset
            if (cursor < valueComparator) {
                lcd->setCursor(LCD_COL_R23, tripActive);
                lcd->print(getMsg(105));
                lcd->setCursor(LCD_COL_R22 + 10, tripActive);
                valueComparator = cursor; // to compare next loop
                tripReset = cursor; // activate trip reset
            }

            //
            // Switch  rows
            if (tripReset == 0) {
                //
                // Activate trip for reset
                if (valueCursor == 1) {
                    tripActive = LCD_ROW_2;
                } else if (valueCursor == 2) {
                    tripActive = LCD_ROW_3;
                } else if (valueCursor == 3) {
                    tripActive = LCD_ROW_4;
                } else if (!delete_trip) {
                    tripReset = 0;
                    lcd->setCursor(0, LCD_ROW_2);
                    valueCursor = 1;// Clear wrong cursor
                    btn->setValueControlled(valueCursor);
                    Serial.println(F("ERROR: Trip cursor out of range .... "));
                }
                lcd->setCursor(0, tripActive);
            }

            //
            // Display cursor blink
            if (drawIndex % 4 == 0) {
                lcd->print(F(">"));

            }

            //
            // Reset trip data from EepRom
            if (delete_trip) {
                if (tripActive == LCD_ROW_2) {
                    eep->resetTripA();
                    Serial.println(F("DELETE trip A "));
                } else if (tripActive == LCD_ROW_3) {
                    eep->resetTripB();
                    Serial.println(F("DELETE trip B "));
                } else if (tripActive == LCD_ROW_4) {
                    eep->resetTripC();
                    Serial.println(F("DELETE trip C "));
                } else {
                    Serial.println(F("ERROR: Cannot find cursor for deleting .... "));
                }
                valueCursor = 1;
                valueComparator = 1;
                tripActive = 0;
                tripReset = 0;
                btn->setValueControlled(valueCursor);
                btn->setNavigationState(true);
            }


        }
    }

/****************************************************************
 * Draw graph
 */
    void displayGraph() {
        uint8_t arrSize = 10;
        uint8_t wdDsp = lcd->getWidth();

        if (initializeDraw) {
            playFast();
            graphLines(arrSize, wdDsp, drawIndex);
        } else {
            this->lastValue = 25;
            graphLines(arrSize, wdDsp, arrSize);
            playSlow();
        }
    }


    void graphLine(uint8_t arrSize, uint8_t wdDsp, uint8_t index) {
        uint8_t cur = /*value*/ graphTest[index];
        lcd->drawLine(((wdDsp / arrSize) * index), this->lastValue,
                      ((wdDsp / arrSize) * (index + 1)), cur);
        this->lastValue = cur;
    }


    void graphLines(uint8_t arrSize, uint8_t wdDsp, uint8_t toIndex) {
        this->lastValue = 25;
        for (uint8_t i = 0; i < toIndex; ++i) {
            graphLine(arrSize, wdDsp, i);
        }
    }


/****************************************************************
 * Fuel information
 */
    void displayFuel() {
        uint8_t bnz = car->getFuelTnk();

        lcd->setCursor(LCD_COL_L11, LCD_ROW_2);
        lcd->print(getMsg(90));
        lcd->setCursor(LCD_COL_L21, LCD_ROW_2);
        lcd->print(bnz);
        lcd->print(getMsg(68));
        lcd->setCursor(LCD_COL_L23, LCD_ROW_2);
        lcd->print(getMsg(121));
        // todo: change this and measure how many km will travel until fuel ends
//        lcd->print((bnz * 100) / eep->getDefFuel());
        lcd->print(getMsg(69));

/* TODO Show fuel information
 * TODO add icons
 * */
#ifdef ADT_FUEL_SYSTEM_SERIAL
        sprintf(char_2, "%02d", lpgCom.getFuelTankLevel());
        lcd->setCursor(LCD_COL_L11, LCD_ROW_3);
        lcd->print(getMsg(96));
        lcd->setCursor(LCD_COL_L23, LCD_ROW_3);
        lcd->print(char_2);
        lcd->print(getMsg(68));
#endif
#ifdef LPG_SWITCHING_DETECT
        if (drawIndex < 2 && initializeDraw) {
            btn->setEditorState(true);
            btn->setValueControlled(138);
            btn->setValueControlled(101);
        }

        lcd->setCursor(LCD_COL_R11, LCD_ROW_3);
        sprintf(char_2, "%02d", lpgCom.getCurrentValue());
        lcd->print(getMsg(83));
        lcd->print(char_2);

        if (!btn->getNavigationState()) {
            this->playUltra();
            lcd->print(getMsg(101));
            lcd->print(getMsg(102));


            if (btn->getValueControlled() < 138) {
                lpgCom.setDynamic();
                btn->setEditorState(false);
                btn->setNavigationState(true);
//                Serial.println(F("LPG set dynamic"));
            }

            if (btn->getValueControlled() > 138) {
                btn->setEditorState(false);
                btn->setNavigationState(true);
                car->passMelodyClass()->play(7);
//                Serial.println(F("LPG set skipped"));
            }
        }
#endif

    }


/****************************************************************
 * Settings editor
 */
    void displayEditor() {
        //
        // Max value 65535
        float defVal = 0, curVal = 0, oldVal = 0, result = 0;

        btn->setEditorState(true);

        switch (MidCursorMenu) {
            case 121:
                defVal = VSS_CORRECTION;
                oldVal = curVal = (car->getCorVss() * 100); // mul by 100 then mul by 0.01
                result = car->getVss();
                break;
            case 122:
                defVal = RPM_CORRECTION;
                oldVal = curVal = car->getCorRpm();
                result = car->getRpm();
                break;
            case 123:
                defVal = DST_CORRECTION;
                oldVal = curVal = car->getCorDst();
                result = (uint32_t) car->getDst();
                break;
            case 124:
                defVal = ECU_CORRECTION;
                oldVal = curVal = car->getCorEcu();
                result = car->getEcu();
                break;
            default:
                break;
        }
        if (drawIndex < 2 && initializeDraw) {
            //
            // Change values and speed of buttons
            btn->setValueControlled(curVal);
            //
            // Change speed of screen
            this->playUltra();
        }
        //
        // Getting back modified value
        curVal = btn->getValueControlled();

        //
        // Current value

        displayFloat(curVal, char_7);
        lcd->setCursor(LCD_COL_L12, LCD_ROW_1);
        lcd->print(getMsg(18));
        lcd->setCursor(LCD_COL_R11, LCD_ROW_1);
        lcd->print(char_7);

        if (!btn->getNavigationState() && drawIndex % 4 == 0) {
            lcd->setCursor(LCD_COL_R11 - (lcd->getStrWidth(("["))) - 8, LCD_ROW_1);
            lcd->print(F("["));
            lcd->setCursor(LCD_COL_R11 + lcd->getStrWidth(char_7) + 10, LCD_ROW_1);
            lcd->print(F("]"));
        }



        /**
         * Set data to carSens
         */
        if (!btn->getNavigationState() && curVal != oldVal) {
            //
            // Change speed of screen
            this->playUltra();
            //
            // Switch screen handler
            switch (MidCursorMenu) {
                case 121:
                    eep->setSensVss(curVal * 0.01);
                    break;
                case 122:
                    eep->setSensRpm(curVal);
                    break;
                case 123:
                    eep->setSensDst(curVal);
                    break;
                case 124:
                    eep->setSensEcu(curVal);
                    break;
            }
            car->setSave(eep->getData());
        }


        displayFloat(defVal, char_7);
        //
        // Default value
//        sprintf(char_7, "%0d.%02d", (uint16_t) defVal, (uint8_t) (defVal * 100) % 100);
        lcd->setCursor(LCD_COL_L10, LCD_ROW_2);

        lcd->setFont(u8g_font_opel_ic_12);
        lcd->print(getMsg(19));
        lcd->print(getMsg(101));
        lcd->print(char_7);
        lcd->print(getMsg(122));
        lcd->print(F("/"));

        //
        // Result value
        sprintf(char_7, "%07d", (uint16_t) result);
        lcd->print(getMsg(122));
        lcd->print(getMsg(31));
        lcd->print(getMsg(101));
        lcd->print(char_7);

        uint8_t msg[2] = {};
        switch (MidCursorMenu) {
            case 121:
            case 123:
            case 124:
                msg[0] = 114;
                msg[1] = 32;
                break;
            case 122:
                msg[0] = 115;
                msg[1] = 32;
                break;
            default:
                break;
        }
        drawInfoMsgMul(msg, 2);
    }

/****************************************************************
 * Rally gage
 */
    void displayRallyGages() {
        const uint8_t wdDsp = 204;
        const uint8_t hgDsp = 74;
        const uint16_t maxPwr = MAX_ENGINE_PWR;
        const uint16_t maxRpm = MAX_ENGINE_RVS;
        const uint8_t all_blocks = 18;

        uint16_t rpm = car->getRpm();

        uint8_t current = (uint8_t) map(rpm, 0, maxRpm, 0, all_blocks);
        if (current < 1) {
            current = 1;
        }
        if (current > all_blocks) {
            current = all_blocks;
        }
        uint8_t width = wdDsp / all_blocks - 1;
        uint8_t height = hgDsp / all_blocks;
        //
        // Power compensation
        uint8_t res = (uint8_t) map(maxPwr, 0, maxRpm, 0, all_blocks);

        //
        // Draw rpm
        for (uint8_t i = 1; i < current; i++) {

            uint8_t crh = height * i;
            if (res < i) {
                //
                // Stepping down the power
                crh = crh - (all_blocks / res) * ((i - res) * 8);
            }
            lcd->drawBox(40 + (wdDsp / all_blocks) * i, hgDsp - crh - res, width, crh);
        }




        // GEARS
        // W47 H63
        // https://github.com/olikraus/u8g2/wiki/fntgrpinconsolata
        uint8_t gear = car->getGear();
        lcd->setCursor(10, 3);

        if (gear > 6 || gear < 1) {
            lcd->print(F("N"));
        } else {
            sprintf(char_2, "%01d", gear);
            lcd->print(char_2);
        }

        // Reset fonts


    }

    /**
     * Simple Voltmeter
     */
    void showVoltmeter(void) {

        lcd->setCursor(LCD_CNR, LCD_ROW_2);
        lcd->print(stt->getVoltage());
        lcd->print(F("V "));
        drawInfoMsg(110);

    }

/**
 * Reset servicing distance
 */
    void showOdoWork(void) {

        btn->setEditorState(true);
        lcd->setCursor(LCD_COL_L10, LCD_ROW_2);
        lcd->print(getMsg(111));
        lcd->print(eep->getWorkDistance());
        lcd->print(getMsg(69)); // km
        lcd->setCursor(LCD_COL_L10, LCD_ROW_4);
        lcd->print(getMsg(106));

        if (drawIndex < 2 && initializeDraw) {
            btn->setEditorState(true);
            btn->setValueControlled(138);
        }


        if (!btn->getNavigationState()) {
            valueComparator = (uint8_t) btn->getValueControlled();
            drawInfoMsg(111);
        } else
            drawInfoMsg(113);

        if (!btn->getNavigationState()) {
            if (btn->getValueControlled() < 138) {
                // TODO add eeprom reset value
                btn->setEditorState(false);
                btn->setNavigationState(true);
                eep->resetWrkOdo();
            }

            if (btn->getValueControlled() > 138) {
                btn->setEditorState(false);
                btn->setNavigationState(true);
            }
        }
    }
};

/**
 *
 * @param index
 */
void Lcd240x62::menus() {
    switch (MidCursorMenu) {
        default:
        case MENU_ENTRY:
            this->changedFont = false;
            this->playUltra();

            displayEntry();
            break;
            //
            // Main / first menu
        case 1:
            showHeader(getMsg(11));
            displayHomeTemperatures();
            displayHomeConsumption();
            displayHomeTrip();
            displayHomeCenter();
            break;
            //
            // Dashboard
        case 11:
            showHeader(getMsg(16));
            displayCarVss();
            displayEngRpm();
            displayCarEcu();
            displayCarDst();
            displayCarOdm();
            displayCarEnt();
            displayCarGrs();
            displayCarWrk();
            displayCarBlb();
            displayCarPec();
            break;
        case 12:
            showHeader(getMsg(17));
            displayEdit();
            break;
        case 121: // VSS
        case 122: // RPM
        case 123: // DTS
        case 124: // CNS
            showHeader(getMsg(MidCursorMenu - 94));
            displayEditor();
            break;
        case 13:
            showHeader(getMsg(15));
            displayInfo();
            break;
            //
            // Travel menu
        case 2:
            showHeader(getMsg(12));
            displayTrips();
            break;
            //
            // Fuel menu
        case 21:
            showHeader(getMsg(13));
            displayFuel();
            break;
        case 22:
            setFont(u8g2_font_logisoso58_tr);
            displayRallyGages();
            break;

        case 4:
            showHeader(getMsg(14));
            displayCarState();
            break;

        case 41:
            showHeader(getMsg(99));
            showVoltmeter();
            break;

        case 42:
            showHeader(getMsg(100));
            showOdoWork();
            break;

        case CarState::MENU_SERVICE:
            stt->menu(this);
            break;

        case ShutDw::MENU_SHUTDOWN:
            sdw->menu(this);
            break;
    }

}


#endif //ARDUINOMID_LCD240X64_H
