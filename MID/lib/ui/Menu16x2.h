//
// Created by Angel Zaprianov on 27.6.2016 г..
//


#ifndef ARDUINO_MID_MENU
#define ARDUINO_MID_MENU
//
//  MenuBackend library - copyright by Alexander Brevig
// Import it from:
// https://github.com/WiringProject/Wiring/tree/master/framework/libraries/MenuBackend
//
// And good example can be found here:
// https://codebender.cc/sketch:37125#MenuBackend_sample.ino
#include <MenuBackend.h>
#include "../../MID.h"
#include "../MainFunc.h"
#include "../CarSens.h"
#include "../WhlSens.h"
#include "../EepRom.h"
#include "../MenuBtn.h"

//
// Define menu tree
#ifndef MENU_ENTRY
#define MENU_ENTRY 0
#endif
//
// Main menu [Home]
#define MENU_NAME_1 "Main"
#define MENU_NAME_11 "Panel"
#define MENU_NAME_12 "Reset"
#define MENU_NAME_13 "Tests"
#define MENU_NAME_14 "About"
//
//
#define MENU_NAME_2 "Trip"
#define MENU_NAME_21 "Trip Average"
#define MENU_NAME_22 "Total Distance"
//
//
#define MENU_NAME_3 "Fuel"
#define MENU_NAME_31 "Fuel Level"
//
//
#define MENU_NAME_4 "Servicing"
#define MENU_NAME_41 "Brake wear "
#define MENU_NAME_42 "Coolant level"
#define MENU_NAME_43 "Window washer "
#define MENU_NAME_44 "Oil level "
#define MENU_NAME_45 "Voltage "
#define MENU_NAME_46 "Wear "


//#define MENU_NAME_5 "Challenges"
//#define MENU_NAME_51 "Stopwatch"
//#define MENU_NAME_52 "Drag Racing 402m"
//#define MENU_NAME_53 "From 0 to 100km"





/**
 *
 */
class Menu16x2 : public MenuUiInterface {
    MenuBackend menu;

    //
    // Defining menu items
    MenuItem
    //
    // Main menu
            mainMenu,
            dshBoardMenu,
            resetCurMenu,
            testingsMenu,
            aboutInfMenu,
    //
    // Trip menu
            tripMenu,
            tripAverage,
            tripTotalKm,
    //
    // Fuel tank
            fuelMenu,
            FuelTankMenu,
    //
    // States
            statMenu,
            stateBrkWre,
            stateClnLvl,
            stateWndWsh,
            stateOilLvl,
            stateBatVlt,
            stateWearDt;


// TODO Lowering code for sRAM exhausting
/*
    //
    // Sprint
            gamesMenu,
            gamesStpWatch,
            gamesDragRace,
            gamesFr0To100;

*/
public:
    //
    // Construct Menu Base
    Menu16x2() : menu(menuUseEvent, menuChangeEvent),//  base menu initialization
            //
            // Main menu initialization
                 mainMenu(MenuItem(MENU_NAME_1)),
                 dshBoardMenu(MenuItem(MENU_NAME_11)),
                 resetCurMenu(MenuItem(MENU_NAME_12)),
                 testingsMenu(MenuItem(MENU_NAME_13)),
                 aboutInfMenu(MenuItem(MENU_NAME_14)),
            //
            // Trip menu initialization
                 tripMenu(MenuItem(MENU_NAME_2)),
                 tripAverage(MenuItem(MENU_NAME_21)),
                 tripTotalKm(MenuItem(MENU_NAME_22)),
            //
            // Fuels menu
                 fuelMenu(MenuItem(MENU_NAME_3)),
                 FuelTankMenu(MenuItem(MENU_NAME_31)),
            //
            // Car State menu
                 statMenu(MenuItem(MENU_NAME_4)),
                 stateBrkWre(MenuItem(MENU_NAME_41)),
                 stateClnLvl(MenuItem(MENU_NAME_42)),
                 stateWndWsh(MenuItem(MENU_NAME_43)),
                 stateOilLvl(MenuItem(MENU_NAME_44)),
                 stateBatVlt(MenuItem(MENU_NAME_45)),
                 stateWearDt(MenuItem(MENU_NAME_46))
/*            //
            // Challenges Menu
            gamesMenu(MenuItem(MENU_NAME_5)),
            gamesStpWatch(MenuItem(MENU_NAME_51)),
            gamesDragRace(MenuItem(MENU_NAME_52)),
            gamesFr0To100(MenuItem(MENU_NAME_53)) */
    {
    }

