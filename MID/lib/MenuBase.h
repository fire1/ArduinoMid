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


public:
    //
    // External changer var
    static unsigned int cursorMenu;
    //
    // Where is menu
    static const char *usedMenu;
    //
    // Constructor
    MenuBase(MenuBtn *_btn) : menu(menuUseEvent, menuChangeEvent) {
        btn = _btn;
    }

    /**
     * @deprecated
     */
    void startEntry() {
    }

    void finishEntry() {
        MenuBase::lastMenu = MenuBase::usedMenu;
        MenuBase::cursorMenu = savedCursor;
        Serial.println(MenuBase::cursorMenu);
    }

protected:
    MenuBtn *btn;
    MenuBackend menu;
    //
    // Saves cursor between changes
    unsigned int savedCursor;
    const char *lastMenu;


    /**
     * Perform navigation
     */
    void listener(int &cursor) {
        //
        // Handle navigation
        if (btn->isUp()) {
            menu.moveDown();
            menu.use();

            if (btn->passAmp()->isMid()) {
                Serial.print("Up hit \n\r");

            }
        }
        if (btn->isDw()) {
            menu.moveRight();
            menu.use();
            if (btn->passAmp()->isMid()) {
                Serial.print("Dw hit \n\r");

            }
        }
        btn->clearLastButton();
        //
        //
        if (MenuBase::lastMenu != MenuBase::usedMenu && MenuBase::cursorMenu != MENU_ENTRY) {
            //
            // Keep cursor in save place
            savedCursor = MenuBase::cursorMenu;
            //
            // Change menu to show screen
            MenuBase::cursorMenu = MENU_ENTRY;
        }
        cursor = MenuBase::cursorMenu;
    }

public:

    static void setChangeMenu( IMidMenu *iMenu ,MenuChangeEvent change){
        iMenu->menuChanged(change);
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

unsigned int MenuBase::cursorMenu;
const char *MenuBase::usedMenu;



//
// Event method set
void menuUseEvent(MenuUseEvent used) {
    //
    // Pass argument to class
    MenuBase::usedMenu = used.item.getName();
}


#endif //ARDUINOMID_MENUBASE_H
