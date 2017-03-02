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

public:
   static const char *usedMenu;

    IMidMenu *mci;

    //
    // Constructor
    MenuBase(MenuBtn *_btn, IMidMenu *_mci)  {
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
        MidCursorMenu = savedCursor;


#if defined(MENU_DEBUG) || defined(GLOBAL_SENS_DEBUG)
        Serial.print("Cursor menu: ");
        Serial.println(MidCursorMenu);
        Serial.print("Saved Cursor menu: ");
        Serial.println(savedCursor);
#endif
    }

protected:
    MenuBtn *btn;

    //
    // Saves cursor between changes
    unsigned int savedCursor;





public:
    /**
     * Perform navigation
     */
    void listener(unsigned int &cursor) {

        //
        // Handles initialization
        if (cursor == 0) {
            mci->moveUp();
        }

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
        if (cursor != savedCursor && cursor != MENU_ENTRY) {
            //
            // Keep cursor in save place
            savedCursor = cursor;
            //
            // Change menu to show screen
            cursor = MENU_ENTRY;
        }

        cursor = savedCursor; // TODO this is unusable... for delete

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


const char *MenuBase::usedMenu;

#endif //ARDUINOMID_MENUBASE_H
