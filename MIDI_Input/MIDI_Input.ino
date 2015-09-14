#include <MIDI.h>
#include <limits.h>

MIDI_CREATE_DEFAULT_INSTANCE();

// -----------------------------------------------------------------------------

// This example shows the old way of checking for input messages.
// It's simpler to use the callbacks now, check out the dedicated example.

#define LED 13                   // LED pin on Arduino Uno

// -----------------------------------------------------------------------------

void on()      // Basic blink function
{
  digitalWrite(LED,HIGH);
}

void off()
{
  digitalWrite(LED,LOW);
}

// ----------------------------------------------------------------------------

const int RED = 11;
const int GREEN =9;
const int BLUE = 10;
static unsigned long lWaitMillis;

void setup()
{
  pinMode(LED, OUTPUT);
  MIDI.begin(MIDI_CHANNEL_OMNI); 
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);
  lWaitMillis = millis() + 1000;  // initial setup
}

float brightness=0;
byte red=0;
byte green=0;
byte blue=0;
long notesReceived=0;
//int reds[]   = { 139 , 139,  10,   10,   72};
//int greens[] = {  0,    39,  10,   139,  72};
//int blues[]  = { 139,    0,  139,  10,    0 };
int reds[]   = { 255 , 128,  0,   255, 128,  0};
int greens[] = {  255,  0, 128, 255, 0,   0};
int blues[]  = {  0,    0,   0,   255, 128, 128};
int brights[]= {  .75,   .1, .1, .75, .1, .1};
int colorsetCounter=0;
int colorset=0;

void loop(){
  if (MIDI.read())                // Is there a MIDI message incoming ?
  {
    switch(MIDI.getType())   { 
      case midi::NoteOn:      
      brightness=brightness+.25;
        colorset = colorsetCounter++ % 6 ;
        red = reds[colorset];
        green = greens[colorset];
        blue = blues[colorset];
        brightness = brightness + brights[colorset];
      on();
      break;
      case midi::NoteOff:
      off();
      break;
    }
  }
  brightness = brightness - .001;
  if ( brightness < 0 ) {
    brightness = 0;
  }
  else if ( brightness > 1 ) {
    brightness = 1;
  }

  setLed(red,green,blue,brightness);
  delay(1);

//  if( (long)( millis() - lWaitMillis ) >= 0) {
//    if ( green == 0 ) {
//    }
//    else if ( green == 30 ) {
//      red = 139;
//      green = 0;
//      blue = 139;
//    }
//    lWaitMillis += 1000; 
//  }

}

void setLed(byte r, byte g, byte b, double brightness) {
  analogWrite(RED,r*brightness);
  analogWrite(GREEN,g*brightness);
  analogWrite(BLUE,b*brightness);
}











