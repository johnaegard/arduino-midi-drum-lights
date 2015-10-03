#include <Arduino.h>
#include <FastLED.h>
#include "lightshow.h"

#define DECAY_RATE 0.5

Lightshow::Lightshow(CRGB pixels[][NUM_PIXELS_PER_STRIP], CRGB::HTMLColorCode color) {
  reset();
  _pixels = pixels;
  _color = color;
}

void Lightshow::reset() {
  _fEnergy = 30;
}

void Lightshow::decay() {
  _fEnergy = _fEnergy - DECAY_RATE;
}

void Lightshow::updatePixels() {
  byte pixelsToLight = ( _fEnergy < 0 ) ? 0 : _fEnergy;
  for ( byte s = 0; s < 5; s++ ) {
    for ( byte p = 0; p < NUM_PIXELS_PER_STRIP; p++ ) {
      if ( p < pixelsToLight ) {
        _pixels[s][p] = _color;
      }
      else {
        _pixels[s][p] = CRGB::Black;
      }
    }
  }
}

void Lightshow::handleNoteOn(byte channel, byte instrument, byte velocity) {
}
void Lightshow::handleNoteOff(byte channel, byte instrument, byte velocity) {
}
