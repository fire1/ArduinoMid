//
// Created by Angel Zaprianov on 19.3.2018 г..
//

#ifndef ARDUINO_MID_DEBUG_H
#define ARDUINO_MID_DEBUG_H

//
// Debug table
#define DBG_SR_VSS 1    // VSS Vehicle Speed Sensor
#define DBG_SR_RPM 2    // RPM Revolutions Per Minute
#define DBG_SR_ECU 3    // ECU Engine Control Unit
#define DBG_SR_TM1 4    // Outside temperature (front)
#define DBG_SR_TM2 5    // Additional  temperature (1)
#define DBG_SR_TM3 6    // Additional  temperature (2)
#define DBG_SR_CNS 7    // Fuel consumption calculation
#define DBG_SR_IFC 8    // Instant consumption calculation
#define DBG_SR_DIM 9    // Display back light level
#define DBG_SR_ENT 10   // Engine coolant temperature
#define DBG_SR_TNK 11   // Fuel tank level
#define DBG_SR_DST 12   // Vehicle distance measured
#define DBG_SR_GRS 13   // Vehicle gear ratio resolver
#define DBG_SR_AVR 14   // Vehicle averages assumed
#define DBG_SR_MNI 16   // UI User Interface menu
#define DBG_SR_MNB 17   // UI button handler
#define DBG_SR_EPR 18   // Internal chip memory
#define DBG_SR_WHL 19   // Steering wheel buttons
#define DBG_SR_RCR 20   // Remote control  radio
#define DBG_SR_STT 21   // Vehicle servicing states
#define DBG_SR_STW 22   // Vehicle warning states
#define DBG_SR_SAL 23   // Speed alarm
#define DBG_SR_LPG 24   // LPG serial communication

static uint8_t CmdSerialDebugging;
/***************************************************************************
 * Debug
 ***************************************************************************/
// serial print macros
#define dump_init(...) { Serial.begin(__VA_ARGS__);  } //initialization

/**
 * Debug from given serial string over 1 second amplitude
 * @param amp
 * @param cmd
 * @return
 */
inline boolean cmd(AmpTime *amp, const uint8_t cmd) {
    return (amp->isSecond() && CmdSerialDebugging == cmd) ? true : false;
}

/**
 * Debug from given serial string over "SEC" amplitude
 * @param amp
 * @param cmd
 * @return
 */
boolean cmdMin(AmpTime *amp, const uint8_t cmd) {
    return (CmdSerialDebugging == (cmd) && amp->isSec()) ? true : false;
}

/**
 * Debug from given serial string over "MAX" amplitude
 * @param amp
 * @param cmd
 * @return
 */
boolean cmdMid(AmpTime *amp, const uint8_t cmd) {
    return (CmdSerialDebugging == (cmd) && amp->isMax()) ? true : false;
}

/**
 * No time amplitude
 * @param cmd
 * @return
 */
boolean cmdLive(uint8_t cmd) {
    return (CmdSerialDebugging == (cmd)) != 0;
}

#define dump(msg, data){Serial.print(F(msg)); Serial.print(F(":\t "));Serial.println(data);}
#define dump_bin(msg, data){Serial.print(F(msg)); Serial.print(F(":\t "));Serial.println(data,BIN);}
#define dump_hex(msg, data){Serial.print(F(msg)); Serial.print(F(":\t "));Serial.println(data,HEX);}
#define dump_dec(msg, data){Serial.print(F(msg)); Serial.print(F(":\t "));Serial.println(data,DEC);}
#define dump_txt(msg){Serial.println(F(msg)); }
#define dump_title(msg){Serial.println(F(msg));Serial.println(F("==============================================================")); }
//

#endif //ARDUINOMID_DEBUG_H
