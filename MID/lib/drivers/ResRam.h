//
// Created by Angel Zaprianov on 6.4.2017 г..
//

#ifndef ARDUINOMID_RESRAM_H
#define ARDUINOMID_RESRAM_H

#ifndef RESERVE_RAM_SIZE
#define RESERVE_RAM_SIZE   2706 // 2048
#endif

#define DEBUG_RESERVE_RAM

//
// Allocating more RAM from Arduino compile
//byte *____Arduino____RAM_reservoir = new byte ("Reserve space. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. I support the Cape Wind project. ");
//byte *____Arduino____RAM_reservoir;
//byte *____Arduino____RAM_reservoir = (byte *) malloc(RESERVE_RAM_SIZE * sizeof(byte));

/**
 *
 */
class ResRam {

    int lastRead = 0, currentSize;

public:

    ResRam() {
        currentSize = RESERVE_RAM_SIZE;
//        ____Arduino____RAM_reservoir[] = new char[RESERVE_RAM_SIZE];
    }

    /**
     *
     */
    void begin() {
        lastRead = getFreeRam();
//        free(____Arduino____RAM_reservoir);
//        delete ____Arduino____RAM_reservoir;
    }

    /**
     * Listen RAM usage
     */
    void listen() {
        int ram = getFreeRam();
        if (lastRead > ram) {
            currentSize = currentSize - (lastRead - ram);


//            ____Arduino____RAM_reservoir = new char[currentSize];
        }

        if (lastRead < ram) {
            currentSize = currentSize + (ram - lastRead);
//            ____Arduino____RAM_reservoir = new char[currentSize];
        }

#ifdef DEBUG_RESERVE_RAM
//        Serial.print(F(" Free heap RAM: "));
//        Serial.print(ram);
//        Serial.print(F("Reserve: "));
//        Serial.print(currentSize);
//        Serial.println();
#endif
        lastRead = ram;
    }

    /**
 * What getFreeRam() is actually reporting is the space between the heap and the stack.
 * it does not report any de-allocated memory that is buried in the heap.
 * Buried heap space is not usable by the stack,
 * and may be fragmented enough that it is not usable for many heap allocations either.
 * The space between the heap and the stack is what you really need to monitor
 * if you are trying to avoid stack crashes.
 */
    int getFreeRam() {
        extern int __heap_start, *__brkval;
        int v;
        return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
    }
};


#endif //ARDUINOMID_RESRAM_H
