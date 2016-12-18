#include <Adafruit_NeoPixel.h>

#define PIN 6

#define LED_COUNT 40

Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, PIN);

// #define BRIGHT_STEP_COUNT 5
// uint8_t brightnesSteps[BRIGHT_STEP_COUNT] = {5,15,40,70,93};
//uint8_t brightness = 4;
//uint8_t lastButtonState = 1;

int led[LED_COUNT], ledb[LED_COUNT], ledc[LED_COUNT];
int ledr[LED_COUNT];
int b = 0;
int aob = 15; // was 15
int bsmin = 1;
int bsmax = 30;
// int bss = bsmin;

long waiting_time = 50;

void setup()
{  
  Serial.begin(9600);
  
  strip.begin();
  strip.setBrightness(50); // 1/3 brightness
  strip.show();
  
  for (int i; i < LED_COUNT; i++) {
    led[i] = 0;
  }
}

void loop() {  
  //static uint8_t i = 0;
  
  //static int pixelIndex;
  
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
      // sendval = 512 - sendval;
      sendval = 511 - sendval;
    }
    
    int o = sendval - ledr[a];
    if (o < 0) {
      o = 0;
    }

#ifdef DBB
    Serial.print("o=");
    Serial.print(o);
    Serial.print(" sendval=");
    Serial.print(sendval);
    Serial.println();
#endif
    
    if (ledc[a] > 2) {
      strip.setPixelColor (a, strip.Color(o, o, sendval));
    } else if (ledc[a] > 1) {
      strip.setPixelColor (a, strip.Color(o, sendval, o));
    } else {
      strip.setPixelColor (a, strip.Color(sendval, o, o));
    }
  }
  strip.show();
  
  delay(waiting_time);
}

