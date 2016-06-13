
static void menuUsed(MenuUseEvent used);
static void menuChanged(MenuChangeEvent changed);

MenuBackend  menu = MenuBackend(menuUsed, menuChanged);

//
//
int lastButtonPushed = LOW;
int buttonLastDwSt = LOW;
int buttonLastUpSt = LOW;

//
// The debounce time
long debounceDelay = 300;
long lastEnterDebounceTime = 0;  // the last time the output pin was toggled
long lastEscDebounceTime = 0;  // the last time the output pin was toggled
long lastDebounceTimeDw = 0;  // the last time the output pin was toggled
long lastDebounceTimeUp = 0;  // the last time the output pin was toggled

MenuItem menuItem1 = MenuItem("Base");
 MenuItem menuItem1SubItem1 = MenuItem("Item1SubItem1");
 MenuItem menuItem1SubItem2 = MenuItem("Item1SubItem2");
MenuItem menuItem2 = MenuItem("Drive");
 MenuItem menuItem2SubItem1 = MenuItem("Item2SubItem1");
 MenuItem menuItem2SubItem2 = MenuItem("Item2SubItem2");
 MenuItem menuItem3SubItem3 = MenuItem("Item2SubItem3");
MenuItem menuItem3 = MenuItem("Tests");

static void setupMenu(){
    menu.getRoot().add(menuItem1).add(menuItem2).add(menuItem3);
    menuItem3.add(menuItem1); // Create Loop menu
//    menuItem1.add(menuItem1SubItem1).addRight(menuItem1SubItem2);
//    menuItem2.add(menuItem2SubItem1).addRight(menuItem2SubItem2).addRight(menuItem3SubItem3);
      //menu.next();
    }


/**
 * 
 */
void navigateMenus() {
    MenuItem currentMenu = menu.getCurrent();
    
    switch (lastButtonPushed) {
        case buttonPinUp:
            menu.moveUp();
            break;
        case buttonPinDw:
            menu.moveDown();
            break;
    }

    lastButtonPushed = 0; //reset the lastButtonPushed variable
}
/**
 *
 */
void readButtons() {  //read buttons status
    int reading;

    int buttonDwState = LOW;             // the current reading from the input pin
    int buttonUpState = LOW;             // the current reading from the input pin

    //  Down button
    //      read the state of the switch into a local variable:
    reading = digitalRead(buttonPinUp);

    if (reading != buttonLastUpSt) {
        lastDebounceTimeUp = millis();
    }

    if ((millis() - lastDebounceTimeUp) > debounceDelay) {
        buttonUpState = reading;
        lastDebounceTimeUp = millis();
    }

    buttonLastUpSt = reading;

    reading = digitalRead(buttonPinDw);

    if (reading != buttonLastDwSt) {
        lastDebounceTimeDw = millis();
    }

    if ((millis() - lastDebounceTimeDw) > debounceDelay) {
        buttonDwState = reading;
        lastDebounceTimeDw = millis();
    }

    buttonLastDwSt = reading;
    //
    // records which button has been pressed
    if (buttonUpState == HIGH) {
        lastButtonPushed = buttonPinUp;
    } else if (buttonDwState == HIGH) {
        lastButtonPushed = buttonPinDw;
    } else {
        lastButtonPushed = 0;
    }
}



/**
 *
 */
static void menuChanged(MenuChangeEvent changed) {
    
    MenuItem newMenuItem = changed.to; //get the destination menu
    lcd.clear();  
    if (newMenuItem.getName() == "Base") {
        lcd.print((char)2);
        lcd.print("--");
    } else if (newMenuItem.getName() == "Item1SubItem1") {
        lcd.print("Item1SubItem1");
    } else if (newMenuItem.getName() == "Item1SubItem2") {
        lcd.print("Item1SubItem2   ");
    } else if (newMenuItem.getName() == "Drive") {
        lcd.print("-");
        lcd.print((char)2);
        lcd.print("-");
    } else if (newMenuItem.getName() == "Item2SubItem1") {
        lcd.print("Item2SubItem1   ");
    } else if (newMenuItem.getName() == "Item2SubItem2") {
        lcd.print("Item2SubItem2   ");
    } else if (newMenuItem.getName() == "Item2SubItem3") {
        lcd.print("Item2SubItem3   ");
    } else if (newMenuItem.getName() == "Tests") {
        lcd.print("--");
        lcd.print((char)2);
    }else{
        lcd.print(newMenuItem.getName());
    }
}



static void menuUsed(MenuUseEvent used) {
    lcd.setCursor(0, 0);
    lcd.print("You used        ");
    lcd.setCursor(0, 1);
    lcd.print(used.item.getName());
    delay(3000);  //delay to allow message reading
    lcd.setCursor(0, 0);
    lcd.print("blaaa");
    //menu.toRoot();  //back to Main
}
