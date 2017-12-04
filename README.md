# ArduinoMid
Vauxhall/Opel Astra G (Astra G & Frontera) MID display replacement with ArduinoMid
This project is rebuild of on-board computer  base on original interface connection.


![https://github.com/fire1/ArduinoMid/blob/master/screens/Screenshot_2017-07-02-17-07-38.jpg](https://github.com/fire1/ArduinoMid/blob/master/screens/Screenshot_2017-07-02-17-07-38.jpg)

## Note:
_This project is rebuild of on-board computer for Vauxhall/Opel vehicles 
 made  between 2000/2004 with gray(32 pin) MID plug.
In this project is used Mega2560 RV3 board.
The project sourcecode is based on Internet published information!_

## Notes
* Source code is in "MID" folder. To upload  softer use MID.ino file.
* Before uploading check software configuration in MID.h file.  
* Configuration is suppurated  is over two files, default MID.h and your custom conf.h
* Make your custom configuration into conf.h file 
* Used libraries are in "libraries" folder [zip them for importing]
* [Extra information for project](https://docs.google.com/document/d/11Yb3lh6lio_FKBEzur-n49dYgrX5xFf_UQObUGn2u_c/edit?usp=sharing)

## Version 
   **1.5.6** _and should be considered as beta (test) quality._

## ArduinoMid supported features

* Multi screen support. For now 16x2 and 240x64.
* Engine RPMs.
* Vehicle speed in KMh RPMs.
* Display back-light works with dashboard lighting.
* Consumption for distance and instance [not tested well]
* Trip time and distance.
* Speed alarm  City / Highway / Out-City
* Smart Speed alarm auto off/on
* Average speed braking and engine revolutions.
* Outside temperature [needs more tests / TODO inside temperature]
* Steering wheel buttons handler.
* Support Sony xPlod to vehicles steering wheel media buttons.
* Fuel gauge/switch support [analog detection from fuel switch].
* Car Inspection menu detects critical level for  Oil, Window washer, Brake ware, voltage [Todo lamps, belt, and other ]
* Engine temperature.
* Power saving operation [factory Bord. Comp. have energy drain when engine is off ...]
* Check menu - based on worked time / distance [Todo]
* Brake pedal as button [Todo ]
* Resolving driving gears 
* Sport mode [Todo ... luring ECU signal]
* Two fuel lines supported [For benzine & propane-butane ... ]
* [TODO] Breaking time detection
* [Adittional] Stopwatch menu
* [Adittional] Drag racing menu [Todo ... Gets best speed / time for 402 maters]
* [Adittional] From 0 to 100km [Todo ... Gets acceleration time from 0 to 100 kilometers]
* [Adittional] Trip distance/time save
* Block the ignition [hiddant code to detect owner of the car]
* Todo more ...

## Menu structure for screen 16x2 
[deprecated for newer version since is not tested]

    -----[Main Menu]-----[Trip Menu]-----[Consumption]-----[Servecing]------
    ----------|---------------|---------------|-----------------|------------
    -----[Dashboard]-----[Averages ]-----[Fuel levels]-----[Sensor list]-----
    ----------|--------------------------------------------------*-----------
    -----[Test Menu]---------------------------------------------*-----------
    -------------------------------------------------------------*-----------
    -------------------------------------------------------------*-----------

## Menu structure for screen 240x64

    -----[Main Menu]-----[Trips Record]-----[Servecing]------
    ----------|-----------------|----------------------------
    -----[Dashboard]-----[Fuel Systems]
    ----------|----------
    -----[Settings]------
    ---------|-----------
    -----[About]---------


## Schematics
   _Hardware is  in development..._
   
  Development PCB/Schematic: [Beta version](https://easyeda.com/fire1.a.zaprianov/MID2-a3445ff633e5483aa7c6525f325a687a)
**  NOT YET TESTED VERSION!  **

    


## Printable PCB
   
   [Donwload printable PCB layers](https://github.com/fire1/ArduinoMid/raw/master/PCB/amid-pcb.pdf)
   
   [Donwload component outlines](https://github.com/fire1/ArduinoMid/raw/master/PCB/amid-e.pdf)
        
        
        
## Donations
    Every cent will be used for new futures. 
    Keep in mind I spend a lot of money and time over this project... 
    
    Also my girlfriend promise me to lock me into a garage 
    if I do not stop spending money  here ... so  ... HELP! ;)
    
### [Donate!](https://paypal.me/AngelZaprianov)
   
   





