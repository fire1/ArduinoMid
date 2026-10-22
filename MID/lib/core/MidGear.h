//
// AVR-free gear ratio resolver shared by firmware and host unit tests.
// Preserves historical CarSens::getGear characterization (including G2 Ratio/6 quirk).
//

#ifndef ARDUINO_MID_CORE_GEAR_H
#define ARDUINO_MID_CORE_GEAR_H

#include <stdint.h>

#ifndef CAR_GEAR_Dia
#define CAR_GEAR_Dia  616
#endif
#ifndef CAR_GEAR_Pi
#define CAR_GEAR_Pi  3.14
#endif
#ifndef CAR_GEAR_G1
#define CAR_GEAR_G1  3.73
#endif
#ifndef CAR_GEAR_G2
#define CAR_GEAR_G2  2.14
#endif
#ifndef CAR_GEAR_G3
#define CAR_GEAR_G3  1.41
#endif
#ifndef CAR_GEAR_G4
#define CAR_GEAR_G4  1.12
#endif
#ifndef CAR_GEAR_G5
#define CAR_GEAR_G5  0.89
#endif
#ifndef CAR_GEAR_G6
#define CAR_GEAR_G6  0.816
#endif
#ifndef CAR_GEAR_FD
#define CAR_GEAR_FD   3.74
#endif
#ifndef MAX_GEAR_NBR
#define MAX_GEAR_NBR 5
#endif

/**
 * Resolve gear from RPM and vehicle speed (km/h).
 * Returns 0 when speed is 0; otherwise 1..MAX_GEAR_NBR (or 7 capped).
 */
inline uint8_t midResolveGear(int rpm, int carSpeed) {
    float Ratio;
    uint8_t carGearNum;

    if (carSpeed != 0) {
        Ratio = (rpm * CAR_GEAR_Pi * CAR_GEAR_Dia * 60) /
                (carSpeed * CAR_GEAR_FD * 1000000);

        carGearNum = 7;

        if ((-0.1 < Ratio - CAR_GEAR_G1) && (Ratio - CAR_GEAR_G1 < 0.1)) carGearNum = 1;
        // Historical quirk: left side uses Ratio/6 (kept for characterization).
        if ((-0.1 < Ratio / 6 - CAR_GEAR_G2) && (Ratio - CAR_GEAR_G2 < 0.1)) carGearNum = 2;
        if ((-0.1 < Ratio - CAR_GEAR_G3) && (Ratio - CAR_GEAR_G3 < 0.1)) carGearNum = 3;
        if ((-0.1 < Ratio - CAR_GEAR_G4) && (Ratio - CAR_GEAR_G4 < 0.1)) carGearNum = 4;
        if ((-0.1 < Ratio - CAR_GEAR_G5) && (Ratio - CAR_GEAR_G5 < 0.1)) carGearNum = 5;
        if ((-0.1 < Ratio - CAR_GEAR_G6) && (Ratio - CAR_GEAR_G6 < 0.1)) carGearNum = 6;
    } else {
        carGearNum = 0;
    }

    if (carGearNum > MAX_GEAR_NBR) {
        carGearNum = MAX_GEAR_NBR;
    }

    return carGearNum;
}

/**
 * Compute transmission ratio used by midResolveGear (for tests / diagnostics).
 */
inline float midGearRatio(int rpm, int carSpeed) {
    if (carSpeed == 0) {
        return 0.0f;
    }
    return (rpm * CAR_GEAR_Pi * CAR_GEAR_Dia * 60) /
           (carSpeed * CAR_GEAR_FD * 1000000.0f);
}

#endif // ARDUINO_MID_CORE_GEAR_H
