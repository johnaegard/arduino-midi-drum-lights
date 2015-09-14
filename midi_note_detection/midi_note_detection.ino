#include <MIDI.h>
#include <limits.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define LED 13                   // LED pin on Arduino Uno

#define KICK 36
#define SNARE 38
#define SNARE_RIM 40
#define XSTICK 37
#define TOM1 48
#define TOM1_RIM 50
#define TOM2 45
#define TOM2_RIM 47
#define TOM3 43
#define TOM3_RIM 58
#define HIHAT_OPEN 46
#define HIHAT_OPEN_RIM 26
#define HIHAT_CLOSED 42
#define HIHAT_CLOSED_RIM 22
#define HIHAT_PEDAL 44
#define CRASH1 49
#define CRASH1_EDGE 55
#define CRASH2 57
#define CRASH2_EDGE 52
#define RIDE 51
#define RIDE_EDGE 59
#define RIDE_BELL 53

void on()      // Basic blink function
{
  digitalWrite(LED,HIGH);
}

void off()
{
  digitalWrite(LED,LOW);
}

// ----------------------------------------------------------------------------

const int RED = 11;
const int GREEN =9;
const int BLUE = 10;
const float DECAY = 0.32;

void setup()
{
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);

  pinMode(LED, OUTPUT);
  MIDI.begin(MIDI_CHANNEL_OMNI); 
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);
}

float fBrightness=0;
float red=0;
float green=0;
float blue=0;
long notesReceived=0;
int reds[]   = { 
  255 , 128,  0,   255, 128,  0};
int greens[] = {  
  255,  0, 128, 255, 0,   0};
int blues[]  = {  
  50,    0,   0,   255, 128, 128};
int brights[]= {  
  .75,   0, 0, .75, 0, 0};
int colorsetCounter=0;
int colorset=0;

void handleNoteOn(byte channel, byte pitch, byte velocity){

  on();

  switch(pitch) {
  case CRASH2_EDGE:
    red=0;
    green=1;
    blue=0;
    break;
  case CRASH2:
    red=1;
    green=0;
    blue=0;
    break;
  default:
    red=0;
    green=0;
    blue=0;
    break;
  }
  float fCandidateBrightness=velocity*1.5;
  if (fCandidateBrightness < fBrightness ) {
    fBrightness += 32;
  }
  else {
    fBrightness = fCandidateBrightness;
  }

}

void handleNoteOff(byte channel, byte pitch, byte velocity){
  off();
}

void loop(){
  MIDI.read();
  fBrightness = fBrightness - .03;
  if ( fBrightness < 0 ) {
    fBrightness = 0;
  }
  else if ( fBrightness > 255 ) {
    fBrightness = 255;
  }
  setLed(red,green,blue,fBrightness);
}

void setLed(float r, float g, float b, float fB) {
  analogWrite(RED,   linearBrightness(r*fB));
  analogWrite(GREEN, linearBrightness(g*fB));
  analogWrite(BLUE,  linearBrightness(b*fB));
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

