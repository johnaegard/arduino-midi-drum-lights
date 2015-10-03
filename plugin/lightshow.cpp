#include "lightshow.h"

#define DECAY_RATE 0.5

Lightshow::Lightshow(midi::MidiInterface<HardwareSerial> *m, CRGB pixels[][NUM_PIXELS_PER_STRIP], CRGB::HTMLColorCode color ) {
  reset();
  _pixels = pixels;
  _color = color;
  _midi = m;
}

void Lightshow::reset() {
  _fEnergy = NUM_PIXELS_PER_STRIP/2;
  _mustSweep = true;
}

void Lightshow::decay() {
  _fEnergy = _fEnergy - DECAY_RATE;
  if ( _fEnergy < 0 ) {
    _fEnergy = 0;
  }
}

void Lightshow::updatePixels() {
  byte pixelsToLight = _fEnergy;
//  if ( ! _mustSweep ) {
//    for ( s = 0; s < 5; s++ ) {
//      if ( pixelsToLight < NUM_PIXELS_PER_STRIP ) {
//        _pixels[s][pixelsToLight + 1] = CRGB::Black;
//      }
//      else if ( pixelsToLight == 0 ) {
//        _pixels[s][0] = CRGB::Black;
//      }
//      _midi->read();
//    }
//  }
//  else {
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
    _mustSweep = false;
//  }
}

void Lightshow::handleNoteOn(byte channel, byte instrument, byte velocity) {
  _fEnergy = _fEnergy + 10;
  if ( _fEnergy > NUM_PIXELS_PER_STRIP) {
    _fEnergy = NUM_PIXELS_PER_STRIP;
  }
}
void Lightshow::handleNoteOff(byte channel, byte instrument, byte velocity) {
}
