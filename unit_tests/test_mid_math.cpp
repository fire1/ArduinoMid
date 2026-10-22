#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <cstring>

#include "core/MidMath.h"

using Catch::Matchers::WithinAbs;

TEST_CASE("displayFloat formats one decimal place", "[math]") {
    char buf[16];

    displayFloat(12.3f, buf);
    REQUIRE(std::strcmp(buf, "12.3") == 0);

    displayFloat(0.5f, buf);
    REQUIRE(std::strcmp(buf, "00.5") == 0);

    displayFloat(-5.2f, buf);
    REQUIRE(std::strcmp(buf, "-5.2") == 0);

    // Clamp + float truncation: -99.1 formats as -99.0
    displayFloat(-99.1f, buf);
    REQUIRE(std::strcmp(buf, "-99.0") == 0);

    displayFloat(-150.0f, buf);
    REQUIRE(std::strcmp(buf, "-99.0") == 0);
}

TEST_CASE("separateFloat uses uint8_t math (characterization)", "[math]") {
    int parts[2];
    // uint8_t(value)*100 overflows for larger integer parts
    separateFloat(12.34f, parts);
    REQUIRE(parts[0] == 1);
    REQUIRE(parts[1] == 34);

    float restored = restoreFloat(12, 34);
    REQUIRE_THAT(restored, WithinAbs(12.34f, 0.001f));
}

TEST_CASE("floatPrecision truncates to two decimals", "[math]") {
    REQUIRE_THAT(floatPrecision(1.239f), WithinAbs(1.23f, 0.001f));
    REQUIRE_THAT(floatPrecision(9.999f), WithinAbs(9.99f, 0.001f));
}

TEST_CASE("elapsedMicroseconds handles wraparound", "[math]") {
    REQUIRE(elapsedMicroseconds(100UL, 250UL) == 150UL);
    REQUIRE(elapsedMicroseconds(0xFFFFFFF0UL, 0x00000010UL) ==
            (4294967295UL - (0xFFFFFFF0UL - 0x00000010UL)));
}

TEST_CASE("elapsedMilliseconds handles wraparound", "[math]") {
    REQUIRE(elapsedMilliseconds(50UL, 80UL) == 30UL);
    REQUIRE(elapsedMilliseconds(0xFFFFFFF0UL, 5UL) ==
            (4294967295UL - (0xFFFFFFF0UL - 5UL)));
}

TEST_CASE("calculateJoules and getHorsepower", "[math]") {
    // (1/2)*1155*30^2 / 5 = 103950
    uint32_t joules = calculateJoules(30, 5);
    REQUIRE(joules == 103950);

    uint16_t hp = getHorsepower(joules);
    REQUIRE(hp == static_cast<uint16_t>(joules * 0.00013404825737265));
}
