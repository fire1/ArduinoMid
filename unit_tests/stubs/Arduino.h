//
// Minimal Arduino stubs for host-side unit tests.
//

#ifndef ARDUINO_MID_HOST_ARDUINO_STUB_H
#define ARDUINO_MID_HOST_ARDUINO_STUB_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

typedef bool boolean;
typedef uint8_t byte;

#ifndef HIGH
#define HIGH 0x1
#endif
#ifndef LOW
#define LOW 0x0
#endif
#ifndef INPUT
#define INPUT 0x0
#endif
#ifndef OUTPUT
#define OUTPUT 0x1
#endif
#ifndef CHANGE
#define CHANGE 1
#endif

#ifndef F
#define F(x) (x)
#endif
#ifndef PROGMEM
#define PROGMEM
#endif
#ifndef pgm_read_word
#define pgm_read_word(addr) (*(const uint16_t *)(addr))
#endif
#ifndef pgm_read_byte
#define pgm_read_byte(addr) (*(const uint8_t *)(addr))
#endif
#ifndef strcpy_P
#define strcpy_P strcpy
#endif
#ifndef bitRead
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#endif
#ifndef bitSet
#define bitSet(value, bit) ((value) |= (1UL << (bit)))
#endif
#ifndef bitClear
#define bitClear(value, bit) ((value) &= ~(1UL << (bit)))
#endif
#ifndef sq
#define sq(x) ((x) * (x))
#endif

inline void cli() {}
inline void sei() {}

inline void pinMode(uint8_t, uint8_t) {}
inline void digitalWrite(uint8_t, uint8_t) {}
inline int digitalRead(uint8_t) { return LOW; }
inline int analogRead(uint8_t) { return 0; }
inline void analogWrite(uint8_t, int) {}
inline void delay(unsigned long) {}
inline void delayMicroseconds(unsigned int) {}

namespace ArduinoStubClock {
inline unsigned long &millisValue() {
    static unsigned long v = 0;
    return v;
}
inline unsigned long &microsValue() {
    static unsigned long v = 0;
    return v;
}
inline void setMillis(unsigned long v) { millisValue() = v; }
inline void advanceMillis(unsigned long d) { millisValue() += d; }
inline void setMicros(unsigned long v) { microsValue() = v; }
inline void advanceMicros(unsigned long d) { microsValue() += d; }
inline void reset() {
    millisValue() = 0;
    microsValue() = 0;
}
}

inline unsigned long millis() { return ArduinoStubClock::millisValue(); }
inline unsigned long micros() { return ArduinoStubClock::microsValue(); }

class __FlashStringHelper;

class HardwareSerial {
public:
    void begin(unsigned long) {}
    int available() { return 0; }
    int read() { return -1; }
    size_t print(const char *) { return 0; }
    size_t println(const char *) { return 0; }
    size_t println() { return 0; }
};

static HardwareSerial Serial;

#endif // ARDUINO_MID_HOST_ARDUINO_STUB_H
