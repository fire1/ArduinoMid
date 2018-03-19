#include <Arduino.h>

#include <SD.h>
#include <SPI.h>


#ifndef __SD_H__

#include "../../libraries/SD/src/SD.h"
#include "../../libraries/SD/src/utility/SdFat.h"

#endif


const int chipSelect = 22;
const char *logFilename = "mid.log";


File logFile;

void setup() {


    Serial.begin(115200);
    // see if the card is present and can be initialized:
    if (!SD.begin(chipSelect)) {
        Serial.println("Card failed, or not present");
    }
    Serial.println("card initialized.");


    logFile = SD.open(logFilename);
    if (logFile) {
        Serial.println("mid.log: ");
        // read from the file until there's nothing else in it:
        while (logFile.available()) {
            Serial.write(logFile.read());
        }
        // close the file:
        logFile.close();
    } else {
        // if the file didn't open, print an error:
        Serial.println("error opening mid.log");
    }


    logFile = SD.open(logFilename, FILE_WRITE);

    if (logFile) {
        logFile.println("test");
        logFile.close();
    }

}

void loop() {


}