#include <FastLED.h>
#include <midi_Message.h>
#include <midi_Namespace.h>
#include <MIDI.h>
#include <midi_Defs.h>
#include <midi_Settings.h>
#include "songs.h"
#include "td15_midi_notes.h"

#include "plain_lightshow.h"
#include "bloodletting.h"


#define BOARD_LED_PIN 13
#define MIDI_ENABLE_PIN 12
#define NUM_PIXELS_PER_STRIP 60
#define MAX_FPS 12
#define MILLIS_BETWEEN_FRAMES 1000/MAX_FPS

unsigned long g_last_paint_millis = millis();
unsigned long g_last_decay_millis = millis() + MILLIS_BETWEEN_FRAMES / 2;
byte p;
byte s;

CRGB g_pixels[5][NUM_PIXELS_PER_STRIP];

MIDI_CREATE_DEFAULT_INSTANCE();

Lightshow *raven = new PlainLightshow(&MIDI,g_pixels, CRGB::Purple);
Lightshow *skin = new PlainLightshow(&MIDI,g_pixels, CRGB::White);
Lightshow *loose = new PlainLightshow(&MIDI,g_pixels, CRGB::Blue);
Lightshow *bloodletting = new BloodlettingLightshow(&MIDI,g_pixels);
Lightshow *creep = new PlainLightshow(&MIDI,g_pixels, CRGB::Yellow);
Lightshow *witch = new PlainLightshow(&MIDI,g_pixels, CRGB::Green);
Lightshow *hands = new PlainLightshow(&MIDI,g_pixels, CRGB::Teal);
Lightshow *bdm = new PlainLightshow(&MIDI,g_pixels, CRGB::Orange);
Lightshow *unknown = new PlainLightshow(&MIDI,g_pixels, CRGB::DeepPink);

Lightshow *activeLightshow = bloodletting;

//
//SETUP
//

void setup() {
  setupMidi();
  setupPixels();
  testPixels();
}

void setupPixels() {
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(8, OUTPUT);
  FastLED.addLeds<NEOPIXEL, 2>(g_pixels[0], NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 3>(g_pixels[1], NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 4>(g_pixels[2], NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 5>(g_pixels[3], NUM_PIXELS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 8>(g_pixels[4], NUM_PIXELS_PER_STRIP);
}

void setupMidi() {
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleProgramChange(handleProgramChange);
  MIDI.begin(MIDI_CHANNEL_OMNI);
  pinMode(MIDI_ENABLE_PIN, OUTPUT);
  digitalWrite(MIDI_ENABLE_PIN, HIGH);
}

void loop() {
  MIDI.read();
  decay();
  MIDI.read();
  repaint();
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
    FastLED.show();
  }
}

//
// MIDI HANDLING
//

void handleNoteOn(byte channel, byte instrument, byte velocity) {
  activeLightshow->handleNoteOn( channel, instrument, velocity );
  on();
}

void handleNoteOff(byte channel, byte instrument, byte velocity) {
  activeLightshow->handleNoteOff( channel, instrument, velocity );
  off();
}

void handleProgramChange (byte channel, byte drumkit) {
  on();
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
  off();
}

//
// PARITY CHECK CODE
//

void flashBoardLed() {
  digitalWrite(BOARD_LED_PIN, HIGH);
  delay(10);
  digitalWrite(BOARD_LED_PIN, LOW);
  delay(10);
}

void testPixels() {
  for (p = 0; p < NUM_PIXELS_PER_STRIP; p++) {
    for (s = 0; s < 5; s++) {
      g_pixels[s][p] = CRGB::HotPink;
      if ( p >= 4 )  {
        g_pixels[s][p - 4] = CRGB::Black;
      }
    }
    FastLED.show();
  }
  allBlack();
}

void allBlack() {
  for (p = 0; p < NUM_PIXELS_PER_STRIP; p++) {
    for (s = 0; s < 5; s++) {
      g_pixels[s][p] = CRGB::Black;
    }
  }
}

void on() {
  digitalWrite(BOARD_LED_PIN, HIGH);
}

void off() {
  digitalWrite(BOARD_LED_PIN, LOW);
}

