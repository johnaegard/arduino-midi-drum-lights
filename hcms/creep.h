#include "lightshow.h"
#define VERSE_PALETTE_INDEX 0
#define CHORUS_PALETTE_INDEX 1
#define CHORUS_DECAY 0.02
#define CHORUS_BRIGHTNESS_FLOOR 0.4
#define CHORUS_BRIGHTNESS_CEIL 1.0

#define VERSE_DECAY 0.015
#define VERSE_BRIGHTNESS_FLOOR 0.55
#define VERSE_BRIGHTNESS_CEIL 1.0

#define CYMBAL_DECAY 0.015
#define CYMBAL_BRIGHTNESS_FLOOR 0.25
#define CYMBAL_BRIGHTNESS_CEIL 1.0

#define CYMBAL_DURATION_MS 250

class CreepLightshow : public Lightshow
{
  public:
    CreepLightshow( OctoWS2811 *p);
    void decay();
    void updatePixels();
    void reset();
    void handleNoteOn(byte channel, byte instrument, byte velocity);
    void handleNoteOff(byte channel, byte instrument, byte velocity);
    
  private:
    byte _numSnares;
    byte _paletteIndex;
    byte _colorIndex;
    float _chorusBrightness;
    bool _chorusMode;
    float _verseBrightness;
    const RGBB palette[2][4] = {
      {COOLBLUE, LIGHTTEAL, COOLVIOLET, COOLGREEN},
      {WHITE,WHITE, WHITE, WHITE}
    };
    void ride();
    void hihat();
    void snare();
    void tom();
    void crash();
};

