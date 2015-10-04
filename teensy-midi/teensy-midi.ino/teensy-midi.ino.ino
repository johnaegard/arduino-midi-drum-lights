#include <MIDI.h>

/* MIDI Input Test - for use with Teensy or boards where Serial is separate from MIDI
 * As MIDI messages arrive, they are printed to the Arduino Serial Monitor.
 *
 * Where MIDI is on "Serial", eg Arduino Duemilanove or Arduino Uno, this does not work!
 *
 * This example code is released into the public domain.
 */

#include <MIDI.h>

void setup() {
  MIDI.setHandleNoteOn(handleNoteOn);  // Put only the name of the function
  MIDI.setHandleNoteOff(handleNoteOff);
  //  MIDI.setHandleActiveSensing(handleActiveSensing);
  MIDI.setHandleProgramChange(handleProgramChange);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  //  Serial.begin(57600);
  //  Serial.println("MIDI Input Test");
  pinMode(14, OUTPUT);
  pinMode(13, OUTPUT);
}

unsigned long t = 0;

void loop() {
  MIDI.read();
}

void handleNoteOn(byte channel, byte note, byte velocity) {
  on();
  //  Serial.println(String("Note On:  ch=") + channel + ", note=" + note + ", velocity=" + velocity);
}

void handleNoteOff(byte channel, byte note, byte velocity) {
  //  Serial.println(String("Note Off: ch=") + channel + ", note=" + note + ", velocity=" + velocity);
  off();
}

void handleProgramChange(byte channel, byte number) {
  Serial.println(String("Program Change: ch=") + channel + ", number=" + number );
}

bool lightToggle = false;
void handleActiveSensing() {
  digitalWrite(13, lightToggle ? HIGH : LOW );
  lightToggle = ! lightToggle;
}

void on() {
  digitalWrite(13, HIGH);
}

void off() {
  digitalWrite(13, LOW);
}

