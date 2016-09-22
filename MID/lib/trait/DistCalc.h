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

double long travelingTimeForDistance = 0;

float CUR_VTD = 0; // Travel distance
float CUR_TDT = 0; //travel time
float CUR_VTT = 0;

int vehicleStopped = LOW;

unsigned long timeTravelDistance = 0;
int long lastRecordTravelTimeDistance = 0;

/**
 * Function to resolve time for distance
 *      Only for calculation the distance
 */
int getTravelDistanceTime(int long currentTimeDistance) {


    //
    // Pass records from mil sec
    if (currentTimeDistance < 1) {
        currentTimeDistance = currentTimeDistance * -1;
    }

    //
    // Starts timing
    if (lastRecordTravelTimeDistance == 0 || vehicleStopped == HIGH) {
        lastRecordTravelTimeDistance = currentTimeDistance;
    }


    timeTravelDistance = timeTravelDistance + (currentTimeDistance - lastRecordTravelTimeDistance);


    lastRecordTravelTimeDistance = currentTimeDistance;
    vehicleStopped = LOW;

    return timeTravelDistance / 1000;
}


unsigned long timeTravelTrip = 0;
int long lastRecordTravelTimeTrip = 0;

/**
 * Trip travel time
 */
int getTravelTime(int long currentTimeTrip) {


    //
    // Pass records from mil sec
    if (currentTimeTrip < 1) {
        currentTimeTrip = currentTimeTrip * -1;
    }


    timeTravelTrip = timeTravelTrip + (currentTimeTrip - lastRecordTravelTimeTrip);


    lastRecordTravelTimeTrip = currentTimeTrip;
    vehicleStopped = LOW;

    return timeTravelTrip / 1000;
}

int long distanceCount = 0;

/**
 * Gets travel distance in meters
 */
void getTravelDistanceMeters() {

    float travelDistanceInMeters = 0;
    //
    // Check reading reach maximum
    if (CUR_VSS > 3) {
        travelAllPulse = travelAllPulse + CUR_VSS;
        travelDistanceInMeters = (travelAllPulse * VssCorrection) * 1000;

        //
        // Pass travel seconds distance
        CUR_TDT = getTravelDistanceTime(millis());

        //
        // Vehicle Time Travel detection
        travelingTimeForDistance = CUR_TDT;


    } else {
        vehicleStopped = HIGH;
    }

    CUR_VTT = getTravelTime(millis());

    if (isSensorReadMid()) {
        //
        // Check is collected enough data for calculation
        if (travelDistanceInMeters > 1) {

            //
            // Travel distance in meters for second =
            // (travelAllPulse * VssCorrection) = All travel km  * 1000 =
            //      all distance in meters / (travel time / 1000 = 1 Second )
            travelDistance = travelDistance + (travelDistanceInMeters / (CUR_TDT));
            //
            // Km with last meters
            // 479 = 2,918km

            int long buff = int(travelDistance / 1000000);
            CUR_VTD = float(buff / 164.60);
            distanceCount++;
        }
    }

    //
    // debug info
    if (DistSensDebug) {
        Serial.print("\n");
//        Serial.print(" Dist Pulse:  \t");
//        Serial.print((int) travelAllPulse);
//        Serial.print("\t Dist calc:  \t");
        Serial.print(int(travelDistance / 1000000));
//        Serial.print(" Dist time:  \t");
//        Serial.print(CUR_TDT);
//        Serial.print(" Trip time:  \t");
//        Serial.print(CUR_VTT);

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
