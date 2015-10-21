#include "creep.h"

CreepLightshow::CreepLightshow( OctoWS2811 *p) {
  _pixels = p;
}

void CreepLightshow::reset() {
  _paletteIndex = VERSE_PALETTE_INDEX;
  _numSnares = 0;
  isStarted = false;
  _chorusMode = false;
  Pixels::floodFill(_pixels, BLACK, 0.0);
}

void CreepLightshow::decay() {
  _chorusBrightness = constrain(_chorusBrightness - CHORUS_DECAY, CHORUS_BRIGHTNESS_FLOOR, CHORUS_BRIGHTNESS_CEIL);
  _verseBrightness = constrain(_verseBrightness - VERSE_DECAY, VERSE_BRIGHTNESS_FLOOR, VERSE_BRIGHTNESS_CEIL);
}

void CreepLightshow::updatePixels() {
  if ( _chorusMode ) {
    _paletteIndex = CHORUS_PALETTE_INDEX;
    Pixels::floodFill(_pixels, palette[_paletteIndex][_colorIndex], _chorusBrightness);
  } else {
    _paletteIndex = VERSE_PALETTE_INDEX;
    Pixels::fillStrip(_pixels, 0, palette[_paletteIndex][_colorIndex], _verseBrightness * 0.8);
    Pixels::fillStrip(_pixels, 4, palette[_paletteIndex][_colorIndex], _verseBrightness * 0.8);
    Pixels::fillStrip(_pixels, 1, palette[_paletteIndex][(_colorIndex + 1 ) % 4], _verseBrightness *0.9);
    Pixels::fillStrip(_pixels, 3, palette[_paletteIndex][(_colorIndex + 1) % 4], _verseBrightness * 0.9);
    Pixels::fillStrip(_pixels, 2, palette[_paletteIndex][(_colorIndex + 2) % 4], _verseBrightness );
  }
}

void CreepLightshow::handleNoteOn(byte channel, byte instrument, byte velocity) {
  isStarted = true;

  switch (instrument) {
    case KICK:
      break;
    case SNARE:
      snare();
      break;
    case SNARE_RIM:
      snare();
      break;
    case XSTICK:
      snare();
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
      hihat();
      break;
    case HIHAT_OPEN_RIM:
      hihat();
      break;
    case HIHAT_CLOSED:
      hihat();
      break;
    case HIHAT_CLOSED_RIM:
      hihat();
      break;
    case HIHAT_PEDAL:
      break;
  }
}

void CreepLightshow::handleNoteOff(byte channel, byte instrument, byte velocity) {}

void CreepLightshow::crash() {
  _verseBrightness = VERSE_BRIGHTNESS_CEIL;
  _chorusBrightness = CHORUS_BRIGHTNESS_CEIL;
}

void CreepLightshow::ride() {
  _chorusBrightness = CHORUS_BRIGHTNESS_CEIL;
  _chorusMode = true;
}

void CreepLightshow::tom() {
  _verseBrightness = VERSE_BRIGHTNESS_CEIL             ;
}

void CreepLightshow::hihat() {
  _chorusMode = false;
}

void CreepLightshow::snare() {
  _colorIndex = _numSnares++ % ( HowBigIsThisArray(palette[0]) ) ;
}
