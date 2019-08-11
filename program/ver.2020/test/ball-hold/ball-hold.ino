#include <Adafruit_NeoPixel.h>

#define RGBLED_OUTPIN    28
#define NUMRGBLED        16

#define BRIGHT 150

Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel(NUMRGBLED, RGBLED_OUTPIN, NEO_GRB + NEO_KHZ800);

void setup() {
  RGBLED.begin();
  RGBLED.show();
}

void loop() {
  RGBLED.begin();
  RGBLED.setBrightness(BRIGHT);

  if (digitalRead(29) == HIGH) {
    for (int i = 0; i < RGBLED.numPixels(); i++) {
      RGBLED.setPixelColor(i, 255, 0, 0);
    }
  } else {
    for (int i = 0; i < RGBLED.numPixels(); i++) {
      RGBLED.setPixelColor(i, 0, 255, 0);
    }
  }

  RGBLED.show();
}
