#include <td15_midi_notes.h>
#include <Adafruit_NeoPixel.h>
#include <MIDI.h>

#define HowBigIsThisArray(x) (sizeof(x) / sizeof(x[0]))
#define NEOPIXEL_PIN    6
#define NUM_PIXELS 60
#define MIDDLE_PIXEL NUM_PIXELS/2
#define BOARD_LED 13
#define DECAY_POT_PIN A5
#define ATTACK_POT_PIN A4
#define MIDI_ON_PIN 12

#define ENERGY_FLOOR 0
#define ENERGY_FOR_SINGLE_LED 255
#define ENERGY_CEIL NUM_PIXELS * ENERGY_FOR_SINGLE_LED

#define TARGET_FPS 50
#define MILLIS_BETWEEN_FRAMES 1000/TARGET_FPS

#define DECAY_FLOOR 0
#define DECAY_CEIL 1500

#define RED_DECAY_FLOOR 100
#define RED_DECAY_CEIL 400
#define WHITE_DECAY_FLOOR 100
#define WHITE_DECAY_CEIL 800

#define SPIN_DECAY 0.0029
#define SPIN_IMPULSE 0.0075

#define SPARKLE_FACTOR_START 0.9
#define SPARKLE_DECAY 0.005

#define KICK_IMPULSE 27000
#define SNARE_IMPULSE 6000
#define TOM_IMPULSE 7500
#define CRASH_IMPULSE 15000

#define PURPLE (strip.Color(192,0,255))
#define ORANGE (strip.Color(255,70,0))
#define TEAL (strip.Color(0,255,60))
#define YELLOW (strip.Color(255,255,0))
#define WHITE (strip.Color(128,128,128))
#define BLACK (strip.Color(0,0,0))
#define RED (strip.Color(128,0,0))
#define BLUE (strip.Color(0,0,128))

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

const uint32_t colors[][9] = {
  {
    RED,
    RED,
    RED,
    PURPLE,
    PURPLE,
    PURPLE,
    YELLOW,
    YELLOW,
    YELLOW
  }
  ,
  {
    TEAL,
    TEAL,
    TEAL,
    ORANGE,
    ORANGE,
    ORANGE,
    BLUE,
    BLUE,
    BLUE
  }
};

byte numColors = HowBigIsThisArray(colors[0]);

float fEnergy = 0;
float fRedEnergy=0;
float fWhiteEnergy=0;

float fSparkleFactor=0;

float decayCache[32];
unsigned long lastPaintMillis = millis(); 
unsigned long lastDecayMillis = millis() + MILLIS_BETWEEN_FRAMES * 1 / 4;
unsigned long lastSpinMillis = millis() + MILLIS_BETWEEN_FRAMES * 2 / 4;
unsigned long lastSparkleDecayMillis = millis() + MILLIS_BETWEEN_FRAMES * 3 / 4;

float fColorCounter = 0;
float fSpinRate = 0;
float fNegativeSpinRate=0;

byte colorSet=0;
byte numColorSets = HowBigIsThisArray(colors);

MIDI_CREATE_DEFAULT_INSTANCE();

void setup(){
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.begin(MIDI_CHANNEL_OMNI); 
  pinMode(BOARD_LED, OUTPUT);
  pinMode(DECAY_POT_PIN,INPUT);
  pinMode(ATTACK_POT_PIN,INPUT);
  memset(decayCache,0,sizeof(decayCache));

  pinMode(MIDI_ON_PIN, OUTPUT);
  digitalWrite(MIDI_ON_PIN,HIGH);

  strip.begin();
  strip.show();
  testPixels();
}

void testPixels() {
  uint32_t c = strip.Color(0,255,0);
  for(uint16_t i=0; i<strip.numPixels()+4; i++) {
    strip.setPixelColor(i  , c); // Draw new pixel
    strip.setPixelColor(i-4, 0); // Erase pixel a few steps back
    strip.show();
    delay(5);
  }
}

