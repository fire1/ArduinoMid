//
// Created by Angel Zaprianov on 3.10.2016 г..
//

#ifndef ARDUINOMID_INJCDATA_H
#define ARDUINOMID_INJCDATA_H

/**
 * Injects data from serial command
 */
class InjData {

private:
    int LINE_BUFFER_SIZE = 80; // max line length is one less than this

    int readSerialLine(char *buffer, int bufsize) {
        for (int index = 0; index < bufsize; index++) {
            // Wait until characters are available
            while (Serial.available() == 0) {
            }

            char ch = Serial.read(); // read next character
            Serial.print(ch); // echo it back: useful with the serial monitor (optional)

            if (ch == '\n') {
                buffer[index] = 0; // end of line reached: null terminate string
                return index; // success: return length of string (zero if string is empty)
            }

            buffer[index] = ch; // Append character to buffer
        }

        // Reached end of buffer, but have not seen the end-of-line yet.
        // Discard the rest of the line (safer than returning a partial line).

        char ch;
        do {
            // Wait until characters are available
            while (Serial.available() == 0) {
            }
            ch = Serial.read(); // read next character (and discard it)
            Serial.print(ch); // echo it back
        } while (ch != '\n');

        buffer[0] = 0; // set buffer to empty string even though it should not be used
        return -1; // error: return negative one to indicate the input was too long
    }

public:

    InjData(int bufferSize);
    void listenerSerial();

};


/*  ***     CPP part of file    *** */


InjData::InjData(int bufferSize) {
    LINE_BUFFER_SIZE = bufferSize;
}


void InjData::listenerSerial() {


    // Read command
    char line[LINE_BUFFER_SIZE];
    if (readSerialLine(line, sizeof(line)) < 0) {
        Serial.println("Error: line too long");
        return; // skip command processing and try again on next iteration of loop
    }else{
            Serial.print("> ");
    }

    // Process command

    if (strcmp(line, "off") == 0) {
//        digitalWrite(LED_PIN, LOW);
    } else if (strcmp(line, "on") == 0) {
//        digitalWrite(LED_PIN, HIGH);
    } else if (strcmp(line, "") == 0) {
        // Empty line: no command
    } else {
        Serial.print("Error: unknown command: \"");
        Serial.print(line);
        Serial.println("\" (available commands: \"off\", \"on\")");
    }
}


#endif //ARDUINOMID_INJCDATA_H
