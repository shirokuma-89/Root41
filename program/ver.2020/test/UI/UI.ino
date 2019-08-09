#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <FaBoLCDmini_AQM0802A.h>

#define RGBLED_OUTPIN    28
#define NUMRGBLED        16

#define SW1 22
#define SW2 23
#define SW3 25

FaBoLCDmini_AQM0802A lcd;
Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel(NUMRGBLED, RGBLED_OUTPIN, NEO_GRB + NEO_KHZ800);

unsigned long timer;
int mode = 0;

void setup() {
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);

  RGBLED.begin();
  RGBLED.show();

  RGBLED.begin();
  RGBLED.setBrightness(50);
  for (int i = 0; i <= 15; i++) {
    RGBLED.setPixelColor(i, 255, 255, 255);
  }
  RGBLED.show();

  lcd.begin();
  lcd.command(0x38);
  lcd.command(0x39);
  lcd.command(0x14);
  lcd.command(0x73);
  lcd.command(0x51);
  lcd.command(0x6c);
  lcd.command(0x38);
  lcd.command(0x01);
  lcd.command(0x0c);
  // Print a message to the LCD.
  lcd.print("HELLO WORLD!");
  delay(500);
}

void loop() {
  RGBLED.begin();
  RGBLED.setBrightness(50);

  if (millis() - timer >= 200) {
    lcd.clear();
    lcd.print("Root41 UI test");
    lcd.command(0xC0);
    if (mode == 0) {
      lcd.print("hogehoge");
    } else if (mode == 1) {
      lcd.print("fugafuga");
    } else {
      lcd.print("piyopiyo");
    }

    timer = millis();
  }

  if (digitalRead(SW1) == LOW) {
    mode = 0;
  }

  if (digitalRead(SW2) == LOW) {
    mode = 1;
  }

  if (digitalRead(SW3) == LOW) {
    mode = 2;
  }

  if (mode == 0) {
    for (int i = 0; i <= 15; i++) {
      RGBLED.setPixelColor(i, 255, 0, 0);
    }
  } else if (mode == 1) {
    for (int i = 0; i <= 15; i++) {
      RGBLED.setPixelColor(i, 0, 255, 0);
    }
  } else {
    for (int i = 0; i <= 15; i++) {
      RGBLED.setPixelColor(i, 0, 0, 255);
    }
  }

  RGBLED.show();
}
