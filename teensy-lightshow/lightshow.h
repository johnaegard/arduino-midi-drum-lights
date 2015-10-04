#ifndef Lightshow_h
#define Lightshow_h

#include <OctoWS2811.h>
#include <Arduino.h>
#include "td15_midi_notes.h"
#include "constants.h"
#include "pixels.h"
#include "colors.h"

class Lightshow {
  public:
    virtual void decay() = 0;
    virtual void updatePixels() = 0;
    virtual void reset() = 0;
    virtual void handleNoteOn(byte channel, byte instrument, byte velocity) = 0;
    virtual void handleNoteOff(byte channel, byte instrument, byte velocity) = 0;

    Lightshow() {}

  protected:
    OctoWS2811 *_pixels;
    byte s, p;
    float _decay;

    void applyDecay(
      float &energy,
      float decayFloor,
      float decayCeiling,
      float energyFloor,
      float energyCeiling
    ) {
      //      _decay = computeDecay( analogRead(DECAY_POT_PIN) / 128, 0, 8, decayFloor, decayCeiling);
      _decay = computeDecay( 8, 0, 8, decayFloor, decayCeiling);
      energy = constrain(energy - _decay, energyFloor, energyCeiling);
    }

    float computeDecay(byte x, float in_min, float in_max, float out_min, float out_max) {
      return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
    }
};

#endif
