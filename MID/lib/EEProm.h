//
// Created by Angel Zaprianov on 28.6.2016 г..
//

#define eepromSize 512          //512 for atmega168, 1024 for 328

#define TrackSpeedGas           // Define this to track gas used at different speeds.
// Costs 85ram + 814 bytes/v0016  766 bytes/v0011
#define SaveTankToEeprom        // Define this to enable Tank data save/restore/view
// Save/Restore current "live" tank dataset, or
// Archive & View tank summaries
// Costs 1432 bytes/v0016  1366 bytes/v0011
//#define SaveCurrentToEeprom   // Uncomment this if you also want to protect Current Trip from power-loss
// If your Current auto-clears after 7 minutes, you probably won't need this

#define eeParmStart  4  //eeprom location, beginning of parms[]

#define savetanksig   B11101101
#define eeTankSig       eepromSize-1    //last byte of eeprom
#define eeTankQueueIdx  eepromSize-2    //2nd last byte of eeprom, contains queue # of last tank summary saved

#ifdef SaveCurrentToEeprom
#define eeHoldTankSize   (sizeof(Trip)+sizeof(Trip)+sizeof(injSpeedHiSec)+sizeof(injSpeedHius))
//136 = 36+36+32+32  ... or 36+36+1+1, if !TrackSpeedGas
#else
#define eeHoldTankSize   (sizeof(Trip)+sizeof(injSpeedHiSec)+sizeof(injSpeedHius))
//100 = 36+32+32   ... or 36+1+1, if !TrackSpeedGas
#endif

#define eeHoldTankStart  (eeTankQueueIdx - eeHoldTankSize)
//start byte of where to store live tank data (in event of power loss)
#define eeHoldCurrentStart  (eeHoldTankStart + sizeof(Trip))
//start byte of where to store live current-trip data (in event of power loss)
#define eeHoldSpeedGasStart  (eeTankQueueIdx - sizeof(injSpeedHiSec) - sizeof(injSpeedHius))
#define eeTankArchiveStart  (eeHoldTankStart - eeTankArchiveSize)
#ifdef TrackSpeedGas
#define eeTankArchiveSize   (9+11+4)
#else
#define eeTankArchiveSize   (11+4)
#endif
//      15 bytes to save: 99.99 gallons, 99.99 idle, 99.99 mph, 99.99 eoc, 9999.99 miles, 4-byte decimal note (2+2+2+2+3+4)
//      optionally, store additional 9 bytes (1 3-digit percent of gas-used for 8 speed ranges + idle)

#define eeTankQueueMax   ((eeHoldTankStart-eeParmStart-sizeof(parms)-128) / eeTankArchiveSize)
//# of 20-byte tank summaries we will store.  store at end of eeprom, leave lower space for parms & future growth
//start_loc_of_temp_tank_save) - (parms overhead) - 128 (future growth)

#define READ  0  //for eeprom_block()
#define WRITE 1  //for eeprom_block()
#define LeftButton ((buttonState & lbuttonBit)==0)
#define MiddleButton ((buttonState & mbuttonBit)==0)
#define RightButton ((buttonState & rbuttonBit)==0)
