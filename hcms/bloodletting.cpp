#include "bloodletting.h"

BloodlettingLightshow::BloodlettingLightshow(OctoWS2811 *p) {
  _pixels = p;
}

//
// BOILERPLATE
//
void BloodlettingLightshow::reset() {
  _fSparkle = 0;
  _currentRedStrip = 0;
  _currentWhiteStrip = 0;
  verseColors();
  isStarted = false;
  Pixels::floodFill(_pixels, BLACK, 1.0);
}

void BloodlettingLightshow::decay() {
  for (byte s = 0; s < 5; s++) {
    applyDecay(_fRedEnergy[s], RED_DECAY, ENERGY_FLOOR, RED_CEIL);
    applyDecay(_fWhiteEnergy[s], WHITE_DECAY, ENERGY_FLOOR, WHITE_CEIL);
  }
  decaySparkle();
}

void BloodlettingLightshow::updatePixels() {
  byte redPixelsToLight;
  byte whitePixelsToLight;

  for (byte s = 0; s < 5; s++) {
    redPixelsToLight =  _fRedEnergy[s];
    whitePixelsToLight = _fWhiteEnergy[s];
    for (byte p = 0; p < PIXELS_PER_STRIP; p++) {

      byte distanceFromMiddle = abs ( p - MIDDLE_PIXEL );

      float sparkleBrightnessFactor =
        1.0 - _fSparkle + ( (float ) ( random( _fSparkle * 10 + 1) ) / 10.0 );

      if (whitePixelsToLight - distanceFromMiddle == 1 ) {
        Pixels::pixelSet(_pixels, s, p, _snareEdgeColor, sparkleBrightnessFactor);
      }
      else if ( distanceFromMiddle < whitePixelsToLight ) {
        Pixels::pixelSet(_pixels, s, p, _snareColor, sparkleBrightnessFactor);
      }
      else if ( distanceFromMiddle < redPixelsToLight ) {
        Pixels::pixelSet(_pixels, s, p, _kickColor, sparkleBrightnessFactor);
      }
      else {
        Pixels::pixelSet(_pixels, s, p, BLACK, 0);
      }
    }
  }
}

//
// SPARKLE
//
void BloodlettingLightshow::resetSparkle() {
  _fSparkle = SPARKLE_FACTOR_START;
}

void BloodlettingLightshow::decaySparkle() {
  _fSparkle = constrain( _fSparkle - SPARKLE_DECAY, SPARKLE_FLOOR, SPARKLE_CEIL);
}

//
// PALETTE CHANGE
//
void BloodlettingLightshow::chorusColors() {
  _snareColor = BLUE;
  _snareEdgeColor = WHITE;
  _kickColor = HALFGREEN;
}

void BloodlettingLightshow::verseColors() {
  _snareColor = YELLOW;
  _snareEdgeColor = WHITE;
  _kickColor = HALFRED;
}

//
// MIDI CRAP
//
void BloodlettingLightshow::handleNoteOn(byte channel, byte instrument, byte velocity) {

  switch (instrument) {
    case KICK:
      _previousRedStrip = _currentRedStrip;
      _currentRedStrip = ( _currentRedStrip + 1 ) % 5;
      if ( millis() - _previousKickDrumMillis < KICK_DRUM_AGGREGATE_MILLIS ) {
        _fRedEnergy[_currentRedStrip] = _fRedEnergy[_previousRedStrip];
        _previousKickDrumMillis = millis();
      }
      _fRedEnergy[_currentRedStrip] =
        constrain( _fRedEnergy[_currentRedStrip] + map(pow(velocity, 1.75), 0, 4804, 0, KICK_IMPULSE), ENERGY_FLOOR, RED_CEIL);

      break;
    case SNARE:
      _fWhiteEnergy[2] =
        constrain( _fWhiteEnergy[2] + map(pow(velocity, 1.75), 0, 4804, 0, SNARE_IMPULSE), ENERGY_FLOOR, WHITE_CEIL);
      _fWhiteEnergy[1] =
        constrain( _fWhiteEnergy[1] + map(pow(velocity, 1.75), 0, 4804, 0, SNARE_IMPULSE), ENERGY_FLOOR, WHITE_CEIL / 3);
      _fWhiteEnergy[3] =
        constrain( _fWhiteEnergy[3] + map(pow(velocity, 1.75), 0, 4804, 0, SNARE_IMPULSE), ENERGY_FLOOR, WHITE_CEIL / 3);
      _fWhiteEnergy[4] =
        constrain( _fWhiteEnergy[4] + map(pow(velocity, 1.75), 0, 4804, 0, SNARE_IMPULSE), ENERGY_FLOOR, WHITE_CEIL / 5);
      _fWhiteEnergy[0] =
        constrain( _fWhiteEnergy[0] + map(pow(velocity, 1.75), 0, 4804, 0, SNARE_IMPULSE), ENERGY_FLOOR, WHITE_CEIL / 5);
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
      chorusColors();
      break;
    case RIDE_EDGE:
      chorusColors();
      break;
    case RIDE_BELL:
      chorusColors();
      break;
    case HIHAT_OPEN:
      verseColors();
      break;
    case HIHAT_OPEN_RIM:
      verseColors();
      break;
    case HIHAT_CLOSED:
      verseColors();
      break;
    case HIHAT_CLOSED_RIM:
      verseColors();
      break;
    case HIHAT_PEDAL:
      verseColors();
      break;

  }

}

void BloodlettingLightshow::crash() {
  isStarted = true;
  resetSparkle();
}

void BloodlettingLightshow::handleNoteOff(byte channel, byte instrument, byte velocity) {}
