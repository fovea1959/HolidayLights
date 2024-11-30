#include <FastLED.h>

#define LED_COUNT 60 // BlinkyTape has 60 LEDs!
struct CRGB leds[LED_COUNT]; // this struct contains 60 CRGB values.  This is where 

#define PIN_BUTTON 10
#define PIN_IO_A   7
#define PIN_IO_B   11
#define PIN_SIGNAL 13
#define PIN_INPUT  10

int b = 0;
int aob = 4;
int bsmin = 1;
int bsmax = 30;
int waiting_time = 20; //time in ms

int led[LED_COUNT], ledb[LED_COUNT], ledc[LED_COUNT];
int ledr[LED_COUNT];

// first, let's get ready to blink using some FastLED routines
// take a look at the FastLED example called Fast2Dev for more usage info
void setup()
{  
  FastLED.addLeds<WS2812B, PIN_SIGNAL, GRB>(leds, LED_COUNT); // this configures the BlinkyBoard - leave as is.
  FastLED.showColor(CRGB(0, 0, 0)); // set the color for the strip all at once.
  FastLED.setBrightness(30);
  FastLED.show(); // you'll always need to call this function to make your changes happen.
}

// this is the main loop where we call the other functions. 
void loop() {
  b++;
  if (b > 3000) b = 0;

  if ((b % aob) == 0) {
    uint8_t pos = random(LED_COUNT);
    if (led[pos] == 0) { // not already blinking
      uint8_t bs = random(bsmax-bsmin)+bsmin;
      ledb[pos] = bs;
      ledr[pos] = random(254);
      led[pos] = ledb[pos];
      ledc[pos] = random(3); // 4 for red/blue/green; 3 for just red/green
    }
  }

  for (uint8_t a = 0; a < LED_COUNT; a++) {
    if (led[a] > 0) {
       led[a] = led[a] + ledb[a];
    }
  
    if (led[a] > 511) {
      led[a] = 0;
    }
      
    int sendval = led[a];
    if (led[a] > 255) {
      sendval = 511 - sendval;
    }
      
    int o = sendval - ledr[a];
    if (o < 0) {
      o = 0;
    }
  
  #ifdef DBB
    Serial.print("a=");
    Serial.print(a);
    Serial.print(" o=");
    Serial.print(o);
    Serial.print(" sendval=");
    Serial.print(sendval);
    Serial.println();
  #endif
      
    if (ledc[a] > 2) {
      leds[a] = CRGB(o, o, sendval);
    } else if (ledc[a] > 1) {
      leds[a] = CRGB(o, sendval, o);
    } else {
      leds[a] = CRGB(sendval, o, o);
    }
  }
  FastLED.show();
  
  delay(waiting_time);
}