void loop(){
  MIDI.read();
  decayEnergy();
  decaySparkle();
  repaint();
}

void decaySparkle() {
  if ( millis() - lastSparkleDecayMillis  >= MILLIS_BETWEEN_FRAMES ) {
   fSparkleFactor = fSparkleFactor - SPARKLE_DECAY; 
   if ( fSparkleFactor < 0 ) {
     fSparkleFactor = 0;
   }
   lastSparkleDecayMillis = millis();
  }
}


void repaint() {
  if ( millis() - lastPaintMillis  >= MILLIS_BETWEEN_FRAMES ) { 
    lightPixels();
    strip.show();
    lastPaintMillis = millis();  
  }
}

float applyDecay( float fEnergy, int decayPotValue, int decayFloor, int decayCeiling ) {
  fEnergy = fEnergy - computeDecay( decayPotValue/32,0,32,decayFloor,decayCeiling);  
  if ( fEnergy <  ENERGY_FLOOR  ) {
    fEnergy = ENERGY_FLOOR;
  }
  else if (fEnergy > ENERGY_CEIL ) {
    fEnergy = ENERGY_CEIL;
  }
  return fEnergy;
}

void decayEnergy() {
  if ( millis() - lastDecayMillis >= MILLIS_BETWEEN_FRAMES ) {
    fRedEnergy = applyDecay( 
      fRedEnergy,
      analogRead(DECAY_POT_PIN),
      RED_DECAY_FLOOR,
      RED_DECAY_CEIL
    );
    fWhiteEnergy = applyDecay( 
      fWhiteEnergy,
      analogRead(DECAY_POT_PIN),
      WHITE_DECAY_FLOOR,
      WHITE_DECAY_CEIL
    );
    lastDecayMillis=millis();
  }
}    

void lightPixels() {
  byte redPixelsToLight = ( fRedEnergy == 0 ) ? 0 : ( fRedEnergy / ENERGY_FOR_SINGLE_LED / 2 ) ;
  byte whitePixelsToLight = ( fWhiteEnergy == 0 ) ? 0 : ( fWhiteEnergy / ENERGY_FOR_SINGLE_LED / 2 ) ;

  for( byte p=0; p<NUM_PIXELS; p++ ) {

    byte distanceFromMiddle = abs ( p - MIDDLE_PIXEL );
    float brightnessFactor = 
      1.0 - fSparkleFactor + ( (float ) ( random( fSparkleFactor * 10 + 1) ) / 10.0 );
    byte brightness = 255 * brightnessFactor;
    byte actualBrightness = linearBrightness( brightness ) / 2;
    
    if ( distanceFromMiddle < whitePixelsToLight ) {
      strip.setPixelColor(p, WHITE);
    }
    else if ( distanceFromMiddle < redPixelsToLight ) {
      strip.setPixelColor(p,actualBrightness,0,0);
    }
    else {
      strip.setPixelColor(p,BLACK);
    }
  }
}



void handleNoteOn(byte channel, byte instrument, byte velocity){
  on();

  switch(instrument) {
  case KICK:
    fRedEnergy = fRedEnergy + map(pow(velocity,1.75),0,4804,0,KICK_IMPULSE);
    break;
  case SNARE:
    fWhiteEnergy = fWhiteEnergy + map(pow(velocity,1.75),0,4804,0,SNARE_IMPULSE);
    break;
  case CRASH1:
    resetSparkleFactor();
    break;
  case CRASH1_EDGE:
    resetSparkleFactor();
    break;
  case CRASH2:
    resetSparkleFactor();
    break;
  case CRASH2_EDGE:
    resetSparkleFactor();
    break;
  case TOM1:
    marchColors(2);
    addEnergy(velocity,TOM_IMPULSE);
    break;
  case TOM2:
    marchColors(2);
    addEnergy(velocity,TOM_IMPULSE);
    break;
  case TOM3:
    marchColors(2);
    addEnergy(velocity,TOM_IMPULSE);
    break;
  case TOM1_RIM:
    marchColors(2);
    addEnergy(velocity,TOM_IMPULSE);
    break;
  case TOM2_RIM:
    marchColors(2);
    addEnergy(velocity,TOM_IMPULSE);
    break;
  case TOM3_RIM:
    marchColors(2);
    addEnergy(velocity,TOM_IMPULSE);
    break;

  }
}

