# ArduinoMid
Vauxhall/Opel Astra G MID display replacement with ArduinoMid
This project is rebuild of on board car computer base on 

## Note:
_This project is rebuild of on-board computer for Vauxhall/Opel vehicles 
 created between 2000/2004 with gray(32 pin) MID plug.
In this project is used Mega2560 RV3 board.
The project sourcecode is based on Internet published information!_

* Source code is in "MID" folder
* Used libraries are in "libraries" folder [zip them for importing]

## Version 
   _Code is  in development..._

## ArduinoMid supported features

* Engine RPMs.
* Vehicle speed in KMh RPMs.
* Display back-light works with dashboard lighting.
* Consumption for distance and instance [not tested well]
* Trip time and distance.
* Speed alarm  City / Highway / Out-City
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
* Todo more ...

## Menu structure

-----[Main Menu]-----[Trip Menu]-----[Consumption]-----[Inspection]-----
----------|---------------|---------------|-----------------|-----------
-----[Dashboard]-----[Averages ]-----[Fuel levels]-----[Sensor list]----
----------|--------------------------------------------------*----------
-----[Test Menu]-----


## Schematics
   _Hardware is  in development..._







