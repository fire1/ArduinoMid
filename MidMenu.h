#ifndef MidMenu_h
#define MidMenu_h

#include "Arduino.h"
/*
IMPORTANT: to use the menubackend library by Alexander Brevig download it at http://www.arduino.cc/playground/uploads/Profiles/MenuBackend_1-4.zip and add the next code at line 195
	void toRoot() {
		setCurrent( &getRoot() );
	}
*/
#include <MenuBackend.h>    //MenuBackend library - copyright by Alexander Brevig

class MidMenu {
    int buttonUp, buttonDw;

public:
    void setButtons(int, int);
    void navigateMenus() {
        MenuItem currentMenu = menu.getCurrent();

        switch (lastButtonPushed) {
            case buttonPinEnter:
                if (!(currentMenu.moveDown())) {  //if the current menu has a child and has been pressed enter then menu navigate to item below
                    menu.use();
                } else {  //otherwise, if menu has no child and has been pressed enter the current menu is used
                    menu.moveDown();
                }
                break;
            case buttonPinEsc:
                menu.toRoot();  //back to main
                break;
            case buttonPinRight:
                menu.moveRight();
                break;
            case buttonPinLeft:
                menu.moveLeft();
                break;
        }

        lastButtonPushed = 0; //reset the lastButtonPushed variable
    }

public:
    void readButtons() {  //read buttons status
        int reading;
        int buttonEnterState = LOW;             // the current reading from the Enter input pin
        int buttonEscState = LOW;             // the current reading from the input pin
        int buttonLeftState = LOW;             // the current reading from the input pin
        int buttonRightState = LOW;             // the current reading from the input pin

        //Enter button
        // read the state of the switch into a local variable:
        reading = digitalRead(buttonPinEnter);

        // check to see if you just pressed the enter button
        // (i.e. the input went from LOW to HIGH),  and you've waited
        // long enough since the last press to ignore any noise:

        // If the switch changed, due to noise or pressing:
        if (reading != lastButtonEnterState) {
            // reset the debouncing timer
            lastEnterDebounceTime = millis();
        }

        if ((millis() - lastEnterDebounceTime) > debounceDelay) {
            // whatever the reading is at, it's been there for longer
            // than the debounce delay, so take it as the actual current state:
            buttonEnterState = reading;
            lastEnterDebounceTime = millis();
        }

        // save the reading.  Next time through the loop,
        // it'll be the lastButtonState:
        lastButtonEnterState = reading;


        //Esc button
        // read the state of the switch into a local variable:
        reading = digitalRead(buttonPinEsc);

        // check to see if you just pressed the Down button
        // (i.e. the input went from LOW to HIGH),  and you've waited
        // long enough since the last press to ignore any noise:

        // If the switch changed, due to noise or pressing:
        if (reading != lastButtonEscState) {
            // reset the debouncing timer
            lastEscDebounceTime = millis();
        }

        if ((millis() - lastEscDebounceTime) > debounceDelay) {
            // whatever the reading is at, it's been there for longer
            // than the debounce delay, so take it as the actual current state:
            buttonEscState = reading;
            lastEscDebounceTime = millis();
        }

        // save the reading.  Next time through the loop,
        // it'll be the lastButtonState:
        lastButtonEscState = reading;


        //Down button
        // read the state of the switch into a local variable:
        reading = digitalRead(buttonPinRight);

        // check to see if you just pressed the Down button
        // (i.e. the input went from LOW to HIGH),  and you've waited
        // long enough since the last press to ignore any noise:

        // If the switch changed, due to noise or pressing:
        if (reading != lastButtonRightState) {
            // reset the debouncing timer
            lastRightDebounceTime = millis();
        }

        if ((millis() - lastRightDebounceTime) > debounceDelay) {
            // whatever the reading is at, it's been there for longer
            // than the debounce delay, so take it as the actual current state:
            buttonRightState = reading;
            lastRightDebounceTime = millis();
        }

        // save the reading.  Next time through the loop,
        // it'll be the lastButtonState:
        lastButtonRightState = reading;


        //Up button
        // read the state of the switch into a local variable:
        reading = digitalRead(buttonPinLeft);

        // check to see if you just pressed the Down button
        // (i.e. the input went from LOW to HIGH),  and you've waited
        // long enough since the last press to ignore any noise:

        // If the switch changed, due to noise or pressing:
        if (reading != lastButtonLeftState) {
            // reset the debouncing timer
            lastLeftDebounceTime = millis();
        }

        if ((millis() - lastLeftDebounceTime) > debounceDelay) {
            // whatever the reading is at, it's been there for longer
            // than the debounce delay, so take it as the actual current state:
            buttonLeftState = reading;
            lastLeftDebounceTime = millis();
        }

        // save the reading.  Next time through the loop,
        // it'll be the lastButtonState:
        lastButtonLeftState = reading;

        //records which button has been pressed
        if (buttonEnterState == HIGH) {
            lastButtonPushed = buttonPinEnter;

        } else if (buttonEscState == HIGH) {
            lastButtonPushed = buttonPinEsc;

        } else if (buttonRightState == HIGH) {
            lastButtonPushed = buttonPinRight;

        } else if (buttonLeftState == HIGH) {
            lastButtonPushed = buttonPinLeft;

        } else {
            lastButtonPushed = 0;
        }
    }

private:
    int _pin;
};

#endif
