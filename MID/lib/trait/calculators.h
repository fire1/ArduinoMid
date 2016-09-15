//
// Created by Angel Zaprianov on 14.9.2016 г..
//

#ifndef ARDUINOMID_CALCULATORS_H
#define ARDUINOMID_CALCULATORS_H

//Distance = Speed × Duration
//	d.      =   v.       ×         t
//	v=72km/hr = 72km/(3600sec) = 0.02km/sec
//t=2sec
//Then d = 0.02km/sec × 2sec = 0.04 km

//
// 1 h = 60 min = 60 x 60 sec = 3600 sec
// 72 km/h = 72000 m/3600 sec = 20 m/sec
// 20


float travelDistance = 0;
/**
 * Gets travel distance in meters
 */
int getTravelDistanceMeters () {

  //
  // Check reading reach maximum
  if (isSensorReadLow ())
	{
	  travelDistance = travelDistance + ((getVssSens () * SNS_INTERVAL_TIME_LOW) / MILLIS_PER_HR);
	}
  
}
/**
 * Get Distance Kilometers
 */
int getTravelDistanceKm () {
  return int (travelDistance * millis ());
}

/**
 * Get Distance Meters
 */
int getTravelDistanceMt () {
  return int (travelDistance / 1000);
}

#endif //ARDUINOMID_CALCULATORS_H
