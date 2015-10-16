#include "bdm.h"

BDMLightshow::BDMLightshow(OctoWS2811 *p) {
  _pixels = p;
}

void BDMLightshow::reset() {
  for ( byte d = 0; d < NUM_DROPS; d++) {
    resetDrop(d);
    _drops[d].strip = d % NUM_STRIPS;
  }
  for ( byte s = 0; s < NUM_STARS; s++) {
    resetStar(s);
  }
}

void BDMLightshow::resetStar(byte s) {
  _stars[s].strip = s % NUM_STRIPS;
  _stars[s].pixel = random(STAR_FLOOR, STAR_CEILING + 1);
  _stars[s].color = BLUEWHITE;
}

void BDMLightshow::resetDrop(byte d) {
  _drops[d].strip = random(0, NUM_STRIPS);
  _drops[d].pixel = 0;
  _drops[d].color = bdmPalette[random(0, HowBigIsThisArray(bdmPalette))];
  _drops[d].velocity =  DROP_VELOCITY_FLOOR + ( ( (float) ( random(0, 100) ) / 100.0f ) * DROP_VELOCITY_RANGE ) ;
}

void BDMLightshow::decay() {
  for ( byte d = 0; d < NUM_DROPS; d++) {
    _drops[d].pixel = _drops[d].pixel + _drops[d].velocity;
    if ( _drops[d].pixel > PIXELS_PER_STRIP ) {
      resetDrop(d);
    }
    _drops[d].velocity = constrain(_drops[d].velocity + DROP_VELOCITY_ACCEL, DROP_VELOCITY_FLOOR, DROP_VELOCITY_FLOOR + DROP_VELOCITY_RANGE);
  }
}

void BDMLightshow::updatePixels() {

  boolean twinkle = ( millis() - _previousTwinkleMillis > MILLIS_BETWEEN_TWINKLES );

  for ( byte s = 0; s < NUM_STARS; s++) {
    if ( twinkle)  {
      _stars[s].twinkleBrightness = ( (float) random( STAR_TWINKLE_FLOOR * 100, 101) ) / 100.0f;
    }
    lightStarPixel(s);
  }

  for ( byte d = 0; d < NUM_DROPS; d++) {
    if ( _drops[d].hasMoved() ) {
      blackoutOldPixels(d);
      _drops[d].updateContrails();
      _drops[d].updateHistory();
    }
    if ( twinkle)  {
      _drops[d].twinkleBrightness = ( (float) random( DROP_TWINKLE_FLOOR * 100, 101) ) / 100.0f;
    }
    lightDropPixels(d);
  }
  if ( twinkle) {
    _previousTwinkleMillis = millis();
  }

}

//
// CONTRAILS
//
void BDMLightshow::blackoutOldPixels(byte d) {
  Pixels::pixelSet(_pixels, _drops[d].oldStrip, _drops[d].oldPixel, BLACK, 0);
  if ( _drops[d].contrail1OldPixel >= 0 ) {
    Pixels::pixelSet(_pixels, _drops[d].oldStrip, _drops[d].contrail1OldPixel, BLACK, 0);
  }
  if ( _drops[d].contrail2OldPixel >= 0 ) {
    Pixels::pixelSet(_pixels, _drops[d].oldStrip, _drops[d].contrail2OldPixel, BLACK, 0);
  }
  if ( _drops[d].contrail3OldPixel >= 0 ) {
    Pixels::pixelSet(_pixels, _drops[d].oldStrip, _drops[d].contrail3OldPixel, BLACK, 0);
  }
}

void BDMLightshow::lightDropPixels(byte d) {
  Pixels::pixelSet(_pixels, _drops[d].strip, _drops[d].wholePixel(), _drops[d].color, _drops[d].twinkleBrightness);
  if ( _drops[d].contrail1Pixel >= 0 ) {
    Pixels::pixelSet(_pixels, _drops[d].strip, _drops[d].contrail1Pixel, _drops[d].color, CONTRAIL_BRIGHTNESS_1 * _drops[d].twinkleBrightness);
  }
  if ( _drops[d].contrail2Pixel >= 0 ) {
    Pixels::pixelSet(_pixels, _drops[d].strip, _drops[d].contrail2Pixel, _drops[d].color, CONTRAIL_BRIGHTNESS_2 * _drops[d].twinkleBrightness);
  }
  if ( _drops[d].contrail3Pixel >= 0 ) {
    Pixels::pixelSet(_pixels, _drops[d].strip, _drops[d].contrail3Pixel, _drops[d].color, CONTRAIL_BRIGHTNESS_3 * _drops[d].twinkleBrightness);
  }
}

void BDMLightshow::lightStarPixel(byte s) {
  Pixels::pixelSet(_pixels, _stars[s].strip, _stars[s].pixel, _stars[s].color, _stars[s].twinkleBrightness);
}

//
// MIDI
//
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

void BDMLightshow::debugAllDrops() {
  for ( byte d = 0; d < NUM_DROPS; d++) {
    Serial.println(_drops[d].toString());
  }
}
