#include <MIDI.h>
#include <limits.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define LED 13                   // LED pin on Arduino Uno
#define CRASH2_EDGE 52
#define CRASH2_BOW 57

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
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);

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
int reds[]   = { 
  255 , 128,  0,   255, 128,  0};
int greens[] = {  
  255,  0, 128, 255, 0,   0};
int blues[]  = {  
  50,    0,   0,   255, 128, 128};
int brights[]= {  
  .75,   0, 0, .75, 0, 0};
int colorsetCounter=0;
int colorset=0;

void handleNoteOn(byte channel, byte pitch, byte velocity){
  brightness=brightness+.25;
  colorset = colorsetCounter++ % 6 ;
  red = reds[colorset];
  green = greens[colorset];
  blue = blues[colorset];
  brightness = brightness + brights[colorset];
  on();
}

void handleNoteOff(byte channel, byte pitch, byte velocity){
  off();
}


void loop(){
  MIDI.read();

  brightness = brightness - .0001;
  if ( brightness < 0 ) {
    brightness = 0;
  }
  else if ( brightness > 1 ) {
    brightness = 1;
  }

  setLed(red,green,blue,brightness);

}

void setLed(byte r, byte g, byte b, double brightness) {
  analogWrite(RED,r*brightness);
  analogWrite(GREEN,g*brightness);
  analogWrite(BLUE,b*brightness);
}

