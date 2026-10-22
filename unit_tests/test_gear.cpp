#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "core/MidGear.h"

using Catch::Matchers::WithinAbs;

TEST_CASE("midResolveGear returns 0 at standstill", "[gear]") {
    REQUIRE(midResolveGear(2000, 0) == 0);
}

TEST_CASE("midGearRatio matches firmware formula", "[gear]") {
    // Ratio = (rpm * Pi * Dia * 60) / (speed * FD * 1e6)
    float expected = (3000.0f * CAR_GEAR_Pi * CAR_GEAR_Dia * 60.0f) /
                     (50.0f * CAR_GEAR_FD * 1000000.0f);
    REQUIRE_THAT(midGearRatio(3000, 50), WithinAbs(expected, 0.0001f));
}

TEST_CASE("midResolveGear matches gear bands (characterization)", "[gear]") {
    // Pick rpm/speed so Ratio ~= G1 (3.73)
    // Ratio = rpm * k / speed, k = Pi*Dia*60/(FD*1e6)
    const float k = (CAR_GEAR_Pi * CAR_GEAR_Dia * 60.0f) / (CAR_GEAR_FD * 1000000.0f);

    auto rpmFor = [&](float gear, int speed) {
        return static_cast<int>(gear * speed / k + 0.5f);
    };

    SECTION("1st gear") {
        int rpm = rpmFor(CAR_GEAR_G1, 20);
        REQUIRE(midResolveGear(rpm, 20) == 1);
    }

    SECTION("3rd gear") {
        int rpm = rpmFor(CAR_GEAR_G3, 50);
        REQUIRE(midResolveGear(rpm, 50) == 3);
    }

    SECTION("4th gear") {
        int rpm = rpmFor(CAR_GEAR_G4, 70);
        REQUIRE(midResolveGear(rpm, 70) == 4);
    }

    SECTION("5th gear") {
        int rpm = rpmFor(CAR_GEAR_G5, 90);
        REQUIRE(midResolveGear(rpm, 90) == 5);
    }

    SECTION("uncapped 6th is clamped by MAX_GEAR_NBR") {
        int rpm = rpmFor(CAR_GEAR_G6, 110);
        // Would be 6, but MAX_GEAR_NBR defaults to 5
        REQUIRE(midResolveGear(rpm, 110) == MAX_GEAR_NBR);
    }

    SECTION("no match yields capped default 7 -> MAX_GEAR_NBR") {
        REQUIRE(midResolveGear(100, 100) == MAX_GEAR_NBR);
    }
}

TEST_CASE("G2 left-side Ratio/6 quirk rarely matches", "[gear]") {
    // Document historical behavior: left condition uses Ratio/6 vs G2,
    // so G2 almost never sets via the intended band alone.
    const float k = (CAR_GEAR_Pi * CAR_GEAR_Dia * 60.0f) / (CAR_GEAR_FD * 1000000.0f);
    int rpm = static_cast<int>(CAR_GEAR_G2 * 40 / k + 0.5f);
    float ratio = midGearRatio(rpm, 40);
    REQUIRE_THAT(ratio, WithinAbs(CAR_GEAR_G2, 0.05f));
    // With the quirk, Ratio/6 ≈ 0.356, not near G2=2.14, so gear stays default (capped)
    REQUIRE(midResolveGear(rpm, 40) == MAX_GEAR_NBR);
}
