#ifndef Lightshow_h
#define Lightshow_h

#include <Arduino.h>
#include <FastLED.h>
#define NUM_PIXELS_PER_STRIP 60

class Lightshow
{
  public:
    Lightshow(CRGB pixels[][NUM_PIXELS_PER_STRIP],CRGB::HTMLColorCode color);
    void decay();
    void updatePixels();
    void reset();
    void handleNoteOn(byte channel, byte instrument, byte velocity);
    void handleNoteOff(byte channel, byte instrument, byte velocity);
        
  private:
    CRGB::HTMLColorCode _color;
    float _fEnergy;
    CRGB (*_pixels)[NUM_PIXELS_PER_STRIP];
};

#endif

