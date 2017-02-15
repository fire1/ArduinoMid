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

#include "MainFunc.h"
#include "CarSens.h"
#include "WhlSens.h"
#include "EepRom.h"
#include "MenuBtn.h"


#ifndef MENU_ENTRY
#define MENU_ENTRY 0
#endif

#define MENU_NAME_1 "Main"
#define MENU_NAME_11 "Panel"
#define MENU_NAME_12 "Test"
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

#define MENU_NAME_5 "Challenges"
#define MENU_NAME_51 "Stopwatch"
#define MENU_NAME_52 "Drag Racing 402m"
#define MENU_NAME_53 "From 0 to 100km"

#ifndef AWAITING
#define AWAITING 2000
#endif


static void MidMenu_menuUsed(MenuUseEvent used);

static void MidMenu_menuChanged(MenuChangeEvent changed);

/**
 *
 */
class Menu16x2 {

    MenuBtn *btn;
    MenuBackend menu;
    MenuItem
    //
    // Main menu
            mainMenu,
            dshBoardMenu,
            testingsMenu,
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
    //
    // Sprint
            gamesMenu,
            gamesStpWatch,
            gamesDragRace,
            gamesFr0To100;

    IntAmp *_amp;

    CarSens *_car;
    EepRom *_eep;


public:

    Menu16x2(MenuBtn *_btn);

    const static int MENU_ENTER = MENU_ENTRY;

    //
    // External changer var
    const static char *where;

    //
    // External changer var
    int static cursorMenu;

/**
 * Sets pins
 * @param pinUp
 * @param pinDw
 * @param pinTn
 */
    void setup(void);

/**
 * Listen buttons and navigate
 * @param whl
 * @param cursor
 */
    void listener(int &cursor);

    /**
     * Returns true if ">S" button activates "edit Оптион"
     * @return bool
     */
    boolean isEditOption(void) { return _isEditOption; }


/**
 *
 * @return int
 */
    int getCursorMenu() {
        return Menu16x2::cursorMenu;
    }

/**
 *
 * @param val
 */
    void setCursor(int val) {
        Menu16x2::cursorMenu = val;
    }

/**
 * Enables/Disable navigation of menu
 * @param active
 */
    void setNavigation(boolean active = true) {
        _isNavigation = active;
    }

/**
 *  Enables/Disable edit option
 * @param active
 */
    void setEditOption(boolean active = true) {
        _isEditOption = active;
    }

/**
 * Gets state of menu navigation
 * @return bool
 */
    boolean isNavigationActive(void) {
        return _isNavigation;
    }


    void playEntry(LiquidCrystal *lcd);

private:
    boolean enterSub = false;
    //
    // Saves cursor between changes
    int savedCursor;

    const char *activeMenu;

    uint8_t btnPinUp, btnPinDw, pinTones;


    unsigned long entryDownState = 0;
    boolean secondTone = 0;
    //
    //
    int lastButtonPushed = LOW;
    int isMainNavigationStatus = 0;
    boolean _isEditOption = false;

    boolean _isNavigation = true;

    char lastMainMenuState = 0;

    bool enterDisplay = 0;

//    void buttons(WhlSens *whl, uint8_t buttonPinUp, uint8_t buttonPinDw);
//
//    void shortcuts(WhlSens *whl);

    void navigate();


};

/***********************************************************************************************
 * ########################################################################################### *
 * ########################################################################################### *
 *                                                                                             *
 *                                   CPP part of file                                          *
 *                                                                                             *
 * ########################################################################################### *
 * ########################################################################################### *
 ***********************************************************************************************/


/**
 * constructor
 */
Menu16x2::Menu16x2(MenuBtn *_btn) :

//
// Define menus
        menu(MenuBackend(MidMenu_menuUsed, MidMenu_menuChanged)),
        //
        // Main menu initialization
        mainMenu(MenuItem(MENU_NAME_1)),
        dshBoardMenu(MenuItem(MENU_NAME_11)),
        testingsMenu(MenuItem(MENU_NAME_12)),
        //
        // Trip menu initialization
        tripMenu(MenuItem(MENU_NAME_2)),
        tripAverage(MenuItem(MENU_NAME_21)),
        tripTotalKm(MenuItem(MENU_NAME_22)),
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
        //
        // Sprint Menu
        gamesMenu(MenuItem(MENU_NAME_5)),
        gamesStpWatch(MenuItem(MENU_NAME_51)),
        gamesDragRace(MenuItem(MENU_NAME_52)),
        gamesFr0To100(MenuItem(MENU_NAME_53)) {

    btn = _btn;
    _amp = _btn->passAmp();
    _car = _btn->passCar();
    _eep = _btn->passEep();
}

/**
 * Event menu changed
 */
