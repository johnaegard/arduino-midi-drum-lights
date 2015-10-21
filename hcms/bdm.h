#ifndef bdm_h
#define bdm_h

#include "lightshow.h"
#define NUM_DROPS 30
#define MILLIS_BETWEEN_TWINKLES 100
#define DROP_TWINKLE_FLOOR 0.6f
#define DROP_VELOCITY_FLOOR 0.1f
#define DROP_VELOCITY_RANGE 0.5f
#define DROP_VELOCITY_ACCEL 0.00075f
#define CONTRAIL_BRIGHTNESS_1 0.5f
#define CONTRAIL_BRIGHTNESS_2 0.35f
#define CONTRAIL_BRIGHTNESS_3 0.15f
#define STAR_TWINKLE_FLOOR 0.4f
#define STAR_TWINKLE_CEIL 0.8f
#define NUM_STARS 16
#define STAR_FLOOR 0
#define STAR_CEILING 60

struct Star {
  RGBB color;
  byte strip;
  byte pixel;
  float twinkleBrightness;
};

struct Drop {
  RGBB color;
  byte strip;
  byte oldStrip;
  float pixel;
  byte oldPixel;
  float velocity;
  int contrail1Pixel;
  int contrail2Pixel;
  int contrail3Pixel;
  int contrail1OldPixel;
  int contrail2OldPixel;
  int contrail3OldPixel;
  float twinkleBrightness;

  String toString() {
    char fmtVelocity[7];
    sprintf(fmtVelocity, "%7.5f", velocity);
    return String("color=") + color.toString() + String(", pixel=") + pixel + String(", velocity=") + fmtVelocity;
  }

  boolean hasMoved() {
    return ( ( oldPixel != (byte)(pixel) ) || (oldStrip != strip ));
  }

  byte wholePixel() {
    return (byte) pixel;
  }

  void updateContrails() {
    contrail1Pixel = wholePixel() - 1;    
    contrail2Pixel = wholePixel() - 2;    
    contrail3Pixel = wholePixel() - 3;    
  }

  void updateHistory() {
    oldPixel = wholePixel();
    oldStrip = strip;
    contrail1OldPixel = contrail1Pixel;
    contrail2OldPixel = contrail2Pixel;
    contrail3OldPixel = contrail3Pixel;
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
    void resetDrop(byte d);
    void resetStar(byte s);
    void debugAllDrops();
    void blackoutOldPixels(byte d);
    void lightDropPixels(byte d);
    void lightStarPixel(byte s);
    void crash();

    Drop _drops[NUM_DROPS];
    Star _stars[NUM_STARS];
    bool _cometMode;
    
    unsigned long _previousTwinkleMillis = millis();
    const RGBB bdmPalette[10] = { LIGHTBLUE, LIGHTGREEN, LIGHTYELLOW, LIGHTTEAL, LIGHTORANGE, VIOLET, HOT_PINK, LIGHTRED, WHITE, BLUE_HVY_PURPLE};

};
#endif
