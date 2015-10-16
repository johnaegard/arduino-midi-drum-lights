#include "plain.h"

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
  for (byte s = 0; s < 5; s++ ) {
    for (byte p = 0; p < PIXELS_PER_STRIP; p++ ) {
      if ( p < pixelsToLight ) {
        Pixels::pixelSet(_pixels, s, p, _color, 1.0);
      }
      else {
        Pixels::pixelSet(_pixels, s, p, BLACK, 0);
      }
    }
  }
}

void PlainLightshow::handleNoteOn(byte channel, byte instrument, byte velocity) {
  _fEnergy = constrain( _fEnergy + 10, 0, ENERGY_CEIL );
  switch (instrument) {
    case KICK:
      break;
    case SNARE:
      break;
    case CRASH1:
      break;
    case CRASH1_EDGE:
      break;
    case CRASH2:
      break;
    case CRASH2_EDGE:
      break;
    case TOM1:
      break;
    case TOM2:
      break;
    case TOM3:
      break;
    case TOM1_RIM:
      break;
    case TOM2_RIM:
      break;
    case TOM3_RIM:
      break;
    case RIDE:
      break;
    case RIDE_EDGE:
      break;
    case RIDE_BELL:
      break;
    case HIHAT_OPEN:
      break;
    case HIHAT_OPEN_RIM:
      break;
    case HIHAT_CLOSED:
      break;
    case HIHAT_CLOSED_RIM:
      break;
    case HIHAT_PEDAL:
      break;
  }
}

void PlainLightshow::handleNoteOff(byte channel, byte instrument, byte velocity) {
}
