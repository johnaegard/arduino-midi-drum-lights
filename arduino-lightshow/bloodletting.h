#include <midi_Message.h>
#include <midi_Namespace.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Settings.h>
#include <Arduino.h>
#include <FastLED.h>
#include "lightshow.h"

#define SPARKLE_DECAY 0.005
#define SPARKLE_FLOOR 0
#define SPARKLE_CEIL 100000
#define SPARKLE_FACTOR_START 0.9

#define ENERGY_FLOOR 0
#define RED_CEIL 60
#define WHITE_CEIL 30

#define RED_DECAY_FLOOR 0.0
#define RED_DECAY_CEIL 1.5

#define WHITE_DECAY_FLOOR 0.5
#define WHITE_DECAY_CEIL 4.0

#define KICK_IMPULSE 60
#define SNARE_IMPULSE 30

class BloodlettingLightshow : public Lightshow
{
  public:
    BloodlettingLightshow(
      midi::MidiInterface<HardwareSerial> *m,
      CRGB pixels[][NUM_PIXELS_PER_STRIP]
      );
    void decay();
    void updatePixels();
    void reset();
    void handleNoteOn(byte channel, byte instrument, byte velocity);
    void handleNoteOff(byte channel, byte instrument, byte velocity);
    
  private:
    void decaySparkle();
    void resetSparkle();
    float _fRedEnergy[5] = {0,0,0,0,0};
    float _fWhiteEnergy[5]={0,0,0,0,0};
    float _fSparkle;
    byte _currentRedStrip;
    byte _currentWhiteStrip;
};
