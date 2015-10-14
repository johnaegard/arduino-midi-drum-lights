#include <Arduino.h>
#include "lightshow.h"

class PlainLightshow : public Lightshow
{
  public:
    PlainLightshow( OctoWS2811 *p, RGBB color);
    void decay();
    void updatePixels();
    void reset();
    void handleNoteOn(byte channel, byte instrument, byte velocity);
    void handleNoteOff(byte channel, byte instrument, byte velocity);
    
  private:
    RGBB _color;
    float _fEnergy;
};

