#include "plain_lightshow.h"

#define DECAY_RATE 1

PlainLightshow::PlainLightshow(
  midi::MidiInterface<HardwareSerial> *m,
  CRGB pixels[][NUM_PIXELS_PER_STRIP],
  CRGB::HTMLColorCode color
) {
  reset();
  _pixels = pixels;
  _color = color;
  _midi = m;
}

void PlainLightshow::reset() {
  _fEnergy = NUM_PIXELS_PER_STRIP / 2;
}

void PlainLightshow::decay() {
  _fEnergy = _fEnergy - DECAY_RATE;
  if ( _fEnergy < 0 ) {
    _fEnergy = 0;
  }
}

void PlainLightshow::updatePixels() {
  byte pixelsToLight = _fEnergy;
  for ( s = 0; s < 5; s++ ) {
    for ( p = 0; p < NUM_PIXELS_PER_STRIP; p++ ) {
      if ( p < pixelsToLight ) {
        _pixels[s][p] = _color;
      }
      else {
        _pixels[s][p] = CRGB::Black;
      }
    }
    _midi->read();
  }
}

void PlainLightshow::handleNoteOn(byte channel, byte instrument, byte velocity) {
  _fEnergy = _fEnergy + 10;
  if ( _fEnergy > NUM_PIXELS_PER_STRIP) {
    _fEnergy = NUM_PIXELS_PER_STRIP;
  }
}

void PlainLightshow::handleNoteOff(byte channel, byte instrument, byte velocity) {
}
