#ifndef Pixel_h
#define Pixel_h

#include <OctoWS2811.h>
#include "colors.h"

class Pixels {
  public:
    static void pixelSet(OctoWS2811 *pixels, byte strip, byte pixel, RGBB color, float brightness);
    static void floodFill(OctoWS2811 *pixels, RGBB color, float brightness);
    
  private:
    static int physicalPixel( byte strip, byte pixel );
    const static byte LINEAR_BRIGHTNESS_TABLE[];
};

#endif
