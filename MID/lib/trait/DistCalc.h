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

int long timerRecordMoving, timerRecordStopped;


double long travelDistance = 0;
int long travelAllPulse = 0;

double long travelingTime = 0;

float CUR_VTD = 0; // Travel distance
int long CUR_VTT = 0; //travel time


int long timeTravel = 0, lastRecordTravelTime = 0;

int long getTimeTravel() {

    int long timeDifference = 0;

    //
    // if car is stopped
    if (timerRecordStopped > 0) {
        timeDifference = timerRecordMoving - timerRecordStopped;
    }
    //
    // Starts timing
    if (lastRecordTravelTime == 0) {
        lastRecordTravelTime = millis();
    }

    //
    // Pass records from mil sec
    timeTravel = (timeTravel + timeDifference) - lastRecordTravelTime;


    lastRecordTravelTime = millis();
    timerRecordMoving = 0;
    timerRecordStopped = 0;

    return timeTravel;
}

/**
 * Gets travel distance in meters
 */
void getTravelDistanceMeters() {

    int long travelDistanceInMeters = 0;
    //
    // Check reading reach maximum
    if (CUR_VSS > 3) {
        travelAllPulse = travelAllPulse + CUR_VSS;
        travelDistanceInMeters = int((travelAllPulse * VssCorrection) * 1000);
        //
        // Vehicle Time Travel detection
        travelingTime = travelingTime + 1;

        //
        // Vehicle is moving
        timerRecordMoving = millis();
    } else {
        //
        // Vehicle is not moving
        timerRecordStopped = millis();
    }


    if (isSensorReadMid()) {
        //
        // Check is collected enough data for calculation
        if (travelDistanceInMeters > 0 && (travelingTime / 10) > 1) {

            //
            // Pass travel seconds distance
            CUR_VTT = getTimeTravel();
            //
            // Travel distance in meters for second =
            // (travelAllPulse * VssCorrection) = All travel km  * 1000 = all distance in meters / (travel time / 1000 = 1 Second )
            travelDistance = travelDistance + travelDistanceInMeters / (travelingTime / 60);
            //
            // Km with last meters
            CUR_VTD = travelDistance / 100;
        }
    }

    //
    // debug info
    if (DistSensDebug) {
        Serial.print("\n");
        Serial.print(" Dist Pulse:  \t");
        Serial.print((int) travelAllPulse);
        Serial.print("\t Dist calc:  \t");
        Serial.print((int) travelDistanceInMeters);
        Serial.print(" Dist time:  \t");
        Serial.print(int(timeTravel));

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

    return CUR_VTD /* MILLIS_PER_HR */;
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
int long getTravelTime() {
    return CUR_VTT; // NOT hour
}

#endif //ARDUINOMID_CALCULATORS_H
