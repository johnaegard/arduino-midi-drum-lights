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
  _crashDarkMode = false;
  _crashDarkModeBlastHappened = false;
  Pixels::floodFill(_pixels, BLACK, 1.0);
  isStarted = false;
}

void WitchLightshow::updatePixels() {
  if (_crashDarkMode == true) {
    if ( ! _crashDarkModeBlastHappened) {
      Pixels::floodFill(_pixels, randomColor(), 1.0);
      _crashDarkModeBlastHappened = true;
    }
  }
  else {
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
        Pixels::pixelSet(_pixels, s, last_pixel, BLACK, 0);
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
      tom();
      break;
    case TOM2:
      tom();
      break;
    case TOM3:
      tom();
      break;
    case TOM1_RIM:
      tom();
      break;
    case TOM2_RIM:
      tom();
      break;
    case TOM3_RIM:
      tom();
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

void WitchLightshow::tom() {
  _crashDarkMode = true;
  _crashDarkModeBlastHappened = false;
}

void WitchLightshow::crash() {
  isStarted = true;
  _numHiHats = 0;
  _crashDarkMode = true;
  _crashDarkModeBlastHappened = false;
}

void WitchLightshow::hiHat() {
  isStarted = true;
  if ( _numHiHats++ % 4 == 0 ) {
    _currentPaletteIndex = ( _currentPaletteIndex + 1 ) % 2;
  }
  scrambleColors();
  _soloColorMode = false;
  _crashDarkMode = false;
  _crashDarkModeBlastHappened = false;
}

void WitchLightshow::ride() {
  isStarted = true;
  byte soloColorIndex = _numRides++ % ( HowBigIsThisArray(palette[0]) ) ;
  _soloColor = palette[_currentPaletteIndex][soloColorIndex];
  _soloColorMode = true;
  _crashDarkMode = false;
  _crashDarkModeBlastHappened = false;
}

void WitchLightshow::handleNoteOff(byte channel, byte instrument, byte velocity) {}
