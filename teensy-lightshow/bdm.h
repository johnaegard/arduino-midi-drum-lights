#ifndef bdm_h
#define bdm_h

#include "lightshow.h"
#define NUM_STARS 15
#define MILLIS_BETWEEN_TWINKLES 75
#define TWINKLE_FLOOR 0.50f
#define VELOCITY_CEIL 0.00005f
#define FIRST_PIXEL_CEIL 15

struct Star {
  RGBB color;
  byte strip;
  byte oldStrip;
  float pixel;
  byte oldPixel;
  float velocity;

  String toString() {
    return String("color=") + color.toString() + String(", pixel=") + pixel + String(", velocity=") + velocity;
  }
};

class BDMLightshow : public Lightshow
{
  public:
    BDMLightshow(OctoWS2811 *p);
    void decay();
    void updatePixels();
    void reset();
    void handleNoteOn(byte channel, byte instrument, byte velocity);
    void handleNoteOff(byte channel, byte instrument, byte velocity);

  private:
    void resetStar(byte s);
    Star _stars[NUM_STARS];
    unsigned long _previousTwinkleMillis = millis();

    const RGBB bdmPalette[7] = { LIGHTBLUE, LIGHTRED, LIGHTGREEN, LIGHTPURPLE, LIGHTYELLOW, LIGHTTEAL, LIGHTORANGE};

};
#endif
