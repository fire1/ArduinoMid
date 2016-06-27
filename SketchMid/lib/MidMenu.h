//
// Created by Angel Zaprianov on 27.6.2016 г..
//

static void menuUsed (MenuUseEvent used);

static void menuChanged (MenuChangeEvent changed);

MenuBackend menu = MenuBackend (menuUsed, menuChanged);

//
//
int lastButtonPushed = LOW;
int buttonLastDwSt = LOW;
int buttonLastUpSt = LOW;

//
// The debounce time
long debounceDelay = 500;
long lastEnterDebounceTime = 0;  // the last time the output pin was toggled
long lastEscDebounceTime = 0;  // the last time the output pin was toggled
long lastDebounceTimeDw = 0;  // the last time the output pin was toggled
long lastDebounceTimeUp = 0;  // the last time the output pin was toggled

MenuItem menuItem1 = MenuItem ("Menu1");
MenuItem menuItem2 = MenuItem ("Menu2");
MenuItem menuItem3 = MenuItem ("Menu3");

static void setupMenu ()
{
  menu.getRoot ().add (menuItem1).add (menuItem2).add (menuItem3);
  menuItem3.add (menuItem1); // Create Loop menu
}

/**
 * Resolve navigation between button press
 */
void navigateMenus ()
{
  MenuItem currentMenu = menu.getCurrent ();

  switch (lastButtonPushed)
    {
      case buttonPinUp:
        menu.moveDown ();
      menu.use ();
      break;
      case buttonPinDw:
        //
        // Todo menu.moveRight(); menu.use();
        break;
    }

  lastButtonPushed = 0; //reset the lastButtonPushed variable
}

/**
 * Read pin button states
 */
void readButtons ()
{  //read buttons status
  int reading;

  int buttonDwState = LOW;             // the current reading from the input pin
  int buttonUpState = LOW;             // the current reading from the input pin

  //  Down button
  //      read the state of the switch into a local variable:
  reading = digitalRead (buttonPinUp);

  if (reading != buttonLastUpSt)
    {
      lastDebounceTimeUp = millis ();
    }

  if ((millis () - lastDebounceTimeUp) > debounceDelay)
    {
      buttonUpState = reading;
      lastDebounceTimeUp = millis ();
    }

  buttonLastUpSt = reading;

  reading = digitalRead (buttonPinDw);

  if (reading != buttonLastDwSt)
    {
      lastDebounceTimeDw = millis ();
    }

  if ((millis () - lastDebounceTimeDw) > debounceDelay)
    {
      buttonDwState = reading;
      lastDebounceTimeDw = millis ();
    }

  buttonLastDwSt = reading;
  //
  // records which button has been pressed
  if (buttonUpState == HIGH)
    {
      lastButtonPushed = buttonPinUp;
    }
  else if (buttonDwState == HIGH)
    {
      lastButtonPushed = buttonPinDw;
    }
  else
    {
      lastButtonPushed = 0;
    }
}

void printNavMenuA ()
{
  lcd.setCursor (13, 2);
  lcd.print ((char) 3);
  lcd.print ((char) 2);
  lcd.print ((char) 2);
  lcd.setCursor (0, 0);

}

void printNavMenuB ()
{
  lcd.setCursor (13, 2);
  lcd.print ((char) 2);
  lcd.print ((char) 3);
  lcd.print ((char) 2);
  lcd.setCursor (0, 0);

}

void printNavMenuC ()
{
  lcd.setCursor (13, 2);
  lcd.print ((char) 2);
  lcd.print ((char) 2);
  lcd.print ((char) 3);
  lcd.setCursor (0, 0);

}

int lastNavInfoTime = 0;

int isInfoTimeMenu ()
{

  while (1 > 0)
    {
      if (millis () > lastNavInfoTime)
        {
          lastNavInfoTime = millis ();
          return 1;
        }
      if (millis () > lastNavInfoTime + 2000)
        {
          return 0;
        }
      return 0;
    }
}

/**
 *
 */
static void menuChanged (MenuChangeEvent changed)
{

  MenuItem newMenuItem = changed.to; //get the destination menu
  lcd.clear ();
  if (newMenuItem.getName () == "Menu1")
    {
      printNavMenuA ();
      cursorMenu = 1;
    }
  else if (newMenuItem.getName () == "Item1SubItem1")
    {
      lcd.print ("Item1SubItem1");
    }
  else if (newMenuItem.getName () == "Item1SubItem2")
    {
      lcd.print ("Item1SubItem2   ");
    }
  else if (newMenuItem.getName () == "Menu2")
    {
      printNavMenuB ();
      cursorMenu = 2;
    }
  else if (newMenuItem.getName () == "Item2SubItem1")
    {
      lcd.print ("Item2SubItem1   ");
    }
  else if (newMenuItem.getName () == "Item2SubItem2")
    {
      lcd.print ("Item2SubItem2   ");
    }
  else if (newMenuItem.getName () == "Item2SubItem3")
    {
      lcd.print ("Item2SubItem3   ");
    }
  else if (newMenuItem.getName () == "Menu3")
    {
      printNavMenuC ();
      cursorMenu = 3;
    }
  else
    {
      lcd.print (newMenuItem.getName ());
    }
}

static void menuUsed (MenuUseEvent used)
{
  lcd.setCursor (0, 0);
  //lcd.print("You are in:       ");
  //lcd.setCursor(0, 1);
  lcd.print (used.item.getName ());
  delay (1000);  //delay to allow message reading
  lcd.setCursor (0, 0);
  lcd.clear ();
  //menu.toRoot();  //back to Main
}