    MenuBackend getMB() {
        return menu;
    }

/**
 * Sets pins
 * @param pinUp
 * @param pinDw
 * @param pinTn
 */
    void setup(void) {
        //
        // Base tree construction
        menu.getRoot()
                .add(mainMenu).add(tripMenu).add(fuelMenu).add(statMenu)/*.add(gamesMenu)*/;
        /*gamesMenu.add(mainMenu); // add last menu to create a Loop menu*/
        statMenu.add(mainMenu); // add last menu to create a Loop menu
        //
        // Home menu layers construction
        mainMenu.addRight(dshBoardMenu).addRight(resetCurMenu).addRight(testingsMenu).addRight(aboutInfMenu);
        aboutInfMenu.addRight(mainMenu); // loop
        // add return menu
        dshBoardMenu.add(mainMenu);
        resetCurMenu.add(mainMenu);
        testingsMenu.add(mainMenu);
        aboutInfMenu.add(mainMenu);
        //
        // Trip layers construction
        tripMenu.addRight(tripAverage).addRight(tripTotalKm);
        tripTotalKm.addRight(tripMenu);
        tripAverage.add(tripMenu);
        tripTotalKm.add(tripMenu);
        //
        // Fuel  layers construction
        fuelMenu.addRight(FuelTankMenu);
        FuelTankMenu.addRight(fuelMenu);
        FuelTankMenu.add(fuelMenu);
        //
        // Inspection layers construction
        statMenu.addRight(stateBrkWre).addRight(stateClnLvl)
                .addRight(stateWndWsh).addRight(stateOilLvl).addRight(stateBatVlt).addRight(stateWearDt);
        stateWearDt.addRight(stateBrkWre);
        stateBrkWre.add(statMenu);
        stateClnLvl.add(statMenu);
        stateWndWsh.add(statMenu);
        stateOilLvl.add(statMenu);
        stateBatVlt.add(statMenu);
        stateWearDt.add(statMenu);
        /*
        //
        // Games menu layers construction
        gamesMenu.addRight(gamesStpWatch).addRight(gamesDragRace);
        gamesDragRace.addRight(gamesStpWatch);
         */
        menu.moveDown();
        MidCursorMenu = 1;
    };


    void menuChanged(MenuChangeEvent change) {
        MenuItem curMenuItem = change.to; //get the destination menu
        const char *curMenuName = curMenuItem.getName();

/*        Serial.print("Change menu: ");
        Serial.println(curMenuName);*/

        if (curMenuName == MENU_NAME_1) {
            MidCursorMenu = 1;
        } else if (curMenuName == MENU_NAME_11) {
            MidCursorMenu = 11;
        } else if (curMenuName == MENU_NAME_12) {
            MidCursorMenu = 12;
        } else if (curMenuName == MENU_NAME_13) {
            MidCursorMenu = 13;
        } else if (curMenuName == MENU_NAME_14) {
            MidCursorMenu = 14;
        } else if (curMenuName == MENU_NAME_2) {
            //
            // Trip Menu
            MidCursorMenu = 2;
        } else if (curMenuName == MENU_NAME_21) {
            MidCursorMenu = 21;
        } else if (curMenuName == MENU_NAME_22) {
            MidCursorMenu = 22;
            //
            // Fuel Menu
        } else if (curMenuName == MENU_NAME_3) {
            MidCursorMenu = 3;
        } else if (curMenuName == MENU_NAME_31) {
            MidCursorMenu = 32;
            //
            // Car State Menu
        } else if (curMenuName == MENU_NAME_4) {
            MidCursorMenu = 4;
        } else if (curMenuName == MENU_NAME_41) {
            MidCursorMenu = 41;
        } else if (curMenuName == MENU_NAME_42) {
            MidCursorMenu = 42;
        } else if (curMenuName == MENU_NAME_43) {
            MidCursorMenu = 43;
        } else if (curMenuName == MENU_NAME_44) {
            MidCursorMenu = 44;
        } else if (curMenuName == MENU_NAME_45) {
            MidCursorMenu = 45;
        } else if (curMenuName == MENU_NAME_46) {
            MidCursorMenu = 46;

            /*
            //
            // Games menu
        } else if (curMenuName == MENU_NAME_5) {
            MidCursorMenu = 5;
        } else if (curMenuName == MENU_NAME_51) {
            MidCursorMenu = 51;
        } else if (curMenuName == MENU_NAME_52) {
            MidCursorMenu = 52;
        } else if (curMenuName == MENU_NAME_53) {
            MidCursorMenu = 53;*/
        }
    }

//
// Sets construction movements

    void moveUp() {
//        Serial.println("Up trigger");
        menu.moveDown();
        menu.use();
    }

    void moveDw() {
//        Serial.println("Dw trigger");
        menu.moveRight();
        menu.use();
    }

};


#endif