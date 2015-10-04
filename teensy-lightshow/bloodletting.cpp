#include "bloodletting.h"

BloodlettingLightshow::BloodlettingLightshow(OctoWS2811 *p) {
  _pixels = p;
  reset();
}

void BloodlettingLightshow::reset() {
  _fSparkle = 0;
  _currentRedStrip = 0;
  _currentWhiteStrip = 0;
}

void BloodlettingLightshow::decay() {
  for (s = 0; s < 5; s++) {
    applyDecay(_fRedEnergy[s], RED_DECAY_FLOOR, RED_DECAY_CEIL, ENERGY_FLOOR, RED_CEIL);
    applyDecay(_fWhiteEnergy[s], WHITE_DECAY_FLOOR, WHITE_DECAY_CEIL, ENERGY_FLOOR, WHITE_CEIL);
  }
  decaySparkle();
}

//
//SPARKLE!
//

void BloodlettingLightshow::resetSparkle() {}

void BloodlettingLightshow::decaySparkle() {
  _fSparkle = constrain( _fSparkle - SPARKLE_DECAY, SPARKLE_FLOOR, SPARKLE_CEIL);
}

void BloodlettingLightshow::updatePixels() {
  byte redPixelsToLight;
  byte whitePixelsToLight;

  for (s = 0; s < 5; s++) {
    redPixelsToLight = ( _fRedEnergy[s] /  2 ) ;
    whitePixelsToLight = _fWhiteEnergy[s] / ( abs(s - _currentRedStrip + 2)  + 1 );
    for (p = 0; p < PIXELS_PER_STRIP; p++) {
      byte distanceFromMiddle = abs ( p - MIDDLE_PIXEL );
      if ( distanceFromMiddle < whitePixelsToLight ) {
        Pixels::pixelSet(_pixels, s, p, WHITE);
      }
      else if ( distanceFromMiddle < redPixelsToLight ) {
        Pixels::pixelSet(_pixels, s, p, RED);
      }
      else {
        Pixels::pixelSet(_pixels, s, p, BLACK);
      }
    }
  }
}

void BloodlettingLightshow::handleNoteOn(byte channel, byte instrument, byte velocity) {
  switch (instrument) {
    case KICK:
      _currentRedStrip = ( _currentRedStrip + 1 ) % 5;
      _fRedEnergy[_currentRedStrip] =
        constrain( _fRedEnergy[_currentRedStrip] + map(pow(velocity, 1.75), 0, 4804, 0, KICK_IMPULSE), ENERGY_FLOOR, RED_CEIL);
      break;
    case SNARE:
      _currentWhiteStrip = ( _currentWhiteStrip == 0 ) ? 4 : _currentWhiteStrip - 1;
      _fWhiteEnergy[_currentWhiteStrip] =
        constrain( _fWhiteEnergy[_currentWhiteStrip] + map(pow(velocity, 1.75), 0, 4804, 0, SNARE_IMPULSE), ENERGY_FLOOR, WHITE_CEIL);
      break;
    case CRASH1:
      resetSparkle();
      break;
    case CRASH1_EDGE:
      resetSparkle();
      break;
    case CRASH2:
      resetSparkle();
      break;
    case CRASH2_EDGE:
      resetSparkle();
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
  }
}
void BloodlettingLightshow::handleNoteOff(byte channel, byte instrument, byte velocity) {}