void resetSparkleFactor() {
  fSparkleFactor = SPARKLE_FACTOR_START;
}

void marchColors(int step) {
  fColorCounter = fColorCounter+step;
}

void addEnergy(byte velocity, int impulse) {
  fEnergy = fEnergy + map(pow(velocity,1.75),0,4804,0,impulse);
}

void handleNoteOff(byte channel, byte pitch, byte velocity){
  off();
}

void on(){
  digitalWrite(BOARD_LED,HIGH);
}

void off(){
  digitalWrite(BOARD_LED,LOW);
}

float computeDecay(int x, float in_min, float in_max, float out_min, float out_max){
  // note see fscale
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  if ( decayCache[x] == 0 ) {
    decayCache[x] = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
  return decayCache[x];
}

const byte LINEAR_BRIGHTNESS_TABLE[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
  0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05,
  0x05, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x0A, 0x0A, 0x0B, 0x0B,
  0x0C, 0x0C, 0x0D, 0x0D, 0x0E, 0x0F, 0x0F, 0x10, 0x11, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1F, 0x20, 0x21, 0x23, 0x24, 0x26, 0x27, 0x29, 0x2B, 0x2C,
  0x2E, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 0x3E, 0x40, 0x43, 0x45, 0x47, 0x4A, 0x4C, 0x4F,
  0x51, 0x54, 0x57, 0x59, 0x5C, 0x5F, 0x62, 0x64, 0x67, 0x6A, 0x6D, 0x70, 0x73, 0x76, 0x79, 0x7C,
  0x7F, 0x82, 0x85, 0x88, 0x8B, 0x8E, 0x91, 0x94, 0x97, 0x9A, 0x9C, 0x9F, 0xA2, 0xA5, 0xA7, 0xAA,
  0xAD, 0xAF, 0xB2, 0xB4, 0xB7, 0xB9, 0xBB, 0xBE, 0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE,
  0xD0, 0xD2, 0xD3, 0xD5, 0xD7, 0xD8, 0xDA, 0xDB, 0xDD, 0xDE, 0xDF, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5,
  0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xED, 0xEE, 0xEF, 0xEF, 0xF0, 0xF1, 0xF1, 0xF2,
  0xF2, 0xF3, 0xF3, 0xF4, 0xF4, 0xF5, 0xF5, 0xF6, 0xF6, 0xF6, 0xF7, 0xF7, 0xF7, 0xF8, 0xF8, 0xF8,
  0xF9, 0xF9, 0xF9, 0xF9, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFC,
  0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD,
  0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF};

byte linearBrightness(byte rawBrightness) {
  return LINEAR_BRIGHTNESS_TABLE[rawBrightness];
}


void spin() {

  if ( millis() - lastSpinMillis < MILLIS_BETWEEN_FRAMES ) {
    return;
  }

  fColorCounter = fColorCounter + fSpinRate - fNegativeSpinRate;

  if ( fSpinRate > 0 ) fSpinRate = fSpinRate - SPIN_DECAY;
  if ( fNegativeSpinRate > 0 ) fNegativeSpinRate = fNegativeSpinRate - SPIN_DECAY;
  if ( fSpinRate < 0 ) fSpinRate = 0;
  if ( fNegativeSpinRate < 0 ) fNegativeSpinRate = 0;

  lastSpinMillis = millis();

}






























