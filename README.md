# Arduino pH Corrector Shield v 2.0
[RU](https://github.com/cyberplantru/Arduino-pH-Corrector-Shield-v2.0)

upd. 17/04/2014 New in sketch v2.5 - add function for calibration pumps/tubes, log file in EEPROM, function "reset all" and "save all"


The a sample code for automatic control pH level.
__________

# Description


pH Corrector Shield is Arduino compatible board,
intended for automation of the control of hydrogen ion concentration in a liquid medium

[![ScreenShot](https://raw.github.com/GabLeRoux/WebMole/master/ressources/WebMole_Youtube_Video.png)](http://www.youtube.com/watch?v=vZGudOGjutk)

Additional information can be found on the project page [link](http://www.cyberplant.info).

### Specifications

![pH Correcor Shield](http://cyberplant.ru/modules/blogwp/wordpress/wp-content/uploads/2014/01/pH_Corrector_Shield_400.jpg)

The Shield uses the following contacts Arduino:

A0 - button Set
A1 - button Menu
A2 - button Down
A3 - button Up

8, 9, 10, 11, 12, 13 - display

3, 5 - tx, rx pH Circuit

6, 7 - pumps pHDown, pHUp

Power supply for the pumps is taken from Vin Arduino

Files PCB [gerber](http://cyberplant.ru/modules/blogwp/wordpress/wp-content/uploads/2014/01/pH_Shield_2.5.zip).

[fritzing project](http://fritzing.org/projects/ph-corrector-shield-v20)

# Assembling

![pH Correcor Kit](http://cyberplant.ru/modules/blogwp/wordpress/wp-content/uploads/2014/01/pH_kit_1_400.jpg)

To assemble the device will need the following items:
	1. Cyberplant pH Correcnor Shield
	2. Arduino Uno or Arduino Mega 
	3. pH Circuit
	4. pH sensor
	5. power supply unit 
	6. Couple peristaltic pump
	7. display



# Installing the Library

In the sketch used library EEPROMex
for nonvolatile memory 

Learn how to install the library on the site of Arduino: <http://arduino.cc/en/Guide/Libraries>.


### ways of expanding

Board is designed in such a way that allows you to connect 
Shield for the remote control, the same as the Ethernet Shield, 
WiFi Shield, Bluetooth Shield.

It is also possible to use instead of pH Circuit other sensors boards from Atlas Scientific, such the Conductivity Circuit etc. 

Also on the Arduino board after installing pH Corrector Shield remain free contacts, which can also be used to connect a variety of sensors and control devices.

# FAQ

Before using this Arduino sketch is recommended 
reset the EEPROM on this monual: <http://arduino.cc/en/Tutorial/EEPROMClear>
The firmware 2.5 is sufficient to perform the function RESET ALL in the menu.