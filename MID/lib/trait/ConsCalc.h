//
// Created by Angel Zaprianov on 27.6.2016 г..
//

#ifndef ARDUINOMID_CONSUMPTION_H
#define ARDUINOMID_CONSUMPTION_H


const int ConsumptionCalibrationReadingsFuel = 10;
const int ConsumptionCalibrationReadingsDistance = 9;


//
// Common Air/Fuel Ratios:
//
//    Natural gas: 17.2
//    Gasoline: 14.7
//    Propane: 15.5
//    Ethanol: 9
//    Methanol: 6.4
//    Hydrogen: 34
//    Diesel: 14.6
//
//
// 56 litres ÷ 800km = 0.07.
// 0.07 x 100 = 7.0 litres (ℓ) per 100 km.
// Therefore, the fuel consumption for that driving period would be 7ℓ/100km
//
//
//                      engine revolutions             fuel consumption
//
// Z18XE Z18XEL	Simtec	         62	                        20

const bool ConCalcDebug = 0;

int injCcMin = 180;
//
// Calculate injection value
long double injectionValue = (injCcMin / 1000 / 60);
//
// Vehicle Used fuel
float CUR_VUF = 0;
//
// Vehicle  instant consumption
float CUR_VIC = 0;

//
// Gets sensed used fuel
float getUsedFuel() {
    return CUR_VUF;
}

//
// Gets sensed instant consumption
float getInstCons() {
    return CUR_VIC;
}

/**
 * gets Instant consumption
 */
void sensCon(void) {

    //
    // Instant consumption
    double injectionOpenTime = ((double) CUR_ECU / 1000000);
    if (CUR_ECU > 3) {
        CUR_VIC = (100 * (((injectionOpenTime * injectionValue) * 3600) * 4)) / CUR_VSS;
    } else {
        CUR_VIC = (((injectionOpenTime * injectionValue) * 3600) * 4) / CUR_VSS;
    }


    //
    // If consumption is more than tank
    if (CUR_VUF > 70) {
        CUR_VIC = 0;
        injectionOpenTime = 0;
    }

    //
    // Calculate total fuel consumption
    CUR_VUF = CUR_VUF + ((injectionOpenTime * injectionValue) * 4);

    if (ConCalcDebug) {
        Serial.print("\n");
        Serial.print(" Ecu Timing:  \t");
        Serial.print(CUR_VIC);
        Serial.print(" Instant is:  \t");
        Serial.print(CUR_VIC);
        Serial.print(" Total used:  \t");
        Serial.print(CUR_VUF);
        Serial.print("\n");
    }

}


#endif //ARDUINOMID_CONSUMPTION_H
