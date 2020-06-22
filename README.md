# arduino_CVToMIDI
Simple CV to MIDI arduino sketch. 

## Features
* Supports CV analog auto calibration (auto range)
* The Two MIDI-Controller data channels are displayed on a SSD1306 as a two waveforms
* The CC number can be set (up/dw) using the two buttons

# Prerequisites
Before compiling install "MIDI Library" and "Adafruit SSD1306" from the arduino IDE Library Manager 

# Resources

## Setup

![Alt text](resources/fritzin_sketch.jpg?raw=true "CVToMIDI fritzing sketch")

![Alt text](resources/arduino_CVtoMIDI.jpg?raw=true "CVToMIDI prototype")

# Granular Theremin - in action

[![IMAGE ALT TEXT HERE](http://img.youtube.com/vi/HBXVmIHtNC8/0.jpg)](http://www.youtube.com/watch?v=HBXVmIHtNC8)

Wire a CV generator to the CV input jacks of the breadboard and the breadboards MIDI out to the MIDI in of a Synth like e.g. crusher-X (https://www.accsone.com/) - load the patch and the MIDI Map into crusher-X:

![Alt text](crusher-X/crusherx.jpg?raw=true "accSone crusher-X")