static void MidMenu_menuChanged(MenuChangeEvent changed) {

    MenuItem curMenuItem = changed.to; //get the destination menu

    const char *curMenuName = curMenuItem.getName();

    if (curMenuName == MENU_NAME_1) {
        Menu16x2::cursorMenu = 1;
    } else if (curMenuName == MENU_NAME_11) {
        Menu16x2::cursorMenu = 11;
    } else if (curMenuName == MENU_NAME_12) {
        Menu16x2::cursorMenu = 12;
    } else if (curMenuName == MENU_NAME_2) {
        //
        // Trip Menu
        Menu16x2::cursorMenu = 2;
    } else if (curMenuName == MENU_NAME_21) {
        Menu16x2::cursorMenu = 21;
    } else if (curMenuName == MENU_NAME_22) {
        Menu16x2::cursorMenu = 22;
        //
        // Fuel Menu
    } else if (curMenuName == MENU_NAME_3) {
        Menu16x2::cursorMenu = 3;
    } else if (curMenuName == MENU_NAME_31) {
        Menu16x2::cursorMenu = 32;

        //
        // Car State Menu
    } else if (curMenuName == MENU_NAME_4) {
        Menu16x2::cursorMenu = 4;
    } else if (curMenuName == MENU_NAME_41) {
        Menu16x2::cursorMenu = 41;
    } else if (curMenuName == MENU_NAME_42) {
        Menu16x2::cursorMenu = 42;
    } else if (curMenuName == MENU_NAME_43) {
        Menu16x2::cursorMenu = 43;
    } else if (curMenuName == MENU_NAME_44) {
        Menu16x2::cursorMenu = 44;
    } else if (curMenuName == MENU_NAME_45) {
        Menu16x2::cursorMenu = 45;
        //
        // Games menu
    } else if (curMenuName == MENU_NAME_5) {
        Menu16x2::cursorMenu = 5;
    } else if (curMenuName == MENU_NAME_51) {
        Menu16x2::cursorMenu = 51;
    } else if (curMenuName == MENU_NAME_52) {
        Menu16x2::cursorMenu = 52;
    } else if (curMenuName == MENU_NAME_53) {
        Menu16x2::cursorMenu = 53;
    }


}

/**
 *  Setup menu
 */
void Menu16x2::setup(void) {


    btnPinUp = btn->getPinUp();
    btnPinDw = btn->getPinDw();
    pinTones = btn->getPinTn();

    //
    //
    menu.getRoot().add(mainMenu).add(tripMenu).add(fuelMenu).add(statMenu).add(gamesMenu);
    gamesMenu.add(mainMenu); // add last menu to create a Loop menu
    //
    // Main menu layers
    mainMenu.addRight(dshBoardMenu).addRight(testingsMenu);
    testingsMenu.addRight(mainMenu); // loop
    // add return menu
    dshBoardMenu.add(mainMenu);
    testingsMenu.add(mainMenu);
    //
    // Trip Menu
    tripMenu.addRight(tripAverage).addRight(tripTotalKm);
    tripTotalKm.addRight(tripMenu);
    tripAverage.add(tripMenu);
    tripTotalKm.add(tripMenu);
    //
    // Fuel  layers
    fuelMenu.addRight(FuelTankMenu);
    FuelTankMenu.addRight(fuelMenu);
    FuelTankMenu.add(fuelMenu);
    //
    // Inspection menu
    statMenu.addRight(stateBrkWre).addRight(stateClnLvl)
            .addRight(stateWndWsh).addRight(stateOilLvl).addRight(stateBatVlt);
    stateBatVlt.addRight(stateBrkWre);
    stateBrkWre.add(statMenu);
    stateClnLvl.add(statMenu);
    stateWndWsh.add(statMenu);
    stateOilLvl.add(statMenu);
    stateBatVlt.add(statMenu);
    //
    // Games menu
    gamesMenu.addRight(gamesStpWatch).addRight(gamesDragRace);
    gamesDragRace.addRight(gamesStpWatch);
    //
    // Move cursor to menu
    menu.moveDown();
    menu.use();
}

/**
 * Define static cursor
 */
int Menu16x2::cursorMenu = 0;


const char *Menu16x2::where = "";

/**
 * Event use changed
 */
static void MidMenu_menuUsed(MenuUseEvent used) {
    //
    // Pass argument to class
    Menu16x2::where = used.item.getName();
}


/**
 * listen menu
 */
void Menu16x2::listener(int &cursor) {
//    buttons(whl, btnPinUp, btnPinDw);
    navigate();
    //
    //
    if (Menu16x2::where != activeMenu && Menu16x2::cursorMenu != MENU_ENTER) {
        //
        // Keep cursor in save place
        savedCursor = Menu16x2::cursorMenu;
        //
        // Change menu to show info
        Menu16x2::cursorMenu = MENU_ENTER;
    }

    cursor = Menu16x2::cursorMenu;

}

