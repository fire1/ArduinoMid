//
// Created by Angel Zaprianov on 23.2.2017 г..
//

#ifndef ARDUINO_MID_MENU_BASE_H
#define ARDUINO_MID_MENU_BASE_H

//#define MENU_DEBUG // Uncomment to get info

#include <Arduino.h>
#include "MenuBackend.h"
#include "../MID.h"
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
    MenuUiInterface *mci;
    MenuBtn *btn;

public:
    //
    // Constructor
    MenuBase(MenuBtn &_btn, MenuUiInterface &_mci) : btn(&_btn), mci(&_mci) {
        MenuBase_savedCursor = 1;
    }


    /**
     * Used to activate / deactivate navigation
     */
    void startEntry() {
//        resRam.listen();
        btn->setNavigationState(false);
    }

    /**
     * Restore position of menu
     */
    void finishEntry() {

        //
        // Handles initialization
        if (MenuBase_savedCursor == 0) {

            //
            // Move menu to first index
            mci->moveUp();
            MenuBase_savedCursor = 1;
        }

        MidCursorMenu = MenuBase_savedCursor;
        btn->setNavigationState(true);
        //
        // Fixes infinity loop from entry if cursor is lose
        if (MidCursorMenu == 0) {
            MidCursorMenu = 1;
        }
#ifdef DEBUG
        if (cmdLive(DBG_SR_MNI)) {
            dump("Current cursor menu", MidCursorMenu);
            dump("Saved cursor menu", MenuBase_savedCursor);
            dump("Used menu", usedMenu.used);
        }
#endif
    }

    void triggerMoveUp() {
        mci->moveUp();
    }

    void triggerMoveDw() {
        mci->moveDw();
    }

    /**
     * Perform navigation
     */
    void listener() {

        //
        // Handle navigation
        if (btn->isUp() && !btn->isHl()) {
            mci->moveUp();
#ifdef DEBUG
            if (cmdLive(DBG_SR_MNI)) {
                dump_txt("Up hit ");
            }
#endif
        }
        if (btn->isDw()) {
            mci->moveDw();
#ifdef DEBUG
            if (cmdLive(DBG_SR_MNI)) {
                dump_txt("Dw hit ");
            }
#endif
        }


        btn->clearLastButton();

#ifdef DEBUG
        if (cmd(btn->passAmp(), DBG_SR_MNI)) {
            dump("Cursor saved", MenuBase_savedCursor);
        }
#endif
        //
        //
        if (MenuBase_savedCursor != MidCursorMenu && MidCursorMenu != MENU_ENTRY) {
            //
            // Keep cursor in save place
            MenuBase_savedCursor = MidCursorMenu;
            //
            // Change menu to dump screen
            MidCursorMenu = MENU_ENTRY;
        }
    }


};


#endif //ARDUINOMID_MENUBASE_H
