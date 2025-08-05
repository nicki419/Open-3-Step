# Open 3 Step

(WIP) Open-Source Circuit and code for custom built 120/240V 3-step (or  n-step) light bulb dimmer working by turning off/on the light switch  quickly to switch between brightness levels. Also provides  memory-functionality to non-memory 3-step bulbs.

---

### Background

I want to build a lamp. After making a beautiful frame for it, 3D-designing and printing bulb holders and other components, and buying designer LED bulbs (3 pairs of different bulbs, for a total of 6), I discovered that four of those bulbs were 3-step dimmable, and two were TRIAC dimmable. I therefore set out to make my TRIAC bulbs 3-step dimmable, with a memory function, so they could work with my 3-step bulbs.

I soldered a 5V power supply with some fuses, capacitors and resistors to a 30x70mm prototyping board, did the same for an ATMEGA328P, an EEPROM and some GPIO headers, wrote the arduino sketch (see v0.1 code) and it worked, after lots of debugging. If not for the next thing I discovered, this repository would not exist.

The dimming worked. The memory worked. Then it hit me. All four of my 3-step dimmable bulbs are from the same company, and have the exact same box design, with a myriad of icons on the side showing its features. Only after making v0.1 did I realise that two of the bulbs did NOT have a memory function, and two more did. So I now had four bulbs with memory, and two without. Since I also bought regular AC switch modules next to the RBDimmer module, I decided to make another version that would also control the two 3-step bulbs without memory.

Over the summer, I learned the basics of PCB Design and ordered my boards for the next version. I got some more components to make this project more easily customisable and added potentiometers for more easy brightness control at the three steps, as well as support for the non-memory 3-step bulbs.


### Versions

**0.2**

Work in progress. This version will implement the features described above on a custom PCB. All the components will, however, also fit on a 30x70mm prototyping board, if you really want to go that route.

At the moment, there is no release. If you want to have an early peek at the files, you must do so on your own. To not clutter the repo, I will share exported PDFs, gerber files, STLs, etc. only via releases.

(Proposed) Changelog:

- Add Support for non-memory 3-step bulbs through fast switching when the system powers up
- Add analogue input via 100K potentiometers and a button to configure the brightness levels outside of code
- Move from arduino to platformio and add separate debug and release profiles (release will have no LED blinking)


**0.1**

The original version is fully implementable on prototyping boards. This is also where the power supply comes from. I chose not to create schematics for the power supply, but there is documentation on what's required and recommended when creating it. If you only need 3-step dimming with a memory function, you may use the code from both v0.1 and v0.2. They should hopefully be cross-compatible, as the new board only adds things, it doesn't modify any existing pins. Consult the documentation of the v0.2 code once it fully releases on how to do this. During development, you probably want to use the v0.1 code, instead.

Therefore, you can refer to the schematics for v0.2 on how to wire everything. You simply need to leave out the push button, potentiometers, and the additional GPIO pin for the AC switch. You also need to configure the values for your bulb in code. For v0.2, I will place these in a header file, for v0.1, there is an array defined at the top of the code.
