//
// Created by Angel Zaprianov on 27.3.2018 г..
//


#ifndef ARDUINO_MID_LOG_CARD
#define ARDUINO_MID_LOG_CARD

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>

#ifndef LOG_CHIP_SELECT
#define LOG_CHIP_SELECT 22
#endif

#ifndef __SD_H__

#include "../../libraries/SD/src/SD.h"
#include "../../libraries/SD/src/utility/SdFat.h"

#endif


#define record(msg, data){Logger.set(F(msg),data);}

class LogCard {
    File logFile;
    boolean isCardReady = false;

private:
    /**
     * Displays title of logger
     * @param msg
     */
    void ttl(const __FlashStringHelper *msg) {
        logFile.print(msg);
        logFile.print(F(": "));
    }

public:
    LogCard(void) {}

    void begin() {

        //
        // Check sd card
        if (!SD.begin(LOG_CHIP_SELECT)) {
            dump_txt("[ERROR] Card failed, or not present!");
            isCardReady = false;
            return;
        }



        //
        // Open sd card
        logFile = SD.open(F("mid.log"), FILE_WRITE);
        if (!logFile) {
            dump_txt("[ERROR] SD Logger card is NOT WRITABLE! ")
            isCardReady = false;
            return;
        }

        isCardReady = true;
        dump_txt("SD Logger card is initialized and ready!")
        logFile.println(F("Starting new trip .... "));
    }

/**
 *
 * @param msg
 * @param data
 */
    void set(const __FlashStringHelper *msg, uint8_t data) {
        if (isCardReady) {
            ttl(msg);
            logFile.println(data);
        }
    }

    void set(const __FlashStringHelper *msg, float data) {
        if (isCardReady) {
            ttl(msg);
            logFile.println(data);
        }
    }

    void set(const __FlashStringHelper *msg, double data) {
        if (isCardReady) {
            ttl(msg);
            logFile.println(data);
        }
    }

    void set(const __FlashStringHelper *msg, int data) {
        if (isCardReady) {
            ttl(msg);
            logFile.println(data);
        }
    }

    void set(const __FlashStringHelper *msg, long data) {
        if (isCardReady) {
            ttl(msg);
            logFile.println(data);
        }
    }

    void set(const __FlashStringHelper *msg, unsigned long data) {
        if (isCardReady) {
            ttl(msg);
            logFile.println(data);
        }
    }


    void shutdown() {
        if(isCardReady) {
            logFile.print(F("Ending the trip after: "));
            logFile.println(millis());
            logFile.close();
        }
    }

};

LogCard Logger;


#endif
