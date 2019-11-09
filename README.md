# MIDIPedal
A simple 3 buttons midi pedal (Start/Stop/Tempo+-)


## Hardware required

  - An Arduino (Nano is plenty enough).
  - A momentary footswitch
  - 2 buttons
  - An enclosure
  - A MIDI DIN5 female connector
  - A 9V female power supply connector
  - 220 Ohm resistor
  
Setup is standard, simply use https://www.arduino.cc/en/tutorial/midi for MIDI and https://www.arduino.cc/en/tutorial/button for the 3 buttons.
Footswitch is expected to be connected on D3, Tempo-- to D4 and Tempo++ to D5 (adapt FOOTSWPIN, WHITESWPIN and BLACKSWPIN accordingly).

## Operation

Press the footswitch, the pedal sends a MIDI Start event.
Press again, the pedal sends a MIDI Stop even.

Once ready, the pedal sends clock events at regular interval (default BPM is 120).

Press tempo+ or tempo- to change de default tempo, the clock will be updated.

## Tests and warranty

Tested successfully with my Novation Circuit.
Warranty ? haha.. :)
