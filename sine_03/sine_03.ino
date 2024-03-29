// The project has to be run on a LGT8F328P based board because it uses analog output pin (8 bit DAC)!

// This is just a proof of concept program, playing a nice sounding sine wave instead of not so nice square wave
// that microcontrolers and buzzers usually give us. 
// It is intended to grow to a propper sine wave Morse buzzer, or even a keyer, while using as little components as possible.

// Connect D4 DAC pin to headphones or an amplified speaker. 
// For a very simple sine wave Morse buzzer, connect Morse key in between. (and short pin 2 to ground to get the tone to be always on)

// For a bit more advanced version, that works better, connect the key between pin 2 and ground.

// Created by YU4HAK. Code is free in every possible sense.

#include <Arduino.h>

#define pinSpk DAC0 // D4 DAC pin, 

const byte ledPin = 13;  // Led pin
const byte interruptPin = 2; // Morse key pin
volatile byte state = LOW;
bool playStatus = false;

const byte pinA = 5;        // rotary encoder
const byte pinB = 3;        // rotary encoder
int buzzFreq = 8;           // Default setting (8) gives around 800hz. Lower values increase frequency. 

int sine_points[] = {
    128, 131, 134, 137, 140, 144, 147, 150,
    153, 156, 159, 162, 165, 168, 171, 174,
    177, 179, 182, 185, 188, 191, 193, 196,
    199, 201, 204, 206, 209, 211, 213, 216,
    218, 220, 222, 224, 226, 228, 230, 232,
    234, 235, 237, 239, 240, 241, 243, 244,
    245, 246, 248, 249, 250, 250, 251, 252,
    253, 253, 254, 254, 254, 255, 255, 255,
    255, 255, 255, 255, 254, 254, 254, 253,
    253, 252, 251, 250, 250, 249, 248, 246,
    245, 244, 243, 241, 240, 239, 237, 235,
    234, 232, 230, 228, 226, 224, 222, 220,
    218, 216, 213, 211, 209, 206, 204, 201,
    199, 196, 193, 191, 188, 185, 182, 179,
    177, 174, 171, 168, 165, 162, 159, 156,
    153, 150, 147, 144, 140, 137, 134, 131,
    128, 125, 122, 119, 116, 112, 109, 106,
    103, 100, 97, 94, 91, 88, 85, 82,
    79, 77, 74, 71, 68, 65, 63, 60,
    57, 55, 52, 50, 47, 45, 43, 40,
    38, 36, 34, 32, 30, 28, 26, 24,
    22, 21, 19, 17, 16, 15, 13, 12,
    11, 10, 8, 7, 6, 6, 5, 4,
    3, 3, 2, 2, 2, 1, 1, 1,
    1, 1, 1, 1, 2, 2, 2, 3,
    3, 4, 5, 6, 6, 7, 8, 10,
    11, 12, 13, 15, 16, 17, 19, 21,
    22, 24, 26, 28, 30, 32, 34, 36,
    38, 40, 43, 45, 47, 50, 52, 55,
    57, 60, 63, 65, 68, 71, 74, 77,
    79, 82, 85, 88, 91, 94, 97, 100,
    103, 106, 109, 112, 116, 119, 122, 125};

void setup() {
  analogReference(DEFAULT);  // 5v
  pinMode(DAC0, ANALOG);  // Initialize pin as DAC

  pinMode (ledPin, OUTPUT);
  pinMode (interruptPin , INPUT_PULLUP);
  pinMode (pinA , INPUT_PULLUP);
  pinMode (pinB , INPUT_PULLUP);

  attachInterrupt (digitalPinToInterrupt (interruptPin ), play, CHANGE ) ;
  attachInterrupt (digitalPinToInterrupt (pinB ), freqDown, FALLING   ) ;
}

// TODO: delay/tone control with button/encoder

void loop() {
   digitalWrite (ledPin, state ) ;
   if(playStatus == true) {
      for (int thisPonit = 0; thisPonit < 256; thisPonit++) {
        analogWrite( pinSpk, sine_points[thisPonit] );    
        delayMicroseconds(buzzFreq); 
      }    
   }   

   if  (digitalRead(pinA) == LOW){
    freqUp();
   }
}

void play()
{
  static unsigned long last_interrupt_time = 0;
  unsigned long interrupt_time = millis();
  // If interrupts come faster than 10ms, assume it's a bounce and ignore
  if (interrupt_time - last_interrupt_time > 10) 
  {
    playStatus = !playStatus;
    state = !state ;
  }
  last_interrupt_time = interrupt_time;
}

void freqDown()
{
  static unsigned long last_interrupt_time1 = 0;
  unsigned long interrupt_time1 = millis();
  // If interrupts come faster than 10ms, assume it's a bounce and ignore
  if (interrupt_time1 - last_interrupt_time1 > 10) 
  {
    buzzFreq += 1;
  }
  last_interrupt_time1 = interrupt_time1;
}

void freqUp()
{
    buzzFreq -= 1;
    delay(100); // simple debounce, as no more interrupt pins :(
}
