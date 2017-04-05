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
    MidMenuInterface *mci;
    MenuBtn *btn;
public:
    //
    //
    // NOTE: Static variable doesn't work + Array corrupted
    // http://forum.arduino.cc/index.php/topic,46462.0.html
    //
    // Saves cursor between changes
    uint8_t savedCursor = 0;
    static const char *usedMenu;
    static const char *usedBack;
    static const char *usedNext;
    static const char *usedDown;

//
    //
    // Constructor
    MenuBase(MenuBtn *_btn, MidMenuInterface *_mci)
    //
    // http://arduino.stackexchange.com/questions/682/is-using-malloc-and-free-a-really-bad-idea-on-arduino
    {
        btn = _btn;
        mci = _mci;
        savedCursor = 1;
//        MenuBase::usedMenu = new char[74] ;
    }

    MidMenuInterface *passMci() {
        return mci;
    }

    /**
     * Used to activate / deactivate navigation
     */
    void startEntry() {
        btn->setNavigationState(false);
//        delete [] MenuBase::usedMenu;
    }

    /**
     * Restore position of menu
     */
    void finishEntry() {

        //
        // Handles initialization
        if (savedCursor == 0) {
#if defined(MENU_DEBUG)
//            if (btn->passAmp()->isMid()) {
            Serial.println(F(" Makes init move ..."));
//            }
            Serial.print(F(" Stage free heap (RAM): "));
            Serial.println(getFreeRam());
#endif

            //
            // Move menu to first index
            mci->moveUp();
            savedCursor = 1;
            delay(1);
        }

        MidCursorMenu = savedCursor;
        btn->setNavigationState(true);

#if defined(MENU_DEBUG)
        Serial.print(F("Cursor menu: "));
        Serial.println(MidCursorMenu);
        Serial.print(F("Saved Cursor menu: "));
        Serial.println(savedCursor);
        Serial.print(F("Used menu: "));
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
            Serial.print(F("Cursor MID: "));
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

const char *MenuBase::usedMenu = new char[174];
const char *MenuBase::usedBack = new char[174];
const char *MenuBase::usedNext = new char[174];
const char *MenuBase::usedDown = new char[174];


#endif //ARDUINOMID_MENUBASE_H
