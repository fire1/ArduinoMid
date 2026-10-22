//
// AVR-free LPG pulse / bit helpers shared by firmware and host unit tests.
//

#ifndef ARDUINO_MID_CORE_LPG_BITS_H
#define ARDUINO_MID_CORE_LPG_BITS_H

#include <stdint.h>

#ifndef LPG_BITS
#define LPG_BITS 14
#endif
#ifndef LPG_BYTE_TS
#define LPG_BYTE_TS 100
#endif
#ifndef LPG_BYTE_B0
#define LPG_BYTE_B0 2000
#endif
#ifndef LPG_BYTE_B1
#define LPG_BYTE_B1 4000
#endif
#ifndef LPG_TIMEOUT
#define LPG_TIMEOUT 5000
#endif

/** Decode result for a single pulse width. */
enum MidLpgPulseKind {
    MID_LPG_PULSE_NONE = -1,
    MID_LPG_PULSE_BIT0 = 0,
    MID_LPG_PULSE_BIT1 = 1,
    MID_LPG_PULSE_TIMEOUT = 2
};

/**
 * Classify pulse width (microseconds) into bit 0, bit 1, timeout, or none.
 */
inline int midLpgPulseToBit(unsigned long pulseUs) {
    if (pulseUs >= LPG_TIMEOUT) {
        return MID_LPG_PULSE_TIMEOUT;
    }
    if (pulseUs > (LPG_BYTE_B1 - LPG_BYTE_TS) && pulseUs <= (LPG_BYTE_B1 + LPG_BYTE_TS)) {
        return MID_LPG_PULSE_BIT1;
    }
    if (pulseUs > (LPG_BYTE_B0 - LPG_BYTE_TS) && pulseUs <= (LPG_BYTE_B0 + LPG_BYTE_TS)) {
        return MID_LPG_PULSE_BIT0;
    }
    return MID_LPG_PULSE_NONE;
}

/**
 * Apply one decoded bit into a data buffer at offset (matches echoLpgISR).
 * Returns new offset, or 0 on timeout / overflow reset.
 * Uses pointer so volatile buffers (ISR) bind cleanly.
 */
inline uint8_t midLpgApplyPulse(uint16_t *dataBuffer, uint8_t offset, unsigned long pulseUs) {
    int kind = midLpgPulseToBit(pulseUs);

    if (kind == MID_LPG_PULSE_TIMEOUT) {
        *dataBuffer = 0;
        return 0;
    }

    if (offset >= LPG_BITS) {
        *dataBuffer = 0;
        offset = 0;
    }

    if (kind == MID_LPG_PULSE_BIT1) {
        *dataBuffer |= (uint16_t) (1 << offset);
        return (uint8_t) (offset + 1);
    }
    if (kind == MID_LPG_PULSE_BIT0) {
        *dataBuffer |= (uint16_t) (0 << offset);
        return (uint8_t) (offset + 1);
    }
    return offset;
}

#ifndef bitRead
#define bitRead(value, bit) (((value) >> (bit)) & 0x01)
#endif

/**
 * BNZ switch: bits 9 and 10 both set (matches LpgSerial::isSwitchBnz).
 */
inline bool midLpgIsSwitchBnz(uint16_t data) {
    return bitRead(data, 9) == 1 && bitRead(data, 10) == 1;
}

/**
 * LPG switch: bit 9 set, bit 10 clear (matches LpgSerial::isSwitchLPG).
 */
inline bool midLpgIsSwitchLpg(uint16_t data) {
    return bitRead(data, 9) == 1 && bitRead(data, 10) == 0;
}

/**
 * Count set-bit length (highest significant bit index + 1), capped at 14.
 * Matches LpgSerial::getDataLength loop behavior.
 */
inline uint8_t midLpgDataLength(uint16_t data) {
    uint8_t length = 0;
    for (; data; data >>= 1) {
        length++;
        if (length > 14) break;
    }
    return length;
}

/**
 * Build a 14-bit pattern from a binary string of '0'/'1' (LSB first, like serial dumps).
 * Example: "10011000100001" — index 0 is bit 0.
 */
inline uint16_t midLpgPatternFromLsbString(const char *bits) {
    uint16_t value = 0;
    for (uint8_t i = 0; bits[i] != '\0' && i < 16; i++) {
        if (bits[i] == '1') {
            value |= (uint16_t) (1 << i);
        }
    }
    return value;
}

#endif // ARDUINO_MID_CORE_LPG_BITS_H
