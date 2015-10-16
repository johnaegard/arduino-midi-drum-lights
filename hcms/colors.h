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

  bool operator==(const RGBB& that) {
    return ( this->red == that.red ) && ( this->green == that.green ) && ( this->blue == that.blue) ;
  }

};

const RGBB BLACK   = RGBB(0, 0, 0);
const RGBB RED     = RGBB(255, 0, 0);
const RGBB BLUE    = RGBB(0, 0, 255);
const RGBB GREEN   = RGBB(0, 255, 0);

const RGBB PURPLE  = RGBB(255, 0, 255);
const RGBB YELLOW  = RGBB(255, 255, 0);
const RGBB TEAL    = RGBB(0, 255, 255);
const RGBB ORANGE  = RGBB(220, 120, 0);

const RGBB WHITE   = RGBB(255, 255, 255);
const RGBB PINK    = RGBB(255, 127, 127);

const RGBB HALFGREEN = RGBB(0, 96, 0);
const RGBB HALFRED = RGBB(128, 0, 0);

const RGBB LIGHTRED     = RGBB(255, 128, 128);
const RGBB LIGHTGREEN   = RGBB(160, 255, 160);
const RGBB LIGHTBLUE    = RGBB(160, 160, 255);
const RGBB LIGHTYELLOW  = RGBB(255, 255, 160);
const RGBB LIGHTTEAL    = RGBB(160, 255, 255);
const RGBB LIGHTORANGE  = RGBB(255, 165, 0);
const RGBB VIOLET       = RGBB(238, 130, 238);
const RGBB HOT_PINK     = RGBB(255, 105, 180);
const RGBB BLUE_HVY_PURPLE = RGBB(160, 100, 192);

const RGBB MISTY_ROSE = RGBB(255, 228, 225);
const RGBB LAVENDER_BLUSH = RGBB(255, 240, 245);
const RGBB LIGHT_CYAN = RGBB(224, 255, 255);
const RGBB ALICE_BLUE = RGBB(240, 248, 255);
const RGBB BUBBLES = RGBB(231, 254, 255 );
const RGBB PASTEL_YELLOW  = RGBB(255, 255, 192);
const RGBB BLUEWHITE = RGBB(32, 32, 48);
const RGBB GREEN_HEAVY_TEAL = RGBB(0,255,160);
const RGBB GREEN_HEAVY_YELLOW = RGBB(120,255,0);






#endif
