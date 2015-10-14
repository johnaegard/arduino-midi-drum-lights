#ifndef colors_h
#define colors_h

struct RGBB {
  byte red;
  byte green;
  byte blue;

  RGBB() {};
  RGBB(byte r, byte g, byte b) {
    red = r;
    green = g;
    blue = b;
  }

  byte redWithBrightness(float brightness) {
    return (byte) ( red * brightness);
  }

  byte greenWithBrightness(float brightness) {
    return (byte)( green * brightness);
  }

  byte blueWithBrightness(float brightness) {
    return (byte)( blue * brightness );
  }

  String toString() {
    return String("<") + red + String(",") + green + String(",") + blue + String(">");
  }
  
};

const RGBB BLACK   = RGBB(0, 0, 0);
const RGBB RED     = RGBB(255, 0, 0);
const RGBB BLUE    = RGBB(0, 0, 255);
const RGBB GREEN   = RGBB(0, 255, 0);

const RGBB PURPLE  = RGBB(255, 0, 255);
const RGBB YELLOW  = RGBB(255, 255, 0);
const RGBB TEAL    = RGBB(0, 255, 255);
const RGBB ORANGE  = RGBB(255, 165, 0);

const RGBB WHITE   = RGBB(255, 255, 255);
const RGBB PINK    = RGBB(255, 127, 127);

const RGBB HALFGREEN = RGBB(0, 96, 0);
const RGBB HALFRED = RGBB(128, 0, 0);

const RGBB LIGHTRED = RGBB(255, 160, 160);
const RGBB LIGHTGREEN = RGBB(160, 255, 160);
const RGBB LIGHTBLUE = RGBB(160, 160, 255);
const RGBB LIGHTPURPLE  = RGBB(255, 160, 255);
const RGBB LIGHTYELLOW  = RGBB(255, 255, 160);
const RGBB LIGHTTEAL    = RGBB(160, 255, 255);
const RGBB LIGHTORANGE  = RGBB(255, 165, 160);


#endif