/**
 * Display menu entry
 */
void Menu16x2::playEntry(LiquidCrystal *lcd) {

    Menu16x2::cursorMenu = MENU_ENTER;
    lcd->clear();
    lcd->setCursor(0, 0);

    lcd->print("~ ");
    lcd->setCursor(15, 0);
    lcd->print((char) 246);
    lcd->setCursor(0, 0);
    tone(pinTones, 2800, 20);
    delay(100);
    lcd->print(Menu16x2::where);

    lcd->setCursor(16, 0);

    delay(300);  //delay to allow message reading
    lcd->setCursor(0, 0);

    _car->clearBaseData();
    activeMenu = Menu16x2::where;
    enterDisplay = 0;
    Menu16x2::cursorMenu = savedCursor;

    lcd->clear();
    //
    // fixes value peek
    // reset base global vars



}

/**@deprecated
 * buttons

void Menu16x2::buttons(WhlSens *whl, uint8_t buttonPinUp, uint8_t buttonPinDw) {

    lastButtonPushed = LOW;

    //
    // Checks is navigation active (default = true)
    if (_isNavigation) {
        //
        // Detect up state button
        if (!digitalRead(buttonPinUp) == HIGH) {

            if (_amp->isLow() && !digitalRead(buttonPinUp) == HIGH) {
                lastButtonPushed = buttonPinUp;
            }
        }
        //
        // Detect down state button
        if (entryDownState + AWAITING > millis() && enterSub && !digitalRead(buttonPinDw) == LOW) {
            //
            //
            tone(pinTones, 700, 20);
            delay(20);
            tone(pinTones, 700, 20);
            enterSub = false;
            //
            // Perform button is released action
            lastButtonPushed = buttonPinDw;
            //
            // Reset entry down state
            entryDownState = 0;
        }
    }



    //
    // Detect EDIT state button
    if (!digitalRead(buttonPinDw) == HIGH) {
        //
        // Controlling start of press state
        if (entryDownState == 0) {
            whl->disable();
            entryDownState = millis();
        }


        if (_amp->isMin() && !digitalRead(buttonPinDw) == HIGH) {
            enterSub = true;
        }

        //
        // Hold
        if (entryDownState + AWAITING < millis() && !digitalRead(buttonPinDw) == HIGH) {
            //
            // If is still high state [pressed]
            if (!digitalRead(buttonPinDw) == HIGH) {
                //
                // Reset entry down state
                entryDownState = 0;
                shortcuts(whl);
                //
                // Check for subMenu if not got inner level entry
                if (_isEditOption == 0) {
                    //
                    // Enter inner level menu
                    _isEditOption = 1;
                    tone(pinTones, 400, 100);
                    //
                    // Exit inner level menu
                } else if (_isEditOption == 1) {
                    _isEditOption = 0;
                    tone(pinTones, 400, 50);
                    secondTone = 1;
                }
            } else {
                //
                // Perform button is released action
                lastButtonPushed = buttonPinDw;
                //
                // Reset entry down state
                entryDownState = 0;
            }
        }

    } else { // <- deprecated
        entryDownState = 0;
        whl->enable(); // unlock radio
    }

    if (_amp->isSec() && secondTone) {
        tone(pinTones, 800, 50);
        secondTone = 0;
    }

}

/**
 * Shortcuts

void Menu16x2::shortcuts(WhlSens whl) {


    // Steering button is pressed
    if (whl->getCurrentState() == whl->STR_BTN_ATT) {
        tone(pinTones, 1000, 10);
        delay(10);
        tone(pinTones, 1000, 10);
        delay(10);

        tone(pinTones, 2500, 10);
        delay(20);
        tone(pinTones, 2500, 10);
        _eep->saveZeroingData();
        delay(20);
        whl->enable();
        return;
    }
    //
    // Change Speed alarm Up
    if (whl->getCurrentState() == whl->STR_BTN_VLU) {
        _car->speedingAlarmsUp();
        tone(pinTones, 800, 50);
        delay(50);
        tone(pinTones, 2000, 80);
        delay(80);
        whl->enable();
        return;
    }
    //
    // Change Speed alarm Down
    if (whl->getCurrentState() == whl->STR_BTN_VLD) {
        _car->speedingAlarmsDw();
        tone(pinTones, 2000, 50);
        delay(50);
        tone(pinTones, 800, 80);
        delay(80);
        whl->enable();
        return;
    }

}
*/

/**
 * Resolve navigation between button press
 */
void Menu16x2::navigate() {

    if (isMainNavigationStatus == 0) {
        if (btn->getLastBtn() == btnPinUp) {
            menu.moveDown();
            menu.use();
        }
        if (btn->getLastBtn() == btnPinDw) {
            menu.moveRight();
            menu.use();
        }
    }
    lastButtonPushed = 0; //reset the lastButtonPushed variable
}


#endif