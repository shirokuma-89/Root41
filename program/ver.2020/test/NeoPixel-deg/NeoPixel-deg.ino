#include <Adafruit_NeoPixel.h>

#define LED_PIN    28

#define LED_COUNT 16

int r = 0;
int g = 0;
int b = 0;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();
  strip.setBrightness(50);
  Serial.begin(115200);
}

void loop() {
  if (Serial.available() >= 3) {
    r = Serial.read();
    g = Serial.read();
    b = Serial.read();
    while (Serial.available())Serial.read();
  }

  strip.begin();
  for (int i = 0; i <= 15; i++) {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show();
}
