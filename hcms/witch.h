#include "lightshow.h"

#define BLOCK_SIZE 10
#define NUM_BLOCKS PIXELS_PER_STRIP / BLOCK_SIZE

struct Block {
  RGBB color;
  unsigned int boostUntil; 
  unsigned int dimUntil; 
};

class WitchLightshow : public Lightshow {
  public:
    WitchLightshow( OctoWS2811 *p);
    void decay();
    void updatePixels();
    void reset();
    void handleNoteOn(byte channel, byte instrument, byte velocity);
    void handleNoteOff(byte channel, byte instrument, byte velocity);

  private:

    RGBB randomColor();
    byte randomColorIndex();
    void scrambleColors();
    void hiHat();
    void ride();
    void crash();
  
    const RGBB palette[2][5] = {
      {BLUE_HVY_PURPLE, BLUE, ORANGE, GREEN_HEAVY_TEAL, WHITE},
      {RED, YELLOW, GREEN, WHITE, BLUE}
    };
    Block _blocks[NUM_STRIPS][NUM_BLOCKS];
    byte _currentPaletteIndex;
    byte _numHiHats;
    byte _numRides;
    bool _soloColorMode;
    RGBB _soloColor;
};
