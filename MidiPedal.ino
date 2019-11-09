/**
  Type: Arduino Uno
  Port : wchusbserial
  Processor: ATMega328P (old bootloader)

  http://fortyseveneffects.github.io/arduino_midi_library/
*/

#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define LEDPIN 2 //D2
#define LED_OFF  LOW
#define LED_ON  HIGH

#define FOOTSWPIN 3 //D3
#define BLACKSWPIN    4 //D4
#define WHITESWPIN    5 //D5
#define PRESSED  HIGH
#define RELEASED LOW
#define INCREMENT 100

bool buttonTrig = false;
long buttonTrigReleaseTs = 0;
bool started = false;
long lastClockTs = 0;
int BPM = 120;
long clockIntervalMicroSec = 0;



void setup() {
  
  MIDI.begin();
  pinMode(LEDPIN, OUTPUT);
  pinMode(FOOTSWPIN, INPUT);
  pinMode(BLACKSWPIN, INPUT);
  pinMode(WHITESWPIN, INPUT);
  updateClockInterval();
}

void updateClockInterval() {
  /*
    x BPMs quarter notes per minute and 24 clocks per quarter => x * 24 clocks per minute.
    interval in microseconds is 60000000/(24 * x) => 2500000/x
  */
  clockIntervalMicroSec = 2500000/BPM;
}

void loop() {

//All buttons released ? ready for a new action
  if (buttonTrig == true) {
    if ((digitalRead(FOOTSWPIN) == RELEASED) && 
        (digitalRead(BLACKSWPIN) == RELEASED) && 
        (digitalRead(WHITESWPIN) == RELEASED)) {

      //Avoid ghost triggering
      if (buttonTrigReleaseTs == 0) {
        buttonTrigReleaseTs = millis() + 100;
      } else if (millis() > buttonTrigReleaseTs) {
        buttonTrig = false;
        buttonTrigReleaseTs = 0;
      }
    }
  }

  //Start / Stop
  if ((buttonTrig == false) && (digitalRead(FOOTSWPIN) == PRESSED)) {
    digitalWrite(LEDPIN, LED_ON);
    if (started == false) {
      MIDI.sendRealTime(MIDI_NAMESPACE::Start);
    } else {
      MIDI.sendRealTime(MIDI_NAMESPACE::Stop);
    }
    started = !started;
    delay(100);
    digitalWrite(LEDPIN, LED_OFF);
    buttonTrig = true;
  }

  // Tempo --
  if ((buttonTrig == false) && (digitalRead(BLACKSWPIN) == PRESSED)) {
    digitalWrite(LEDPIN, LED_ON);
    digitalWrite(LEDPIN, LED_OFF);
    BPM--;
    if (BPM < 50) {
      BPM = 50;
    }
    updateClockInterval();
    buttonTrig = true;
  }

  //Tempo ++
  if ((buttonTrig == false) && (digitalRead(WHITESWPIN) == PRESSED)) {
    digitalWrite(LEDPIN, LED_ON);
    digitalWrite(LEDPIN, LED_OFF);
    BPM++;
    if (BPM > 200) {
      BPM = 200;
    }
    updateClockInterval();
    buttonTrig = true;
  }


  if (micros() - lastClockTs >= clockIntervalMicroSec) {
    lastClockTs = micros();
    MIDI.sendRealTime(MIDI_NAMESPACE::Clock);
  }
}
