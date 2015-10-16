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
#include "plain.h"
#include "bloodletting.h"
#include "bdm.h"
#include "witch.h"

#define BOARD_LED_PIN 13
#define MAX_FPS 100
#define MILLIS_BETWEEN_FRAMES 1000/MAX_FPS
#define OCTO_CONFIG WS2811_GRB | WS2811_800kHz
#define MILLIS_BETWEEN_FPS 2000
#define VELOCITY_FLOOR 32

#define DEMO true
#define MILLIS_BETWEEN_DEMO_NOTES 315
#define DEMO_CHANNEL 10

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

unsigned long g_last_demo_note_millis = millis();
unsigned long g_demo_notes_sent = 0;

Lightshow *raven;
Lightshow *skin;
Lightshow *loose;
Lightshow *bloodletting;
Lightshow *creep;
Lightshow *witch;
Lightshow *hands;
Lightshow *bdm;
Lightshow *unknown;

Lightshow *activeLightshow;

//
// SETUPS
//
void setup() {
  helloBoardLed();
  setupRandom();
  setupMidi();
  setupPixels();
  helloPixels();
  setupDebugChannel();
  setupLightshows();
}

void setupRandom() {
  pinMode(A1, INPUT);
  randomSeed(analogRead(A1));
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

void setupLightshows() {
  raven = new PlainLightshow( &g_pixels, PURPLE);
  skin = new PlainLightshow( &g_pixels, WHITE);
  loose = new PlainLightshow( &g_pixels, BLUE);
  bloodletting = new BloodlettingLightshow( &g_pixels);
  creep = new PlainLightshow( &g_pixels, YELLOW);
  witch = new WitchLightshow( &g_pixels);
  hands = new PlainLightshow( &g_pixels, TEAL);
  bdm = new BDMLightshow( &g_pixels);
  unknown = new PlainLightshow( &g_pixels, PINK);

  activeLightshow = witch;
  activeLightshow->reset();
}

//
// LLOOOOOOOOOOOOOOOOOOOOOPPP!
//
void loop() {
  demo();
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
  if (velocity > VELOCITY_FLOOR ) {
    activeLightshow->handleNoteOn( channel, instrument, velocity );
  }
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
      Pixels::pixelSet(&g_pixels, s, p, ORANGE, 1.0);
      if ( p >= 4 )  {
        Pixels::pixelSet(&g_pixels, s, p - 4, BLACK, 0.0);
      }
    }
    g_pixels.show();
    delay(10);
  }
  Pixels::floodFill(&g_pixels, BLACK, 0);
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

void demo() {
  if ( millis() - g_last_demo_note_millis >= MILLIS_BETWEEN_DEMO_NOTES  ) {

    byte note = ( g_demo_notes_sent++ % 16 ) + 1;
    switch ( note ) {
      case 1:
        activeLightshow->handleNoteOn(DEMO_CHANNEL, HIHAT_OPEN, 127);
        activeLightshow->handleNoteOn(DEMO_CHANNEL, KICK, 127);
        break;
      case 3:
        activeLightshow->handleNoteOn(DEMO_CHANNEL, HIHAT_OPEN, 127);
        activeLightshow->handleNoteOn(DEMO_CHANNEL, SNARE, 127);
        break;
      case 5:
        activeLightshow->handleNoteOn(DEMO_CHANNEL, HIHAT_OPEN, 127);
        activeLightshow->handleNoteOn(DEMO_CHANNEL, KICK, 127);
        break;
      case 7:
        activeLightshow->handleNoteOn(DEMO_CHANNEL, HIHAT_OPEN, 127);
        activeLightshow->handleNoteOn(DEMO_CHANNEL, SNARE, 127);
        break;
      case 9:
        activeLightshow->handleNoteOn(DEMO_CHANNEL, RIDE, 127);
        activeLightshow->handleNoteOn(DEMO_CHANNEL, KICK, 127);
        break;
      case 10:
        activeLightshow->handleNoteOn(DEMO_CHANNEL, RIDE, 127);
        break;
      case 11:
        activeLightshow->handleNoteOn(DEMO_CHANNEL, RIDE, 127);
        activeLightshow->handleNoteOn(DEMO_CHANNEL, SNARE, 127);
        break;
      case 12:
        activeLightshow->handleNoteOn(DEMO_CHANNEL, RIDE, 127);
        break;
      case 13:
        activeLightshow->handleNoteOn(DEMO_CHANNEL, RIDE, 127);
        activeLightshow->handleNoteOn(DEMO_CHANNEL, KICK, 127);
        break;
      case 14:
        activeLightshow->handleNoteOn(DEMO_CHANNEL, RIDE, 127);
        break;
      case 15:
        activeLightshow->handleNoteOn(DEMO_CHANNEL, RIDE, 127);
        activeLightshow->handleNoteOn(DEMO_CHANNEL, SNARE, 127);
        break;
      case 16:
        activeLightshow->handleNoteOn(DEMO_CHANNEL, RIDE, 127);
        break;
    
    }
    g_last_demo_note_millis = millis();
  }
}

