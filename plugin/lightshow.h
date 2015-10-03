#include <midi_Message.h>
#include <midi_Namespace.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Settings.h>
#include <Arduino.h>
#include <FastLED.h>

#ifndef Lightshow_h
#define Lightshow_h

#define NUM_PIXELS_PER_STRIP 60

class Lightshow
{
  public:
    Lightshow(midi::MidiInterface<HardwareSerial> *m,CRGB pixels[][NUM_PIXELS_PER_STRIP],CRGB::HTMLColorCode color);
    void decay();
    void updatePixels();
    void reset();
    void handleNoteOn(byte channel, byte instrument, byte velocity);
    void handleNoteOff(byte channel, byte instrument, byte velocity);

  private:
    CRGB::HTMLColorCode _color;
    float _fEnergy;
    CRGB (*_pixels)[NUM_PIXELS_PER_STRIP];
    byte s, p;
    bool _mustSweep = true;
    midi::MidiInterface<HardwareSerial> *_midi;
};

#endif

