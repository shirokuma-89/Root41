void _LED::gyroShow(void) {
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
    if (!device.robot) {
      RGBLED.setPixelColor(light, 255, 0, 0);
      RGBLED.setPixelColor((light + 1) % 16, 255, 0, 0);
      RGBLED.setPixelColor((light + 2) % 16, 255, 0, 0);
      RGBLED.setPixelColor((light + 15) % 16, 255, 0, 0);
      RGBLED.setPixelColor((light + 14) % 16, 255, 0, 0);
      RGBLED.setPixelColor((light + 13) % 16, 255, 0, 0);
    } else {
      RGBLED.setPixelColor(light, 0, 0, 255);
      RGBLED.setPixelColor((light + 1) % 16, 0, 0, 255);
      RGBLED.setPixelColor((light + 2) % 16, 0, 0, 255);
      RGBLED.setPixelColor((light + 15) % 16, 0, 0, 255);
      RGBLED.setPixelColor((light + 14) % 16, 0, 0, 255);
      RGBLED.setPixelColor((light + 13) % 16, 0, 0, 255);
    }
  }
}

void _LED::changeAll(int red, int green, int blue) {
  for (int i = 0; i <= 15; i++) {
    RGBLED.setPixelColor(i, red, green, blue);
  }
}
