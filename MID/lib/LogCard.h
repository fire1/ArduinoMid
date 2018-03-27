//
// Created by Angel Zaprianov on 27.3.2018 г..
//


#ifndef ARDUINO_MID_LOG_CARD
#define ARDUINO_MID_LOG_CARD

#include <Arduino.h>
#include <SD.h>
#include <SPI.h>


#ifndef __SD_H__

#include "../../libraries/SD/src/SD.h"
#include "../../libraries/SD/src/utility/SdFat.h"

#endif






#define record(msg, data){Log.set(F(msg),data);}

class LogCard {
    File logFile;
    boolean logActive = false;


public:
    LogCard(void) {}

    void begin() {
        logFile = SD.open(F("mid.log"), FILE_WRITE);
        if (logFile) {
            dump_txt("SD Logger card is inactive!")
        }
    }

/**
 *
 * @param msg
 * @param data
 */
    void set(const __FlashStringHelper *msg, uint8_t data) {
        if (logActive) {
            logFile.print(msg);
            logFile.println(data);
        }
    }

    void set(const __FlashStringHelper *msg, float data) {
        if (logActive) {
            logFile.print(msg);
            logFile.println(data);
        }
    }

    void set(const __FlashStringHelper *msg, double data) {
        if (logActive) {
            logFile.print(msg);
            logFile.println(data);
        }
    }

    void set(const __FlashStringHelper *msg, int data) {
        if (logActive) {
            logFile.print(msg);
            logFile.println(data);
        }
    }

    void set(const __FlashStringHelper *msg, long data) {
        if (logActive) {
            logFile.print(msg);
            logFile.println(data);
        }
    }

    void set(const __FlashStringHelper *msg, unsigned long data) {
        if (logActive) {
            logFile.print(msg);
            logFile.println(data);
        }
    }


    void shutdown() {
        logFile.close();
    }

};

LogCard Logger;


#endif
