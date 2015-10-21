#include "lightshow.h"

#define LOOSE_BLOCK_SIZE 5
#define NUM_LOOSE_BLOCKS PIXELS_PER_STRIP / LOOSE_BLOCK_SIZE
#define SPIN_CEIL .75
#define SPIN_DECAY .00325
#define SPIN_BOOST .5

struct LooseBlock {
  RGBB color;
};

class LooseLightshow : public Lightshow {
  public:
    LooseLightshow( OctoWS2811 *p);
    void decay();
    void updatePixels();
    void reset();
    void handleNoteOn(byte channel, byte instrument, byte velocity);
    void handleNoteOff(byte channel, byte instrument, byte velocity);

  private:
    LooseBlock _blocks[NUM_STRIPS][NUM_LOOSE_BLOCKS];
    byte _smallPaletteIndex;
    byte _largePaletteIndex;
    unsigned int _kickCount;
    unsigned int _snareCount;
    unsigned int _rideCount;
    float _spinRate;
    float _spinPixelOffset;
    
    const RGBB smallPalette[3] = { RED, GREEN, BLUE };
    const RGBB largePalette[7] = { YELLOW, PURPLE, ORANGE, TEAL, HOT_PINK, WHITE, BLACK };

    void ride();
    void hihat();
    void snare();
    void tom();
    void crash();
    void kick();
    RGBB colorNow(byte stripe,byte block);

};
