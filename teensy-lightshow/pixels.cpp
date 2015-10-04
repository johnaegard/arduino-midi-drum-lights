#include "pixels.h"
#include "constants.h"

void Pixels::pixelSet(OctoWS2811 *pixels, byte strip, byte pixel, int color) {
  // virtual strips 0..2 are on physical strips 0..2 (pixels 0..179)
  // virtual strips 3..4 are on physical strips 4..5 (pixels 240..359)
  int physicalPixel = ( strip < 3 ) ?
                      strip * PIXELS_PER_STRIP + pixel :
                      ( strip + 1 ) * PIXELS_PER_STRIP + pixel ;
  pixels->setPixel(physicalPixel, color);
}

void Pixels::floodFill(OctoWS2811 *pixels, int color) {
  for (byte p = 0; p < PIXELS_PER_STRIP; p++) {
    for (byte s = 0; s < 5; s++) {
      pixelSet(pixels, s, p, color);
    }
  }
}

