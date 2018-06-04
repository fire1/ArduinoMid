//
// Created by Angel Zaprianov on 26.4.2018 г..
//

#ifndef ARDUINOMID_COMCHAR_H
#define ARDUINOMID_COMCHAR_H

#include <Arduino.h>
#include "../../MID.h"

//
// Common characters
//

//
// Icons                          //  BD                            /  RG
const char msg73[] PROGMEM = "¥"; // 100km                          / Warning
const char msg74[] PROGMEM = "°"; // celsius                        / -||-
const char msg75[] PROGMEM = "¢"; // instant                        /  overheating
const char msg76[] PROGMEM = "^"; // point
const char msg77[] PROGMEM = "`"; // time
const char msg78[] PROGMEM = "®"; // break
const char msg79[] PROGMEM = "·"; // waste drop
const char msg80[] PROGMEM = "¡"; // road                           / Engine
const char msg81[] PROGMEM = "¶"; // coffee                         / --||--
const char msg82[] PROGMEM = "·"; // temperature
const char msg83[] PROGMEM = "¼"; // connection                     / --||--
const char msg84[] PROGMEM = "½"; // settings
const char msg85[] PROGMEM = "¾"; // petrol station
const char msg86[] PROGMEM = "¿"; // car                            / --||--
const char msg87[] PROGMEM = "¨"; // cloudy                         / --||--
const char msg88[] PROGMEM = "©"; // oven /ice                      / --||--
const char msg89[] PROGMEM = "ª"; // sun
const char msg90[] PROGMEM = "«"; // petrol
const char msg91[] PROGMEM = "¬"; // bottom
const char msg92[] PROGMEM = "®"; // break
const char msg93[] PROGMEM = "¹"; // lamp
const char msg94[] PROGMEM = "ø"; // average
const char msg95[] PROGMEM = "¬"; // Switch arrow
const char msg96[] PROGMEM = "»"; // LPG
const char msg97[] PROGMEM = "¦"; // inside temperature             / --||--
const char msg98[] PROGMEM = "§"; // outside temperature            / --||--
const char msg101[] PROGMEM = " "; // empty space
const char msg102[] PROGMEM = "£"; // checkbox                      / Oil
const char msg103[] PROGMEM = "@"; // Lowe level                    / --||--
const char msg104[] PROGMEM = "¤"; // problem                       / Battery
const char msg105[] PROGMEM = "×"; // delete
const char msg106[] PROGMEM = "Ŵ"; // Info                          / --||-- ?
const char msg107[] PROGMEM = ""; //
const char msg108[] PROGMEM = ""; //
const char msg109[] PROGMEM = ""; //

const char msg116[] PROGMEM = "¯"; // --------                      / DRL
const char msg117[] PROGMEM = ""; //
const char msg118[] PROGMEM = ""; //
const char msg119[] PROGMEM = ""; //

//
// PROGMEM language table
const char *const LcdMsgTable[] PROGMEM = {
        msg0, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9, msg10, msg11, msg12, msg13, msg14, msg15,
        msg16, msg17, msg18, msg19, msg20, msg21, msg22, msg23, msg24, msg25, msg26, msg27, msg28, msg29, msg30,
        msg31, msg32, msg33, msg34, msg35, msg36, msg37, msg38, msg39, msg40, msg41, msg42, msg43, msg44, msg45,
        msg46, msg47, msg48, msg49, msg50, msg51, msg52, msg53, msg54, msg55, msg56, msg57, msg58, msg59, msg60,
        msg61, msg62, msg63, msg64, msg65, msg66, msg67, msg68, msg69, msg70, msg71, msg72, msg73, msg74, msg75,
        msg76, msg77, msg78, msg79, msg80, msg81, msg82, msg83, msg84, msg85, msg86, msg87, msg88, msg89, msg90,
        msg91, msg92, msg93, msg94, msg95, msg96, msg97, msg98, msg99, msg100, msg101, msg102, msg103, msg104,
        msg105, msg106, msg107, msg108, msg109, msg110, msg111, msg112, msg113, msg114, msg115,
        msg116, msg117, msg118, msg119, msg120, msg121, msg122, msg123
};


#endif //ARDUINOMID_COMCHAR_H
