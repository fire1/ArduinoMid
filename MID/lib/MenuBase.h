//
// Created by Angel Zaprianov on 23.2.2017 г..
//

#ifndef ARDUINO_MID_MENUBASE_H
#define ARDUINO_MID_MENUBASE_H

#define MENU_DEBUG // Uncomment to get info

#include <Arduino.h>
#include <MenuBackend.h>
#include "../conf.h"
#include "MenuBtn.h"


//
// Define menu tree
#ifndef MENU_ENTRY
#define MENU_ENTRY 0
#endif


/**
 *
 */
class MenuBase {
    IMidMenu *mci;
    MenuBtn *btn;
public:

    //
    // Saves cursor between changes
    uint8_t savedCursor;
    static const char *usedMenu;
    static const char *lastMenu;

    //
    // Constructor
    MenuBase(MenuBtn *_btn, IMidMenu *_mci) {
        btn = _btn;
        mci = _mci;

    }

    IMidMenu *passMci() {
        return mci;
    }

    /**
     * @deprecated
     */
    void startEntry() {
    }

    void finishEntry() {
        //
        // Handles initialization
        if (!savedCursor) {
            mci->moveUp();
            savedCursor= 1;
        }
        MenuBase::lastMenu = MenuBase::usedMenu;
        MidCursorMenu = savedCursor;

#if defined(MENU_DEBUG) || defined(GLOBAL_SENS_DEBUG)
        Serial.print("Cursor menu: ");
        Serial.println(MidCursorMenu);
        Serial.print("Saved Cursor menu: ");
        Serial.println(savedCursor);
        Serial.print("Used menu: ");
        Serial.println(MenuBase::usedMenu);
#endif
    }


    /**
     * Perform navigation
     */
    void listener() {

        //
        // Handle navigation
        if (btn->isUp()) {
            mci->moveUp();
#if defined(MENU_DEBUG) || defined(GLOBAL_SENS_DEBUG)
            if (btn->passAmp()->isMid()) {
                Serial.print("Up hit \n\r");
            }
#endif
        }
        if (btn->isDw()) {
            mci->moveDw();
#if defined(MENU_DEBUG) || defined(GLOBAL_SENS_DEBUG)
            if (btn->passAmp()->isMid()) {
                Serial.print("Dw hit \n\r");
            }
#endif
        }



        btn->clearLastButton();
        //
        //
        if (savedCursor != MidCursorMenu && MidCursorMenu != MENU_ENTRY ) {
            //
            // Keep cursor in save place
            savedCursor = MidCursorMenu;
            //
            // Change menu to show screen
            MidCursorMenu = MENU_ENTRY;
        }

        if (btn->passAmp()->isSecond()) {
            Serial.print("Cursor MID: ");
            Serial.println(MidCursorMenu);
        }
    }

    boolean isNavigationActive() {
        return true;
    }

    void setNavigation(boolean nav) {

    }

    boolean isEditOption() {
        return false;
    }

    void setEditOption(boolean edit) {

    }

};


const char *MenuBase::usedMenu = "";
const char *MenuBase::lastMenu = "";

#endif //ARDUINOMID_MENUBASE_H
