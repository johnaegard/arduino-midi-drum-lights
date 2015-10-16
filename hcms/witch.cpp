#include "witch.h"

//
// BOILERPLATE
//

WitchLightshow::WitchLightshow( OctoWS2811 *p) {
  _pixels = p;
}

void WitchLightshow::reset() {
  _currentPaletteIndex = 0;
  _numHiHats = 0;
  _numRides = 0;
}

void WitchLightshow::updatePixels() {
  for ( byte s = 0; s < NUM_STRIPS; s++) {
    for (byte b = 0; b < NUM_BLOCKS; b++) {
      RGBB color = BLACK;
      if ( ( ! _soloColorMode ) || ( _soloColorMode && _blocks[s][b].color == _soloColor ) ) {
        color = _blocks[s][b].color;
      }
      byte first_pixel = b * BLOCK_SIZE;
      byte last_pixel = first_pixel + BLOCK_SIZE - 1 ;
      for (byte p = first_pixel; p < last_pixel; p++) {
        Pixels::pixelSet(_pixels, s, p, color, 1.0);
      }
    }
  }
}

void WitchLightshow::decay() {}

//
// COLORS
//
byte WitchLightshow::randomColorIndex() {
  return random(0, HowBigIsThisArray(palette[_currentPaletteIndex]));
}

RGBB WitchLightshow::randomColor() {
  return palette[_currentPaletteIndex][randomColorIndex()];
}

void WitchLightshow::scrambleColors() {
  for ( byte s = 0; s < NUM_STRIPS; s++) {
    for (byte b = 0; b < NUM_BLOCKS; b++) {
      RGBB color = randomColor();
      while ( _blocks[s][b].color == color ) {
        color = randomColor();
      }
      _blocks[s][b].color = color;
    }
  }
}

//
// MIDI
//
void WitchLightshow::handleNoteOn(byte channel, byte instrument, byte velocity) {
  switch (instrument) {
    case KICK:
      break;
    case SNARE:
      break;
    case CRASH1:
      crash();
      break;
    case CRASH1_EDGE:
      crash();
      break;
    case CRASH2:
      crash();
      break;
    case CRASH2_EDGE:
      crash();
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
      ride();
      break;
    case RIDE_EDGE:
      ride();
      break;
    case RIDE_BELL:
      ride();
      break;
    case HIHAT_OPEN:
      hiHat();
      break;
    case HIHAT_OPEN_RIM:
      hiHat();
      break;
    case HIHAT_CLOSED:
      hiHat();
      break;
    case HIHAT_CLOSED_RIM:
      hiHat();
      break;
    case HIHAT_PEDAL:
      break;
  }
}

void WitchLightshow::crash() {
  _numHiHats = 0;
}

void WitchLightshow::hiHat() {
  if ( _numHiHats++ % 4 == 0 ) {
    _currentPaletteIndex = ( _currentPaletteIndex + 1 ) % 2;
  }
  scrambleColors();
  _soloColorMode = false;
}

void WitchLightshow::ride() {
  byte soloColorIndex = _numRides++ % ( HowBigIsThisArray(palette[0]) ) ;
  _soloColor = palette[_currentPaletteIndex][soloColorIndex];
  _soloColorMode = true;
}

void WitchLightshow::handleNoteOff(byte channel, byte instrument, byte velocity) {}
