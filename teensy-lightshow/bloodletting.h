#include "lightshow.h"

#define SPARKLE_DECAY 0.0025
#define SPARKLE_FLOOR 0
#define SPARKLE_CEIL 100000
#define SPARKLE_FACTOR_START 0.95

#define ENERGY_FLOOR 0
#define RED_CEIL 30
#define WHITE_CEIL 30

#define RED_DECAY 0.125
#define WHITE_DECAY 0.6

#define KICK_IMPULSE 35
#define SNARE_IMPULSE 15

#define KICK_DRUM_AGGREGATE_MILLIS 250

class BloodlettingLightshow : public Lightshow
{
  public:
    BloodlettingLightshow(OctoWS2811 *p);
    void decay();
    void updatePixels();
    void reset();
    void handleNoteOn(byte channel, byte instrument, byte velocity);
    void handleNoteOff(byte channel, byte instrument, byte velocity);
    
  private:
    void decaySparkle();
    void resetSparkle();
    float _fRedEnergy[5] = {0,0,0,0,0};
    float _fWhiteEnergy[5] ={0,0,0,0,0};
    float _fSparkle;
    byte _currentRedStrip;
    byte _previousRedStrip;
    byte _currentWhiteStrip;
    unsigned int _previousKickDrumMillis = millis();
};
