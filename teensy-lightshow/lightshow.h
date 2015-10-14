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

    void applyDecay(
      float &energy,
      float decay,
      float energyFloor,
      float energyCeiling
    ) {
      energy = constrain(energy - decay, energyFloor, energyCeiling);
    }

};

#endif
