#include "loose.h"

LooseLightshow::LooseLightshow( OctoWS2811 *p) {
  _pixels = p;
}
void LooseLightshow::reset() {
  _largePaletteIndex = 0;
  _smallPaletteIndex = 0;
  _kickCount = 0;
  _snareCount = 0;
  _rideCount = 0;
  _spinRate = 0;
  _spinPixelOffset = 0;

  isStarted = false;
  Pixels::floodFill(_pixels, BLACK, 0.0);
}

void LooseLightshow::decay() {
  _spinRate = constrain(_spinRate - SPIN_DECAY, 0, SPIN_CEIL );
}

RGBB LooseLightshow::colorNow(byte strip, byte block) {
  if ( ( strip + block + _snareCount ) % 2 == 0 ) {
    return smallPalette[ _kickCount % HowBigIsThisArray(smallPalette) ];
  }
  else {
    return largePalette[ _kickCount % HowBigIsThisArray(largePalette) ];
  }
}

void LooseLightshow::updatePixels() {
  if (_spinRate > 0 ) {
    Serial.print("spinrate=");
    Serial.println(_spinRate, 4);
    Serial.print("spinPixelOffset=");
    Serial.println(_spinPixelOffset, 4);
  }
  _spinPixelOffset = ( _spinPixelOffset + _spinRate );
  for ( byte s = 0; s < NUM_STRIPS; s++) {
    int spinDirection = ( s % 2 == 0 ) ? 1 : -1;
    for (byte b = 0; b < NUM_LOOSE_BLOCKS; b++) {
      RGBB color = colorNow(s, b);
      int pixelOffset = ( (int) ( _spinPixelOffset + _rideCount )  * spinDirection ) % ( LOOSE_BLOCK_SIZE * 2 -1 ) ;
      int first_pixel = ( b * LOOSE_BLOCK_SIZE ) + ( pixelOffset );
      int last_pixel = first_pixel + LOOSE_BLOCK_SIZE;
      if (( s == 1 ) && (_spinRate > 0)) {
        Serial.print(s, DEC);
        Serial.print(",");
        Serial.print(b, DEC);
        Serial.print(",");
        Serial.print(pixelOffset, DEC);
        Serial.print(",");
        Serial.print(first_pixel, DEC);
        Serial.print(",");
        Serial.print(last_pixel, DEC);
        Serial.println();
      }
      for (int p = first_pixel; p < last_pixel; p++) {
        int pixel = p % PIXELS_PER_STRIP;
        pixel = ( pixel < 0 ) ? pixel + PIXELS_PER_STRIP : pixel;
        if (( s == 1 ) && (_spinRate > 0)) {
          Serial.print(pixel, DEC);
          Serial.print(",");
        }
        Pixels::pixelSet(_pixels, s, pixel, color, 1.0);
      }
if (( s == 1 ) && (_spinRate > 0)) {
      Serial.println();
}
    }
  }
}

void LooseLightshow::handleNoteOn(byte channel, byte instrument, byte velocity) {
  isStarted = true;
  switch (instrument) {
    case KICK:
      kick();
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

void LooseLightshow::handleNoteOff(byte channel, byte instrument, byte velocity) {

}

void LooseLightshow::crash() {
  _spinRate = SPIN_CEIL;
}

void LooseLightshow::hihat() {
}

void LooseLightshow::ride() {
  _rideCount++;
}

void LooseLightshow::kick() {
  _kickCount++;
}

void LooseLightshow::tom() {
  _snareCount++;
}
void LooseLightshow::snare() {
  _snareCount++;
}
