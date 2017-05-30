//
// Created by Angel Zaprianov on 23.2.2017 г..
//

#ifndef ARDUINO_MID_MENU240X64_H
#define ARDUINO_MID_MENU240X64_H

#include <Arduino.h>
#include "../../MID.h"
#include "../MenuBtn.h"
#include "../MenuBase.h"
#include <MenuBackend.h>


#define MENU_NAME_1 "Home"
#define MENU_NAME_11 "Dash"
#define MENU_NAME_12 "Info"

#define MENU_NAME_2 "Trip"
#define MENU_NAME_3 "Fuel"
#define MENU_NAME_4 "State"


/**
 *
 */
class Menu240x60 : public MenuUiInterface {

    MenuBackend menu;
    MenuBtn *btn;
    MenuItem
    //
    // Main menu
            mainMenu,
            dshBoardMenu,
            testingsMenu,
    //
    // Trip
            tripMenu,
    //
    // Fuel tank
            fuelMenu,
    //
    // States
            statMenu
    //
    // Sprint
//            gamesMenu
    ;


public:
/**
 * Menu constructor
 */
    Menu240x60(MenuBtn &_b) : btn(&_b), menu(menuUseEvent, menuChangeEvent),//  base menu initialization

            //
            // Main menu
                              mainMenu(MenuItem(MENU_NAME_1)),
                              dshBoardMenu(MenuItem(MENU_NAME_11)),
                              testingsMenu(MenuItem(MENU_NAME_12)),
            //
            // Trip menu
                              tripMenu(MenuItem(MENU_NAME_2)),
            //
            // Fuels menu
                              fuelMenu(MenuItem(MENU_NAME_3)),
            //
            // Servicing menu
                              statMenu(MenuItem(MENU_NAME_4)) {
    }

    void setup(void) {
        menu.getRoot()
                .add(mainMenu).add(tripMenu).add(fuelMenu).add(statMenu);
        statMenu.add(mainMenu);

        mainMenu.addRight(dshBoardMenu).addRight(testingsMenu);
        dshBoardMenu.add(mainMenu);
        testingsMenu.add(mainMenu);
        testingsMenu.addRight(mainMenu);

        menu.moveDown();
        MidCursorMenu = 1;

    }

    MenuBackend getMB() {
        return menu;
    }


    void menuChanged(MenuChangeEvent change) {
        //
        // Check is navigation is active
        if (btn->getNavigationState() == 0) {
            return;
        }

        //
        // Resolve
        MenuItem curMenuItem = change.to; //get the destination menu
        const char *curMenuName = curMenuItem.getName();

        Serial.println(curMenuName);


        if (curMenuName == MENU_NAME_1) {
            MidCursorMenu = 1;
            //
            // Sub menu / Dashboard
        } else if (curMenuName == MENU_NAME_11) {
            MidCursorMenu = 11;
        } else if (curMenuName == MENU_NAME_12) {
            MidCursorMenu = 12;
            //
            // Trip
        } else if (curMenuName == MENU_NAME_2) {
            MidCursorMenu = 2;
            //
            // Fuel
        } else if (curMenuName == MENU_NAME_3) {
            MidCursorMenu = 3;
            //
            // State
        } else if (curMenuName == MENU_NAME_4) {
            MidCursorMenu = 4;
        }
    }

    void moveUp() {
        menu.moveDown();
        menu.use();

    }

    void moveDw() {
        menu.moveRight();
        menu.use();

    }
};


#endif //ARDUINOMID_MENU240X64_H