//
// Created by Angel Zaprianov on 14.9.2016 г..
//

#ifndef ARDUINOMID_CALCULATORS_H
#define ARDUINOMID_CALCULATORS_H

const bool DistSensDebug = 1;
//Distance = Speed × Duration
//	d.      =   v.       ×         t
//	v=72km/hr = 72km/(3600sec) = 0.02km/sec
//t=2sec
//Then d = 0.02km/sec × 2sec = 0.04 km

//
// 1 h = 60 min = 60 x 60 sec = 3600 sec
// 72 km/h = 72000 m/3600 sec = 20 m/sec
// 20

int long travelDistance = 0;

/**
 * Gets travel distance in meters
 */
void getTravelDistanceMeters() {
  //
  // Check reading reach maximum
  if(isSensorReadLow())
	{
	  // getVssSens() * 1000 = meters
	  travelDistance = travelDistance + ((getVssSens() * 1000 * (SNS_INTERVAL_TIME_LOW * CUR_VTT)) / MILLIS_PER_HR);
	}

  //
  // debug info
  if(DistSensDebug)
	{
	  Serial.print("\n");
	  Serial.print(" Dist All:  \t");
	  Serial.print(travelDistance);
	  Serial.print(" Dist KM:  \t");
	  Serial.print(travelDistance / 100000);
	  Serial.print("\n");
	}
}

/**
 * Get Distance Kilometers
 */
int getTravelDistance() {

  if(travelDistance < 0)
	{
	  return 0;
	}

  return int(travelDistance / 100000);
}

/**
 * Get Distance Meters
 */
int getTravelDistanceMt() {
  if(travelDistance < 0)
	{
	  return 0;
	}
  return int(travelDistance);
}

int getTravelTime() {
  return CUR_VTT;
}

#endif //ARDUINOMID_CALCULATORS_H
