




const byte SqWavePin = 4;                 // Square wave genereated from digital pin 4
const byte Button1 = 6;                   // Pushbutton switch D6, connected to ground
const byte Button2 = 14;                  // Pushbutton switch A0, connected to ground
const byte Button3 = 15;                  // Pushbutton switch A1, connected to ground
const byte Button4 = 16;                  // Pushbutton switch A2, connected to ground

byte MenuMode = 0;                        // MenuMode 0 means not in a menu

boolean MenuModeActive = false;           // Only goes true when a menu item has been selected

unsigned long ButtonsOffTime;             // How long since last button was released

boolean Button1State;                     // State of Button1 from last cycle
boolean Button2State;                     // State of Button2 from last cycle
boolean Button3State;                     // State of Button3 from last cycle
boolean Button4State;                     // State of Button4 from last cycle
boolean ButtonPressed = false;            // Becomes true when a button press is detected
boolean ButtonPressedPrev = false;        // Previous value of ButtonPressed

int CalDistPrev;                          // Previous value of CalDist during calibration

int SensorCount = 0;                      // Speed Sensor pulse count
int TimerCount = 0;                       // Timer2 overflow count
int Freq;                                 // Copy of SensorCount, and is the actual vehicle speed
boolean Trigger = false;                  // When it becomes true it triggers the print cycle
boolean SqWaveOut = false;                // Used to toggle output state of pin 4
int FreqPrev1 = 0;                        // Previous value of Freq, used for averaging speed
int FreqPrev2 = 0;                        // Another previous value of Freq, used for averaging speed
int Speed;                                // Average of all 3 Freq variables, and is what is displayed on LCD

int DistanceMiles;                        // Cumulative distance travelled in miles
int DistanceTenths;                       // Cumulative distance travelled, tenths digit
int DistanceClicks;                       // Cumulative speed sensor pulses received, reduced by CalDist each 1/10 mile
unsigned long TotalMiles;                 // Total distance travelled in miles
int TotalTenths;                          // Total distance travelled, tenths digit
int TotalClicks;                          // Total speed sensor pulses received, reduced by CalDist each 1/10 mile

long DistanceFeet;                        // Cumulative distance travelled in feet, temporary display option
unsigned int DistanceFeetTenths;          // Cumulative distance travelled in feet, tenths digit, temporary display option
int FeetScaleFactor;                      // Scale Factor used to calculate feet from speed sensor pulses


ISR(TIMER2_OVF_vect) {                    // This is the Interrupt Service Routine, called when Timer2 overflows

    TimerCount +=8;                         // Increment TimerCount by 8
    if (TimerCount > CalValue)  {           // Check to see if it's time to display the count
      Freq = SensorCount;                   // Copies the interrupt 0 count into variable "Freq"
      Trigger = true;                       // sets "Trigger" to let the main loop know it's time to print data
      SensorCount = 0;                      // Reset the interrupt 0 count
      TimerCount -= CalValue;               // Reset Timer2 count, but keep the excess for next time around
    }
    SqWaveOut = !SqWaveOut;                 // Toggle SqWaveOut to its opposite state
    digitalWrite(SqWavePin, SqWaveOut);     // Set pin 4 to the new state of SqWaveOut

}


void setup()  {

// turn on DigitalPin 13 LED to signal that the custom characters have been loaded into the LCD
  pinMode(13, OUTPUT);
  loadchars();
  digitalWrite(13, HIGH);

  pinMode(SqWavePin, OUTPUT);                  // Output on pin 4 will be oscillating
  pinMode(Button1, INPUT);                     // Set Button1 pin as input
  pinMode(Button2, INPUT);                     // Set Button2 pin as input
  pinMode(Button3, INPUT);                     // Set Button3 pin as input
  pinMode(Button4, INPUT);                     // Set Button4 pin as input
  digitalWrite(Button1, HIGH);                 // Turn on Button1's internal pull-up resistor
  digitalWrite(Button2, HIGH);                 // Turn on Button2's internal pull-up resistor
  digitalWrite(Button3, HIGH);                 // Turn on Button3's internal pull-up resistor
  digitalWrite(Button4, HIGH);                 // Turn on Button4's internal pull-up resistor

  CalDist = (int(EEPROM.read(101)) * 256) + (int(EEPROM.read(102)));  // read CalDist out of EEPROM
  CalValue = int((2812500.0 / CalDist) + .5);               // calculate CalValue from CalDist

  // set up the LCD's number of columns and rows:
  lcd.begin(20,2);

  // read total mileage out of EEPROM
  TotalMiles = (EEPROM.read(5) * 65536) + (EEPROM.read(6) * 256) + (EEPROM.read(7));
  TotalTenths = EEPROM.read(8);
  TotalClicks = (EEPROM.read(9) * 256) + (EEPROM.read(10));

  // read Distance out of EEPROM
  DistanceMiles = (EEPROM.read(0) * 256) + (EEPROM.read(1));
  DistanceTenths = EEPROM.read(2);
  DistanceClicks = (EEPROM.read(3) * 256) + (EEPROM.read(4));

// print the speed in big font
  lcd.setCursor(0, 0);                  // set the cursor to (13,0)
  Freq = Freq % 1000;                   // drop any digits above 999
  printbigchar(int(Freq / 100),0);      // print the speed hundreds
  Freq = Freq % 100;                    // drop any digits above 99
  printbigchar(int(Freq / 10),1);       // print the speed tens
  Freq = Freq % 10;                     // drop any digits above 9
  printbigchar(Freq,2);                 // print the speed ones
  // write Distance to LCD
  printmileage();

  attachInterrupt(0, AddSensorCount, RISING);  // Interrupt 0 is on digital pin 2
  attachInterrupt(1, EEPROMwrite, FALLING);    // Interrupt 1 is on digital pin 3

  //  -------------------------------------------------------------------------------------
  //  This bit of code is adapted from an article by dfowler at uchobby.com
  //  http://www.uchobby.com/index.php/2007/11/24/arduino-interrupts/

  //Timer2 Settings: Timer Prescaler / 64, mode 0
  //Timmer clock = 16 MHz / 64 = 250 KHz or 0.5us
  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 0<<CS21 | 0<<CS20;      // Set Timer2 frequency to 250 KHz
  // Used to be 010 for 2 MHz clock

  //Timer2 Overflow Interrupt Enable
  TIMSK2 = 1<<TOIE2;


  //  -------------------------------------------------------------------------------------

  ButtonsOffTime = millis();                 // Set ButtonStateTime to current time

}