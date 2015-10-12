//
//LIBRARIES
//
#include <MIDI.h>
#include <OctoWS2811.h>

//
// LABELS
//
#include "songs.h"
#include "colors.h"
#include "constants.h"

//
// LIBRARIES
//
#include "pixels.h"

//
// LIGHTSHOWS
//
#include "plain_lightshow.h"
#include "bloodletting.h"

#define BOARD_LED_PIN 13
#define MAX_FPS 100
#define MILLIS_BETWEEN_FRAMES 1000/MAX_FPS
#define OCTO_CONFIG WS2811_GRB | WS2811_800kHz
#define MILLIS_BETWEEN_FPS 2000

//
// GLOBALS
//
DMAMEM int displayMemory[PIXELS_PER_STRIP * 6];
int drawingMemory[PIXELS_PER_STRIP * 6];
const int config = WS2811_GRB | WS2811_800kHz;
OctoWS2811 g_pixels(PIXELS_PER_STRIP, displayMemory, drawingMemory, config);
unsigned long g_last_paint_millis = millis();
unsigned long g_last_decay_millis = millis() + MILLIS_BETWEEN_FRAMES / 2;
unsigned long g_last_fps_millis = millis();
elapsedMillis runtime;
unsigned long g_frames_processed = 0;

//
// LIGHTSHOWS
//
Lightshow *raven = new PlainLightshow( &g_pixels, PURPLE);
Lightshow *skin = new PlainLightshow( &g_pixels, WHITE);
Lightshow *loose = new PlainLightshow( &g_pixels, BLUE);
Lightshow *bloodletting = new BloodlettingLightshow( &g_pixels);
Lightshow *creep = new PlainLightshow( &g_pixels, YELLOW);
Lightshow *witch = new PlainLightshow( &g_pixels, GREEN);
Lightshow *hands = new PlainLightshow( &g_pixels, TEAL);
Lightshow *bdm = new PlainLightshow( &g_pixels, ORANGE);
Lightshow *unknown = new PlainLightshow( &g_pixels, PINK);

Lightshow *activeLightshow = bloodletting;

//
// SETUPS
//
void setup() {
  helloBoardLed();
  setupMidi();
  setupPixels();
  helloPixels();
  setupDebugChannel();
}

void setupMidi() {
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleProgramChange(handleProgramChange);
  MIDI.setHandleActiveSensing(handleActiveSensing);
  MIDI.begin(MIDI_CHANNEL_OMNI);
}

void setupPixels() {
  g_pixels.begin();
  g_pixels.show();
}

void setupDebugChannel() {
  Serial.begin(57600);
  Serial.println("setup() complete!");
  Serial.println("Ready for action");
}

void loop() {
  MIDI.read();
  decay();
  repaint();
  outputFPS();
}

//
// DELEGATES TO LIGHTSHOW PLUGIN
//

void decay() {
  if ( millis() - g_last_decay_millis >= MILLIS_BETWEEN_FRAMES ) {
    activeLightshow->decay();
    g_last_decay_millis = millis();
  }
}

void repaint() {
  if ( millis() - g_last_paint_millis >= MILLIS_BETWEEN_FRAMES ) {
    activeLightshow->updatePixels();
    g_last_paint_millis = millis();
    g_pixels.show();
    g_frames_processed++;
  }
}

//
// MIDI CALLBACKS
//

void handleNoteOn(byte channel, byte instrument, byte velocity) {
  Serial.println(String("Note On:  ch=") + channel + ", note=" + instrument + ", velocity=" + velocity);
  activeLightshow->handleNoteOn( channel, instrument, velocity );
  on();
}

void handleNoteOff(byte channel, byte instrument, byte velocity) {
  off();
}

void handleProgramChange (byte channel, byte drumkit) {
  Serial.println(String("Program Change: ch=") + channel + ", number=" + drumkit );
  switch (drumkit) {
    case BLOODLETTING:
      activeLightshow = bloodletting;
      break;
    case CREEP:
      activeLightshow = creep;
      break;
    case RAVEN:
      activeLightshow = raven ;
      break;
    case SKIN:
      activeLightshow = skin;
      break;
    case LOOSE:
      activeLightshow = loose ;
      break;
    case BDM:
      activeLightshow = bdm;
      break;
    case HANDS:
      activeLightshow = hands;
      break;
    case WITCH:
      activeLightshow = witch;
      break;
    default:
      activeLightshow = unknown;
  }
  activeLightshow->reset();
}

bool activeSensingLightToggle = false;
void handleActiveSensing() {
  digitalWrite(BOARD_LED_PIN, activeSensingLightToggle ? HIGH : LOW );
  activeSensingLightToggle = ! activeSensingLightToggle;
}

//
// STATUS
//

void outputFPS() {
  if ( millis() - g_last_fps_millis >= MILLIS_BETWEEN_FPS ) {
    Serial.println(String("frames=") + g_frames_processed );
    float fps = g_frames_processed / ( runtime / 1000 )  ;
    Serial.println(String("FPS=") + fps );
    g_last_fps_millis = millis();
  }
}

void helloPixels() {
  for (byte p = 0; p < PIXELS_PER_STRIP; p++) {
    for (byte s = 0; s < 5; s++) {
      Pixels::pixelSet(&g_pixels, s, p, ORANGE);
      if ( p >= 4 )  {
        Pixels::pixelSet(&g_pixels, s, p - 4, BLACK);
      }
    }
    g_pixels.show();
    delay(10);
  }
  Pixels::floodFill(&g_pixels, BLACK);
  g_pixels.show();
}

void helloBoardLed() {
  pinMode(BOARD_LED_PIN, OUTPUT);
  digitalWrite(BOARD_LED_PIN, HIGH);
  delay(250);
  digitalWrite(BOARD_LED_PIN, LOW);
  delay(100);
  digitalWrite(BOARD_LED_PIN, HIGH);
  delay(250);
  digitalWrite(BOARD_LED_PIN, LOW);
}

void on() {
  digitalWrite(BOARD_LED_PIN, HIGH);
}

void off() {
  digitalWrite(BOARD_LED_PIN, LOW);
}
