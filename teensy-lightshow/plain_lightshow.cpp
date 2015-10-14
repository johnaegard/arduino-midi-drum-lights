#include "plain_lightshow.h"

#define DECAY_RATE 0.2
#define ENERGY_CEIL PIXELS_PER_STRIP/2

PlainLightshow::PlainLightshow( OctoWS2811 *p, RGBB color) {
  _pixels = p;
  _color = color;
  reset();
}

void PlainLightshow::reset() {
  _fEnergy = ENERGY_CEIL;
}

void PlainLightshow::decay() {
  _fEnergy = constrain(_fEnergy - DECAY_RATE, 0, ENERGY_CEIL);
}

void PlainLightshow::updatePixels() {
  byte pixelsToLight = _fEnergy;
  for ( s = 0; s < 5; s++ ) {
    for ( p = 0; p < PIXELS_PER_STRIP; p++ ) {
      if ( p < pixelsToLight ) {
        Pixels::pixelSet(_pixels,s,p,_color,1.0);
      }
      else {
        Pixels::pixelSet(_pixels,s,p,BLACK,0);
      }
    }
  }
}

void PlainLightshow::handleNoteOn(byte channel, byte instrument, byte velocity) {
  _fEnergy = constrain( _fEnergy + 10, 0, ENERGY_CEIL );
}

void PlainLightshow::handleNoteOff(byte channel, byte instrument, byte velocity) {
}
