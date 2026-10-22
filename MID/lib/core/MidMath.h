//
// AVR-free math helpers shared by firmware and host unit tests.
//

#ifndef ARDUINO_MID_CORE_MATH_H
#define ARDUINO_MID_CORE_MATH_H

#include <stdint.h>
#include <stdio.h>

#ifndef sq
#define sq(x) ((x) * (x))
#endif

#ifndef CAR_WEIGHT
#define CAR_WEIGHT 1155
#endif

/**
 * Format float as "DD.d" into output (clamps below -99).
 * Return type matches historical firmware signature.
 */
inline char displayFloat(float value, char *output) {
    if (value < -99) {
        value = -99;
    }

    int dig1 = int(value) * 10;
    int dig2 = int((value * 10) - dig1);

    dig1 = dig1 / 10;
    if (dig2 < 0) {
        dig2 = dig2 * -1;
    }

    sprintf(output, "%02d.%1d", dig1, dig2);
    return 0;
}

/**
 * Split float into integer and two-digit fractional parts.
 */
inline void separateFloat(float value, int arrOutput[2]) {
    uint8_t dig1 = uint8_t(value) * 100;
    uint8_t dig2 = uint8_t((value * 100) - dig1);

    dig1 = dig1 / 100;
    if (dig2 < 0) {
        dig2 = dig2 * -1;
    }

    arrOutput[0] = dig1;
    arrOutput[1] = dig2;
}

/**
 * Create float from two integers (a + b*0.01).
 */
inline float restoreFloat(uint8_t a, uint8_t b) {
    float bf = b * 0.01;
    return a + bf;
}

/**
 * Truncate float to two decimal places.
 */
inline float floatPrecision(float val) {
    return ((int) (val * 100.0)) / 100.0F;
}

/**
 * Elapsed ticks with unsigned wraparound (same formula for us/ms).
 */
inline unsigned long elapsedMicroseconds(unsigned long startMicroSeconds,
                                         unsigned long currentMicroseconds) {
    if (currentMicroseconds >= startMicroSeconds)
        return currentMicroseconds - startMicroSeconds;
    return 4294967295UL - (startMicroSeconds - currentMicroseconds);
}

inline unsigned long elapsedMilliseconds(unsigned long startMilliseconds,
                                         unsigned long currentMilliseconds) {
    if (currentMilliseconds >= startMilliseconds)
        return currentMilliseconds - startMilliseconds;
    return 4294967295UL - (startMilliseconds - currentMilliseconds);
}

/**
 * Approximate kinetic power: (1/2) * mass * v^2 / time.
 */
inline uint32_t calculateJoules(uint32_t travelMeters, uint16_t travelTime) {
    return (0.5) * CAR_WEIGHT * sq(travelMeters) / travelTime;
}

/**
 * Convert joules/sec-style value to horsepower (firmware constant).
 */
inline uint16_t getHorsepower(uint32_t joules) {
    return uint16_t(joules * 0.00013404825737265);
}

#endif // ARDUINO_MID_CORE_MATH_H
