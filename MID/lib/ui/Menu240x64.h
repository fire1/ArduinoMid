//
// Created by Angel Zaprianov on 23.2.2017 г..
//

#ifndef ARDUINO_MID_MENU240X64_H
#define ARDUINO_MID_MENU240X64_H

#include <Arduino.h>
#include "../../MID.h"
#include "../MenuBtn.h"
#include "../MenuBase.h"
#include "../MenuBack.h"

const uint8_t MENU_NAME_1 = 1;
const uint8_t MENU_NAME_11 = 11;
const uint8_t MENU_NAME_12 = 12;
const uint8_t MENU_NAME_13 = 13;

const uint8_t MENU_NAME_121 = 121;
const uint8_t MENU_NAME_122 = 122;
const uint8_t MENU_NAME_123 = 123;
const uint8_t MENU_NAME_124 = 124;

const uint8_t MENU_NAME_2 = 2;
const uint8_t MENU_NAME_21 = 21;
const uint8_t MENU_NAME_22 = 22;
const uint8_t MENU_NAME_4 = 4;
const uint8_t MENU_NAME_41 = 41;
const uint8_t MENU_NAME_42 = 42;
const uint8_t MENU_NAME_43 = 43;

/**
 * Building the menu
 */
class Menu240x60 : public MenuUiInterface {

    MenuBack menu;
    MenuBtn *btn;
    MenuItem
    //
    // Main menu
            mainMenu,
            gagesMenu,
            setupMenu,
            aboutMenu,

            vssSet,
            rpmSet,
            dstSet,
            ecuSet,
    //
    // Trip
            tripMenu,
            raceMenu,
    //
    // Fuel tank
            fuelMenu,
    //
    // States
            statMenu,
            voltmeter,
            dynaMeter,
            resetWork
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
                              mainMenu(MenuItem(MENU_NAME_1, 1)),
                              gagesMenu(MenuItem(MENU_NAME_11)),
                              setupMenu(MenuItem(MENU_NAME_12)),

                              vssSet(MenuItem(MENU_NAME_121)),
                              rpmSet(MenuItem(MENU_NAME_122)),
                              dstSet(MenuItem(MENU_NAME_123)),
                              ecuSet(MenuItem(MENU_NAME_124)),

                              aboutMenu(MenuItem(MENU_NAME_13)),
            //
            // Trip menu
                              tripMenu(MenuItem(MENU_NAME_2, 12)),
                              fuelMenu(MenuItem(MENU_NAME_21)),
                              raceMenu(MenuItem(MENU_NAME_22)),

            //
            // Fuels menu

            //
            // Servicing menu
                              statMenu(MenuItem(MENU_NAME_4, 14)),
                              voltmeter(MenuItem(MENU_NAME_41)),
                              resetWork(MenuItem(MENU_NAME_42)),
                              dynaMeter(MenuItem(MENU_NAME_43)) {
    }

    void begin(void) {
        menu.getRoot()
                .add(mainMenu).add(tripMenu).add(statMenu);
        statMenu.add(mainMenu);

        mainMenu.addRight(gagesMenu).addRight(setupMenu).addRight(aboutMenu);
        gagesMenu.add(mainMenu);
        setupMenu.add(vssSet).add(rpmSet).add(dstSet).add(ecuSet);
        ecuSet.add(setupMenu);
        aboutMenu.add(mainMenu);
        aboutMenu.addRight(mainMenu);

        tripMenu.addRight(fuelMenu).addRight(raceMenu);
        fuelMenu.add(tripMenu);
        raceMenu.addRight(tripMenu);
        raceMenu.add(tripMenu);

        statMenu.addRight(voltmeter).addRight(resetWork).addRight(statMenu);
        voltmeter.add(dynaMeter);
        dynaMeter.addRight(statMenu);
        resetWork.add(statMenu);


        menu.moveDown();
        MidCursorMenu = 1;

    }

    MenuBack getMB() {
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
        MidCursorMenu = curMenuItem.getName();
        return;

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
