// LED.ino

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

  RGBLED.setPixelColor(light, 0, 50, 255);
  RGBLED.setPixelColor((light + 1) % 16, 0, 50, 255);
  RGBLED.setPixelColor((light + 15) % 16, 0, 50, 255);
  RGBLED.setPixelColor((light + 2) % 16, 0, 50, 255);
  RGBLED.setPixelColor((light + 14) % 16, 0, 50, 255);
}

void _LED::lineShow(void) {
  for (int i = 0; i <= 15; i++) {
    RGBLED.setPixelColor(i, 255, 255, 0);
  }
  if (line.val[0]) {
    RGBLED.setPixelColor(0, 255, 0, 0);
    RGBLED.setPixelColor(1, 255, 0, 0);
    RGBLED.setPixelColor(2, 255, 0, 0);
    RGBLED.setPixelColor(15, 255, 0, 0);
    RGBLED.setPixelColor(14, 255, 0, 0);
  }
  if (line.val[1]) {
    RGBLED.setPixelColor(10, 255, 0, 0);
    RGBLED.setPixelColor(11, 255, 0, 0);
    RGBLED.setPixelColor(12, 255, 0, 0);
    RGBLED.setPixelColor(13, 255, 0, 0);
    RGBLED.setPixelColor(14, 255, 0, 0);
  }
  if (line.val[2]) {
    RGBLED.setPixelColor(2, 255, 0, 0);
    RGBLED.setPixelColor(3, 255, 0, 0);
    RGBLED.setPixelColor(4, 255, 0, 0);
    RGBLED.setPixelColor(5, 255, 0, 0);
    RGBLED.setPixelColor(6, 255, 0, 0);
  }
  if (line.val[3]) {
    RGBLED.setPixelColor(6, 255, 0, 0);
    RGBLED.setPixelColor(7, 255, 0, 0);
    RGBLED.setPixelColor(8, 255, 0, 0);
    RGBLED.setPixelColor(9, 255, 0, 0);
    RGBLED.setPixelColor(10, 255, 0, 0);
  }
}