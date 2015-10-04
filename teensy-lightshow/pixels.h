#ifndef Pixel_h
#define Pixel_h

#include <OctoWS2811.h>



class Pixels {
  public:
    static void pixelSet(OctoWS2811 *pixels, byte strip, byte pixel, int color);
    static void floodFill(OctoWS2811 *pixels, int color);
};
#endif
