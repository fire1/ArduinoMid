#include <catch2/catch_test_macros.hpp>

#include "core/MidLpgBits.h"

TEST_CASE("midLpgPulseToBit classifies pulse widths", "[lpg]") {
    REQUIRE(midLpgPulseToBit(2000) == MID_LPG_PULSE_BIT0);
    REQUIRE(midLpgPulseToBit(1993) == MID_LPG_PULSE_BIT0);
    // Lower bound is exclusive: > (B0 - TS) => > 1900
    REQUIRE(midLpgPulseToBit(1900) == MID_LPG_PULSE_NONE);
    REQUIRE(midLpgPulseToBit(1901) == MID_LPG_PULSE_BIT0);
    REQUIRE(midLpgPulseToBit(2100) == MID_LPG_PULSE_BIT0);

    REQUIRE(midLpgPulseToBit(4000) == MID_LPG_PULSE_BIT1);
    REQUIRE(midLpgPulseToBit(3982) == MID_LPG_PULSE_BIT1);
    // Lower bound exclusive: > (B1 - TS) => > 3900
    REQUIRE(midLpgPulseToBit(3900) == MID_LPG_PULSE_NONE);
    REQUIRE(midLpgPulseToBit(3901) == MID_LPG_PULSE_BIT1);
    REQUIRE(midLpgPulseToBit(4100) == MID_LPG_PULSE_BIT1);

    REQUIRE(midLpgPulseToBit(5000) == MID_LPG_PULSE_TIMEOUT);
    REQUIRE(midLpgPulseToBit(8000) == MID_LPG_PULSE_TIMEOUT);

    REQUIRE(midLpgPulseToBit(1000) == MID_LPG_PULSE_NONE);
    REQUIRE(midLpgPulseToBit(3000) == MID_LPG_PULSE_NONE);
}

TEST_CASE("midLpgApplyPulse builds bit buffer", "[lpg]") {
    uint16_t buffer = 0;
    uint8_t offset = 0;

    offset = midLpgApplyPulse(&buffer, offset, 4000); // bit0 = 1
    offset = midLpgApplyPulse(&buffer, offset, 2000); // bit1 = 0
    offset = midLpgApplyPulse(&buffer, offset, 2000); // bit2 = 0
    offset = midLpgApplyPulse(&buffer, offset, 4000); // bit3 = 1

    REQUIRE(offset == 4);
    REQUIRE(bitRead(buffer, 0) == 1);
    REQUIRE(bitRead(buffer, 1) == 0);
    REQUIRE(bitRead(buffer, 2) == 0);
    REQUIRE(bitRead(buffer, 3) == 1);

    offset = midLpgApplyPulse(&buffer, offset, 9000); // timeout
    REQUIRE(offset == 0);
    REQUIRE(buffer == 0);
}

TEST_CASE("switch detection matches LpgSerial bit 9/10 rules", "[lpg]") {
    // Matches LpgSerial::isSwitchBnz / isSwitchLPG
    uint16_t bnz = 0;
    bnz |= (1u << 9);
    bnz |= (1u << 10);
    REQUIRE(midLpgIsSwitchBnz(bnz));
    REQUIRE_FALSE(midLpgIsSwitchLpg(bnz));

    uint16_t lpg = 0;
    lpg |= (1u << 9);
    // bit 10 clear
    REQUIRE(midLpgIsSwitchLpg(lpg));
    REQUIRE_FALSE(midLpgIsSwitchBnz(lpg));

    // EuropeGas comment pattern interpreted MSB-first (left = bit13):
    // "10011000100001" => bits 13,10,9,5,0 set -> BNZ
    uint16_t europeBnz =
            (1u << 13) | (1u << 10) | (1u << 9) | (1u << 5) | (1u << 0);
    REQUIRE(midLpgIsSwitchBnz(europeBnz));

    // "10001000100001" => bits 13,9,5,0 -> LPG
    uint16_t europeLpg =
            (1u << 13) | (1u << 9) | (1u << 5) | (1u << 0);
    REQUIRE(midLpgIsSwitchLpg(europeLpg));
}

TEST_CASE("midLpgPatternFromLsbString is LSB-first", "[lpg]") {
    // "1001" => bit0=1, bit1=0, bit2=0, bit3=1
    uint16_t v = midLpgPatternFromLsbString("1001");
    REQUIRE(bitRead(v, 0) == 1);
    REQUIRE(bitRead(v, 1) == 0);
    REQUIRE(bitRead(v, 2) == 0);
    REQUIRE(bitRead(v, 3) == 1);
}

TEST_CASE("midLpgDataLength counts significant bits", "[lpg]") {
    REQUIRE(midLpgDataLength(0) == 0);
    REQUIRE(midLpgDataLength(0x0001) == 1);
    REQUIRE(midLpgDataLength(0x0008) == 4);
    REQUIRE(midLpgDataLength(0x2000) == 14); // bit 13 set
}
