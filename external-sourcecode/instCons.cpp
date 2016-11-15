// instant fuel consumption






void getInstantConsumption(char *retbuf) {
    long toggle_speed;
    long cons;
    char decs[16];

    toggle_speed = params.per_hour_speed;
    if (!params.use_metric)  // convert toggle speed to km/h if it's set in IMP
        toggle_speed = (toggle_speed * 1609) / 1000;

    // VSS Vehicle speed sensor
    //
    // divide MAF by 100 because our function return MAF*100
    // but multiply by 100 for double digits precision
    // divide MAF by 14.7 air/fuel ratio to have g of fuel/s
    // divide by 730 (g/L at 15°C) according to Canadian Gov to have L/s
    // multiply by 3600 to get litre per hour
    // formula: (3600 * MAF) / (14.7 * 730 * VSS)
    // = maf*0.3355/vss L/km
    // mul by 100 to have L/100km

    // if maf is 0 it will just output 0
    if (vss < toggle_speed)
        cons = (maf * 3355) / 10000;  // L/h, do not use float so mul first then divide
    else
        cons = (maf * 3355) / (vss * 100); // L/100kmh, 100 comes from the /10000*100

    if (params.use_metric) {
        long_to_dec_str(cons, decs, 2);
        sprintf_P(retbuf, pctspcts, decs, (vss < toggle_speed) ? "L\004" : "\001\002");
    }
    else {
        // MPG
        // 6.17 pounds per gallon
        // 454 g in a pound
        // 14.7 * 6.17 * 454 * (VSS * 0.621371) / (3600 * MAF / 100)
        // multipled by 10 for single digit precision

        // new comment: convert from L/100 to MPG

        if (vss < toggle_speed)
            cons = (cons * 10) / 378;   // convert to gallon, can be 0 G/h
        else {
            if (cons == 0)             // if cons is 0 (DFCO?) lcdDisplay 999.9MPG
                cons = 9999;
            else
                cons = 235214 / cons;     // convert to MPG
        }

        long_to_dec_str(cons, decs, 1);
        sprintf_P(retbuf, pctspcts, decs, (vss < toggle_speed) ? "G\004" : "\006\007");
    }
}