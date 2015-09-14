#include <MIDI.h>
#include <limits.h>

MIDI_CREATE_DEFAULT_INSTANCE();

// -----------------------------------------------------------------------------

// This example shows the old way of checking for input messages.
// It's simpler to use the callbacks now, check out the dedicated example.

#define LED 13                   // LED pin on Arduino Uno

// -----------------------------------------------------------------------------

void on()      // Basic blink function
{
  digitalWrite(LED,HIGH);
}

void off()
{
  digitalWrite(LED,LOW);
}

// ----------------------------------------------------------------------------

void setup()
{
  pinMode(LED, OUTPUT);
  MIDI.begin(MIDI_CHANNEL_OMNI); 
}

void loop(){
  if (MIDI.read())                // Is there a MIDI message incoming ?
  {
    switch(MIDI.getType())   { 
      case midi::NoteOn:      
      on();
      break;
      case midi::NoteOff:
      off();
      break;
    }
  }
}
