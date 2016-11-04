//
// Created by Angel Zaprianov on 4.11.2016 г..
//

// Turns Arduino onboard led (pin 13) on or off using serial command input.

// Pin 13, a LED connected on most Arduino boards.
int const LED = 13;

// Serial Input Variables
int intLoopCounter = 0;
String strSerialInput = "";

// the setup routine runs once when you press reset:
void setup()
{
    // initialize the digital pin as an output.
    pinMode(LED, OUTPUT);

    // initialize serial port
    Serial.begin(250000); // CHANGE BAUD RATE based on the board speed.

    // initialized
    Serial.println("Initialized.");
}

// the loop routine runs over and over again forever:
void loop()
{
    // Slow down a bit.
    // Note: This may have to be increased for longer strings or increase the iteration in GetPossibleSerialData() function.
    delay(1);
    CheckAndExecuteSerialCommand();
}

void CheckAndExecuteSerialCommand()
{
    //Get Data from Serial
    String serialData = GetPossibleSerialData();
    bool commandAccepted = false;

    if (serialData.startsWith("LED.ON"))
    {
        commandAccepted = true;
        digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    }
    else if  (serialData.startsWith("LED.OFF"))
    {
        commandAccepted = true;
        digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
    }
    else if (serialData != "")
    {
        Serial.println();
        Serial.println("*** Command Failed ***");
        Serial.println("\t" + serialData);
        Serial.println();
        Serial.println();
        Serial.println("*** Invalid Command ***");
        Serial.println();
        Serial.println("Try:");
        Serial.println("\tLED.ON");
        Serial.println("\tLED.OFF");
        Serial.println();
    }

    if (commandAccepted)
    {
        Serial.println();
        Serial.println("*** Command Executed ***");
        Serial.println("\t" + serialData);
        Serial.println();
    }
}

String GetPossibleSerialData()
{
    String retVal;
    int iteration = 10; // 10 times the time it takes to do the main loop
    if (strSerialInput.length() > 0)
    {
        // Print the retreived string after looping 10(iteration) ex times
        if (intLoopCounter > strSerialInput.length() + iteration)
        {
            retVal = strSerialInput;
            strSerialInput = "";
            intLoopCounter = 0;
        }
        intLoopCounter++;
    }

    return retVal;
}

void serialEvent()
{
    while (Serial.available())
    {
        strSerialInput.concat((char) Serial.read());
    }
}