//
// Created by Angel Zaprianov on 23.2.2017 г..
//

#ifndef ARDUINO_MID_MENUBASE_H
#define ARDUINO_MID_MENUBASE_H

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
class MenuBase : virtual public IMidMenu {

    MenuBtn *btn;
public:

    const static int MENU_ENTER = MENU_ENTRY;

    //
    // External changer var
    static const char *where;

    //
    // External changer var
    static int cursorMenu;

    //
    // Constructor
    MenuBase(MenuBtn *_btn, cb_use callUse, cb_change callChange) : menu(callUse, callChange) {
        btn = _btn;
    }


    void startEntry() {
        MenuBase::cursorMenu = MENU_ENTER;
        Serial.print(MenuBase::cursorMenu);
    }

    void finishEntry() {
        activeMenu = MenuBase::where;
        MenuBase::cursorMenu = savedCursor;
    }


protected:
    MenuBackend menu;
    //
    // Saves cursor between changes
    int savedCursor;
    const char *activeMenu;


    /**
     * Perform navigation
     */
    void listener(int &cursor) {


        //
        // Handle navigation
        if (btn->isUp()) {
            menu.moveDown();
            menu.use();

            if(btn->passAmp()->isMid()){
                Serial.print("Up hit \n\r");

            }
        }
        if (btn->isDw()) {
            menu.moveRight();
            menu.use();
        }
        btn->clearLastButton();
        //
        //
        if (MenuBase::where != activeMenu && MenuBase::cursorMenu != MENU_ENTER) {
            //
            // Keep cursor in save place
            savedCursor = MenuBase::cursorMenu;
            //
            // Change menu to show info
            MenuBase::cursorMenu = MENU_ENTER;
        }
        cursor = MenuBase::cursorMenu;
    }


public:

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

int MenuBase::cursorMenu = 1;
const char *MenuBase::where = "";


#endif //ARDUINOMID_MENUBASE_H
