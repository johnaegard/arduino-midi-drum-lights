#include <td15_midi_notes.h>
#include <Adafruit_NeoPixel.h>
#include <MIDI.h>

#define HowBigIsThisArray(x)       (sizeof(x) / sizeof(x[0]))
#define NEOPIXEL_PIN    6
#define NUM_PIXELS 60
#define BOARD_LED 13
#define DECAY_POT_PIN A5
#define ATTACK_POT_PIN A4
#define MIDI_ON_PIN 12
#define ENERGY_FLOOR 0
#define ENERGY_FOR_SINGLE_LED 255
#define ENERGY_CEIL NUM_PIXELS * ENERGY_FOR_SINGLE_LED

#define CRASH_BOOST 15000
#define DECAY_FLOOR 0
#define DECAY_CEIL 1300
#define TARGET_FPS 30
#define MILLIS_BETWEEN_FRAMES 1000/TARGET_FPS

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

const uint32_t colors[][2] = { 
  { 
    strip.Color(255,0,0), strip.Color(255,255,255)       }
  ,
  { 
    strip.Color(0,255,0), strip.Color(255,255,0)       }
  ,
  { 
    strip.Color(65,0,65), strip.Color(255,75,0)       }
};

float fEnergy = 0;
float decayCache[32];
byte colorCounter = 0;
unsigned long lastPaintMillis = millis();
unsigned long lastDecayMillis = millis();

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
  lightPixels();
  paint();
}

void paint() {
  if ( millis() - lastPaintMillis  >= MILLIS_BETWEEN_FRAMES ) { 
    strip.show();
    lastPaintMillis = millis();  
  }
}

void decayEnergy() {
  if ( millis() - lastDecayMillis  >= MILLIS_BETWEEN_FRAMES ) {
    float decay = computeDecay( analogRead(DECAY_POT_PIN)/32,0,32,DECAY_FLOOR,DECAY_CEIL);
    fEnergy = fEnergy - decay;
    if ( fEnergy <  ENERGY_FLOOR  ) {
      fEnergy = ENERGY_FLOOR;
    }
    else if (fEnergy > ENERGY_CEIL ) {
      fEnergy = ENERGY_CEIL;
    }
    lastDecayMillis=millis();
  }
}

void lightPixels() {
  byte pixelsToLight = ( fEnergy == 0 ) ? 0 : ( fEnergy / ENERGY_FOR_SINGLE_LED  ) ;

  for(byte p=0;p<pixelsToLight;p++) {
    if (  pixelsToLight - p > 4 )  {
      strip.setPixelColor(p,colors[colorCounter][0]);
    }
    else {
      strip.setPixelColor(p,colors[colorCounter][1]);
    }
  }

  byte finalPixel = pixelsToLight;
  byte finalPixelBrightness = linearBrightness(( (int) fEnergy) % ENERGY_FOR_SINGLE_LED);
  strip.setPixelColor(finalPixel,finalPixelBrightness,finalPixelBrightness,finalPixelBrightness);

  for(byte p=pixelsToLight+1;p<NUM_PIXELS;p++) {
    strip.setPixelColor(p,0,0,0);
  }
}

void handleNoteOn(byte channel, byte instrument, byte velocity){
  on();

  long energy = velocity * analogRead(ATTACK_POT_PIN);


  switch(instrument) {
  case KICK:
    fEnergy = fEnergy + map(pow(velocity,1.75),0,4804,0,CRASH_BOOST);
    break;
  case SNARE:
    colorCounter = ( colorCounter + 1 ) % HowBigIsThisArray(colors); 
    break;
  }
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







