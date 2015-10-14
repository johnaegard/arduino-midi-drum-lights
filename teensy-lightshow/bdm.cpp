#include "bdm.h"

BDMLightshow::BDMLightshow(OctoWS2811 *p) {
  _pixels = p;
}

void BDMLightshow::reset() {
  Pixels::floodFill(_pixels, BLACK, 0);
  for ( byte s = 0; s < NUM_STARS; s++) {
    resetStar(s);
    _stars[s].strip = s % NUM_STRIPS;
  }
}

void BDMLightshow::resetStar(byte s) {
  _stars[s].strip = random(0, NUM_STRIPS);
  _stars[s].pixel = random(0, FIRST_PIXEL_CEIL);
  _stars[s].color = bdmPalette[random(0, 7)];
//  _stars[s].velocity = 0.02f;
  _stars[s].velocity = ( (float) random(1, VELOCITY_CEIL * 100000) / ( VELOCITY_CEIL * 100000.0f )) ;
}

void BDMLightshow::decay() {
  for ( byte s = 0; s < NUM_STARS; s++) {
    _stars[s].pixel = _stars[s].pixel + _stars[s].velocity;
    if ( _stars[s].pixel > PIXELS_PER_STRIP ) {
      resetStar(s);
    }
  }
}

void BDMLightshow::updatePixels() {
  if ( millis() - _previousTwinkleMillis > MILLIS_BETWEEN_TWINKLES ) {
    for ( byte s = 0; s < NUM_STARS; s++) {
      if ( (_stars[s].oldPixel != (byte)(_stars[s].pixel) ) || (_stars[s].oldStrip != _stars[s].strip ))  {
        Pixels::pixelSet(_pixels, _stars[s].oldStrip, _stars[s].oldPixel, BLACK, 0);
        _stars[s].oldPixel = (byte)(_stars[s].pixel);
        _stars[s].oldStrip = (byte)(_stars[s].strip);
      }
      float twinkleBrightness = ( (float) random( TWINKLE_FLOOR*100, 101) ) / 100.0f;
      Pixels::pixelSet(_pixels, _stars[s].strip, (byte)(_stars[s].pixel), _stars[s].color, twinkleBrightness);
    }
    _previousTwinkleMillis = millis();
  }
}

void BDMLightshow::handleNoteOn(byte channel, byte instrument, byte velocity) {

  switch (instrument) {
    case KICK:
      break;
    case SNARE:
      reset();
      break;
    case SNARE_RIM:
      reset();
      break;
    case XSTICK:
      reset();
      break;
  }
}

void BDMLightshow::handleNoteOff(byte channel, byte instrument, byte velocity) {}
