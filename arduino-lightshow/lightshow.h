#ifndef Lightshow_h
#define Lightshow_h

#include <Arduino.h>
#include "td15_midi_notes.h"

#define DECAY_POT_PIN A5
#define NUM_PIXELS_PER_STRIP 60
#define MIDDLE_PIXEL NUM_PIXELS_PER_STRIP / 2

class Lightshow {
  public:
    virtual void decay() = 0;
    virtual void updatePixels() = 0;
    virtual void reset() = 0;
    virtual void handleNoteOn(byte channel, byte instrument, byte velocity) = 0;
    virtual void handleNoteOff(byte channel, byte instrument, byte velocity) = 0;

    Lightshow() {
      pinMode(DECAY_POT_PIN, INPUT);
//      _decayCache = {0, 0, 0, 0, 0, 0, 0, 0};
    }

  protected:
    CRGB (*_pixels)[NUM_PIXELS_PER_STRIP];
    byte s, p;
    midi::MidiInterface<HardwareSerial> *_midi;
    float _decay;
    float _decayCache[9] = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

    void applyDecay(
      float &energy,
      float decayFloor,
      float decayCeiling,
      float energyFloor,
      float energyCeiling
    ) {
      _decay = computeDecay( analogRead(DECAY_POT_PIN) / 128, 0, 8, decayFloor, decayCeiling);
      energy = constrain(energy - _decay, energyFloor, energyCeiling);
    }

    float computeDecay(byte x, float in_min, float in_max, float out_min, float out_max) {
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
      if ( _decayCache[x] == 0.0 ) {
        _decayCache[x] = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
      }
      return _decayCache[x];
    }
};

#endif
