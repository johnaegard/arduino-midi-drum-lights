#include "lightshow.h"

#define SPARKLE_DECAY 0.005
#define SPARKLE_FLOOR 0
#define SPARKLE_CEIL 100000
#define SPARKLE_FACTOR_START 0.9

#define ENERGY_FLOOR 0
#define RED_CEIL 60
#define WHITE_CEIL 30

#define RED_DECAY_FLOOR 0.0
#define RED_DECAY_CEIL 0.3

#define WHITE_DECAY_FLOOR 0.5
#define WHITE_DECAY_CEIL 1.0

#define KICK_IMPULSE 60
#define SNARE_IMPULSE 60

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
    byte _currentWhiteStrip;
};
