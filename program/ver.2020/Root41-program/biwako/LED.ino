void _LED::gyroShow(unsigned long _color = 'hogehoge') {
  int deg2 = gyro.deg - 11;
  deg2 += 360;
  deg2 %= 360;
  if (deg2 > 180) {
    deg2 -= 360;
  }
  deg2 *= -1;
  deg2 += 360;
  deg2 %= 360;
  int light = 0;
  for (int i = 0; i <= 15; i++) {
    if (deg2 <= (i + 1) * 22.5) {
      light = i;
      break;
    }
  }

  if (LED.white) {
    RGBLED.setPixelColor(light, 255, 255, 255);
    RGBLED.setPixelColor((light + 1) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 2) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 15) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 14) % 16, 255, 255, 255);
    RGBLED.setPixelColor((light + 13) % 16, 255, 255, 255);
  } else {
    if (_color == 'hogehoge') {
      RGBLED.setPixelColor(light, LED.defaltColor);
      RGBLED.setPixelColor((light + 1) % 16, LED.defaltColor);
      RGBLED.setPixelColor((light + 2) % 16, LED.defaltColor);
      RGBLED.setPixelColor((light + 15) % 16, LED.defaltColor);
      RGBLED.setPixelColor((light + 14) % 16, LED.defaltColor);
      RGBLED.setPixelColor((light + 13) % 16, LED.defaltColor);
    } else {
      RGBLED.setPixelColor(light, _color);
      RGBLED.setPixelColor((light + 1) % 16, _color);
      RGBLED.setPixelColor((light + 2) % 16, _color);
      RGBLED.setPixelColor((light + 15) % 16, _color);
      RGBLED.setPixelColor((light + 14) % 16, _color);
      RGBLED.setPixelColor((light + 13) % 16, _color);
    }
  }
}

void _LED::changeAll(int red, int green, int blue) {
  for (int i = 0; i <= 15; i++) {
    RGBLED.setPixelColor(i, red, green, blue);
  }
}

void _LED::changeAll(unsigned long _color) {
  for (int i = 0; i <= 15; i++) {
    RGBLED.setPixelColor(i, _color);
  }
}
