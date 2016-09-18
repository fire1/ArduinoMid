const int numReadings = 10; // Need to do some smoothing of the readings for fuel level
// Define the number of samples to keep track of. The higher the number,
// the more the readings will be smoothed, but the slower the output will
// respond to the input. Using a constant rather than a normal variable lets
// use this value to determine the size of the readings array.
int readings[numReadings]; // the readings from the analog input
int index = 0; // the index of the current reading
int total = 0; // the running total
int averagefuel = 0; // the average of fuel reading as an analog input (calculated)
int percentage = 0; // percentage of tank capacity remaining (calculated)
int inputPin = A0; // analog input from fuel sendor (measured)
int fuelled = 10; // led write location for fuel led warning light (defined)
int vout = 0; // fuel sensor voltage from analog input (calculated)
int rtwo = 0; // resistance across fuel sending unit (calculated)
int oilled = 8; // led write location for oil led (defined)
int oilvin = A2; // analog input for oil sensor (defined)
int oilvolt = 0; // oil sensor voltage from analog input (actual analog input, not converted to an actual voltage)
int watervin = A1; // location for water sensor input (defined)
int waterled = 9; // led write location for water temp led (defined)
int watervolt = 0; // water sensor voltage from analog input (actual analog input, not converted to an actual voltage)
#include // inclusion of lcd library
LiquidCrystal lcd(12, 11, 5, 4, 3, 2); // initialize the library with the numbers of the interface pins
#include // inclusion of bargraph library
byte lcdNumCols = 16; // -- number of columns in the LCD
LcdBarGraph lbg(&lcd, 16, 0, 1); // -- creating bargraph instance
void setup() {
  Serial.begin(9600); // initialize serial communication with computer:
  for(int thisReading = 0; thisReading < numReadings; thisReading++) // initialize all the readings to 0:
    readings[thisReading] = 0;
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  pinMode(0, INPUT); // configure analog pins as inputs
  pinMode(1, INPUT);
  pinMode(2, INPUT);
}
void loop() {
  total = total - readings[index]; // subtract the last reading:
  readings[index] = analogRead(inputPin); // read from the sensor:
  total = total + readings[index]; // add the reading to the total:
  index = index + 1; // advance to the next position in the array:
  if(index >= numReadings) // if we're at the end of the array...
    index = 0; // ...wrap around to the beginning:
  averagefuel = total / numReadings; // calculate the average:

  float vout = averagefuel * (5.0 / 1023.0); // calculate vout
  float rtwo = 1000 / ((5 / vout) - 1); // calculate rtwo (R1 on board is 1000 ohms)

  if(rtwo < 75) percentage = 100; // takes ohm range of tank (1050 to 70) and divides it up by 12 gallons or 8%
  else if(rtwo < 151) percentage = 92; // starts checking to see if its full and works its way down
  else if(rtwo < 289) percentage = 81;
  else if(rtwo < 315) percentage = 76;
  else if(rtwo < 396) percentage = 68;
  else if(rtwo < 396) percentage = 68;
  else if(rtwo < 478) percentage = 60;
  else if(rtwo < 560) percentage = 52;
  else if(rtwo < 642) percentage = 44;
  else if(rtwo < 723) percentage = 36;
  else if(rtwo < 805) percentage = 28;
  else if(rtwo < 887) percentage = 20;
  else if(rtwo < 968) percentage = 12;
  else if(rtwo < 1050) percentage = 4;
  lcd.setCursor(0, 0); // Fuel status display on LCD
  lcd.print("FUEL LEVEL");
  lcd.print(" ");
  lcd.setCursor(12, 0);
  lcd.print(percentage);
  lcd.setCursor(15, 0);
  lcd.print("%");
  lcd.setCursor(0, 1);
  lbg.drawValue(percentage, 100);
  float oilvolt = analogRead(oilvin); // Read oil and water gauges analog input
  float watervolt = analogRead(watervin);
  if(percentage < 13)digitalWrite(fuelled, HIGH); // turns on fuel warning led if fuel under 13%
  if(watervolt < 460)
    digitalWrite(waterled, HIGH); // turns on water temp led if temp above 210 F as determined in calibration
  if(oilvolt > 410) digitalWrite(oilled, HIGH); // turns on oil pressure led if pressure below 12.5 psi
  delay(1000); // flashes led/s
  digitalWrite(fuelled, LOW);
  digitalWrite(waterled, LOW);
  digitalWrite(oilled, LOW);
  delay(1000);
}

