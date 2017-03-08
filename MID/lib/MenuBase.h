//
// Created by Angel Zaprianov on 23.2.2017 г..
//

#ifndef ARDUINO_MID_MENUBASE_H
#define ARDUINO_MID_MENUBASE_H

//#define MENU_DEBUG // Uncomment to get info

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
    MidMenuInterface *mci;
    MenuBtn *btn;
public:

    //
    // Saves cursor between changes
    uint8_t savedCursor;
    static char usedMenu[74];


    //
    // Constructor
    MenuBase(MenuBtn *_btn, MidMenuInterface *_mci) {
        btn = _btn;
        mci = _mci;

    }

    MidMenuInterface *passMci() {
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
#if defined(MENU_DEBUG) 
            if (btn->passAmp()->isMid()) {
                Serial.println("Makes initialization move ... \n\r");
            }
#endif
            //
            // Move menu to first index
            mci->moveUp();
            savedCursor = 1;
        }

        MidCursorMenu = savedCursor;

#if defined(MENU_DEBUG) 
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
#if defined(MENU_DEBUG) 
            if (btn->passAmp()->isMid()) {
                Serial.print("Up hit \n\r");
            }
#endif
        }
        if (btn->isDw()) {
            mci->moveDw();
#if defined(MENU_DEBUG) 
            if (btn->passAmp()->isMid()) {
                Serial.print("Dw hit \n\r");
            }
#endif
        }


        btn->clearLastButton();


#if defined(MENU_DEBUG) 
        if (btn->passAmp()->isSecond()) {
            Serial.print("Cursor saved: ");
            Serial.println(savedCursor);
        }
#endif
        //
        //
        if (savedCursor != MidCursorMenu && MidCursorMenu != MENU_ENTRY) {
            //
            // Keep cursor in save place
            savedCursor = MidCursorMenu;
            //
            // Change menu to show screen
            MidCursorMenu = MENU_ENTRY;
        }
#if defined(MENU_DEBUG) 
        if (btn->passAmp()->isSecond()) {
            Serial.print("Cursor MID: ");
            Serial.println(MidCursorMenu);
        }
#endif
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

char MenuBase::usedMenu[74] = "";

#endif //ARDUINOMID_MENUBASE_H
