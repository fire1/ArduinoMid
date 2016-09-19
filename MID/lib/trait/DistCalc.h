//
// Created by Angel Zaprianov on 14.9.2016 г..
//

#ifndef ARDUINOMID_CALCULATORS_H
#define ARDUINOMID_CALCULATORS_H

const bool DistSensDebug = 1;
const int long correctionDistance = 1;
const int correctionDistanceTime = 1;

//Distance = Speed × Duration
//	d.      =   v.       ×         t
//	v=72km/hr = 72km/(3600sec) = 0.02km/sec
//t=2sec
//Then d = 0.02km/sec × 2sec = 0.04 km

//
// 1 h = 60 min = 60 x 60 sec = 3600 sec
// 72 km/h = 72000 m/3600 sec = 20 m/sec
// 20

double long travelDistance = 0;
int long travelAllPulse = 0;


/**
 * Gets travel distance in meters
 */
void getTravelDistanceMeters() {
    //
    // Check reading reach maximum
    travelAllPulse = travelAllPulse + CUR_VDT;

    if (isSensorReadLow()) {
        //
        // Travel distance in meters for second =
        // (travelAllPulse * VssCorrection) = All travel km  * 1000 = all distance in meters / (travel time / 1000 = 1 Second )
        double calculatedDistance = ((travelAllPulse * VssCorrection) * 1000);

        if (calculatedDistance > 0) {
            travelDistance = travelDistance + calculatedDistance / CUR_VTT;
        }
    }

    //
    // debug info
    if (DistSensDebug) {
        Serial.print("\n");
        Serial.print(" Dist All:  \t");
        Serial.print((int) travelDistance);
        Serial.print(" Dist time:  \t");
        Serial.print(CUR_VTT);
        Serial.print("\t Dist KM:  \t");
        Serial.print((int) travelDistance / (float) 10);
        Serial.print("\n");
    }
}

/**
 * Get Distance Kilometers
 */
float getTravelDistance() {

    if (travelDistance < 0) {
        return (float) 0;
    }

    return travelDistance / (float) 3600000;
}

/**
 * Get Distance Meters
 */
int getTravelDistanceMt() {
    if (travelDistance < 0) {
        return 0;
    }
    return int(travelDistance);
}

/**
 * Gets total travel time
 */
float getTravelTime() {
    return CUR_VTT / (float) 360000;
}

#endif //ARDUINOMID_CALCULATORS_H
