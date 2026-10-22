#include <catch2/catch_test_macros.hpp>

#include "Arduino.h"
#include "AmpTime.h"

TEST_CASE("AmpTime flags fire on cadence", "[amptime]") {
    ArduinoStubClock::reset();
    AmpTime amp;

    amp.listener();
    REQUIRE_FALSE(amp.isSec());
    REQUIRE_FALSE(amp.isSecond());
    REQUIRE_FALSE(amp.isRfr());

    // First edge at AMP_SEC (100ms)
    ArduinoStubClock::setMillis(100);
    amp.listener();
    REQUIRE(amp.isSec());
    REQUIRE_FALSE(amp.isSecond());

    // Next call before next period clears the one-shot flag
    ArduinoStubClock::setMillis(150);
    amp.listener();
    REQUIRE_FALSE(amp.isSec());

    // Real-time second
    ArduinoStubClock::setMillis(1000);
    amp.listener();
    REQUIRE(amp.isSecond());

    // Screen refresh AMP_RFR = 601
    ArduinoStubClock::reset();
    AmpTime amp2;
    ArduinoStubClock::setMillis(601);
    amp2.listener();
    REQUIRE(amp2.isRfr());

    ArduinoStubClock::setMillis(700);
    amp2.listener();
    REQUIRE_FALSE(amp2.isRfr());
}

TEST_CASE("AmpTime isMin and isSens", "[amptime]") {
    ArduinoStubClock::reset();
    AmpTime amp;

    ArduinoStubClock::setMillis(AMP_MIN);
    amp.listener();
    REQUIRE(amp.isMin());

    ArduinoStubClock::setMillis(MILLIS_SENS);
    amp.listener();
    REQUIRE(amp.isSens());
}
