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

int CUR_USF = 0;

int injCcMin = 180;
//
// Calculate injection value
long double injectionValue = (injCcMin / 1000 / 60);
int CUR_IFC = 0;

/**
 * gets Instant consumption
 */
void sensCon(void) {

    int injectionOpenTime = ((double) CUR_IPT / 1000000);
    if (CUR_VSS > 3) {
        CUR_IFC = (100 * (((injectionOpenTime * injectionValue) * 3600) * 4)) / CUR_VSS;
    } else
        CUR_IFC = (((injectionOpenTime * injectionValue) * 3600) * 4) / CUR_VSS;

    if (CUR_VSS == 0 && CUR_IFC > 5 || CUR_IFC > 80) {
        CUR_IFC = 0;
        injectionOpenTime = 0;
    }
    CUR_USF += ((injectionOpenTime * injectionValue) * 4);
}

float getUsedFuel() {
    return CUR_USF;
}

float getInstCons() {
    return CUR_IFC;
}


#endif //ARDUINOMID_CONSUMPTION_H
