# How To Install and Setup the Dimmer

1. Download the latest release. It contains STL files for mounts, schematics pdf and gerber files for the PCB, and compiled debug and release firmware for delays of 100ms to 5000ms (explained later)
2. Read through mounts.md and parts_list.md. Order the PCB if you don't want to solder on a protoboard (highly recommended)
3. Solder all components, prepare the boards, wire everything up.
4. Load the debug 5000ms firmware onto your 328P, and "boot" the system.
5. The lamp will either be on full brightness, or off. This is normal.

## Setting up the brightness levels

First things first - you need to decide in what order you want to dim. It can either be HIGH - MEDIUM - LOW or LOW - MEDIUM - HIGH. If you use this system, like me, with another 3-step dimmable lamp, whether that has a memory or not, you need to use the order it uses.

To enter settings mode, double click the button. Your lamp will now assume the brightness the first potentiometer is set to. You can turn the potentiometer, then click the button once. That will show you the new brightness, and also save it to the EEPROM. When setting the lowest brightness, some TRIAC dimmable bulbs have a minimum they can handle on boot, which might be higher than the minimum you can set it to. Mine, for instance, when at its absolute lowest can not turn on from a state of prolonged off-time to it's lowest possible brightness, so I had to configure it to be some 10% brighter than it's theoretical lowest.

Double tap again to exit settings mode. The step you were currently on is the step that will be used. Hence, the settings mode also serves to toggle through the three steps, to sync it up with your 3-step bulbs with their own memory.


## Measuring the delay of your 3-step bulbs

When using this together with regular 3-step bulbs, you'll want to know their max switching delay. You need to match the firmware you use for that delay, hence the release contains delays from 100ms to 5000ms. When not in settings mode, hold the button to enter the calibration mode.

Two important things to note: While calibrating, do not rest your finger on the button. That might trigger a button hold and leave calibration mode. You will also need your phone to be able to record timelapse at 1/2, ideally 1/4 so you can count easier. When testing with a bulb that does not have its own memory, take not of its default state. You DO NOT want to test when on the state before that, because you would not be able to tell it apart.
For example, my bulb defaults to HIGH. I therefore made sure when testing delays it was on MEDIUM. In case the delay tested does trigger it, it would switch to LOW. In case it did not, it would switch to HIGH. That's how I knew I'd foudn my delay. In my bulb, it stopped switching at 4100ms, so my max delay is 4000 ms.

Press the button to toggle at the current delay. It starts at 100ms. The LED will be ON once for every 100ms of delay. Double click the button to increase the delay by 100ms. Do not bother counting. Just keep increasing and testing until you've found the FIRST delay where your lamp no longer switches. Press the button once to repeat that delay, and record the LED with your phone in slow motion. You can then count how often it blinks easily, subtract 100ms, and you'll know your delay.


## The issue of uptime

Capacitors degrade over time. Up to 30% can be expected. I therefore recommend you add, on another protoboard, as many capacitors as necessary according to the following (crude) calculation:

(Your delay + 500ms)*1.3

That will give you your required uptime. Add enough capacitors to reach it. I found each cap adds roughly 500ms, but that'S based on very crude measurements. I consider the system dead as soon as the LED dies, because it's driven by the 328P. I therefore assume as long as the LED is on, the system is live.

Test on a breadboard how many caps you need, add one for good measure, and you're hopefully good to go.
