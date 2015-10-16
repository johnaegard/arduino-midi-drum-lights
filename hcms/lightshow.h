#ifndef Lightshow_h
#define Lightshow_h
#include <Arduino.h>
#include "td15.h"
#include "constants.h"
#include "pixels.h"
#include "colors.h"
#define HowBigIsThisArray(x) (sizeof(x) / sizeof(x[0]))

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
