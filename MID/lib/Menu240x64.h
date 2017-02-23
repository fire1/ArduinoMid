//
// Created by Angel Zaprianov on 23.2.2017 г..
//

#ifndef ARDUINOMID_MENU240X64_H
#define ARDUINOMID_MENU240X64_H

#include <Arduino.h>
#include "../conf.h"
#include "MenuBtn.h"
#include <MenuBackend.h>

static void MidMenu_menuUsed(MenuUseEvent used);

static void MidMenu_menuChanged(MenuChangeEvent changed);

/**
 *
 */
class Menu240x60 : virtual public MidMenu {
    MenuBtn *btn;
public:
    Menu240x60(MenuBtn *_btn);

    void setup(void);

/**
 * Listen buttons and navigate
 * @param whl
 * @param cursor
 */
    void listener(int &cursor);

    void startEntry();

    void finishEntry();

    boolean isNavigationActive();
};

Menu240x60::Menu240x60(MenuBtn *_btn) {
    btn = _btn;
}

void Menu240x60::setup() {

}

void Menu240x60::listener(int &cursor) {

}

boolean Menu240x60::isNavigationActive() {

}

void Menu240x60::startEntry() {

}

void Menu240x60::finishEntry() {

}

#endif //ARDUINOMID_MENU240X64_H
