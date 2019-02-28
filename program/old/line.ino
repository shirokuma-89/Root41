//初期化
_line::_line(void) {
  for (int i = 0; i <= 3; i++) {
    val[i] = false;
  }
}

//読み取り
void _line::read(void) {
  on = false;

  val[0] = !digitalRead(LINE1);
  val[1] = !digitalRead(LINE2);
  val[2] = !digitalRead(LINE3);
  val[3] = !digitalRead(LINE4);

  if (val[0]) {
    on = true;
  }
  if (val[1]) {
    on = true;
  }
  if (val[2]) {
    on = true;
  }
  if (val[3]) {
    on = true;
  }

  if (show && on) {
    if (val[0]) {
      RGBLED.setPixelColor(0, 255, 0, 0);
      RGBLED.setPixelColor(1, 255, 0, 0);
      RGBLED.setPixelColor(2, 255, 0, 0);
      RGBLED.setPixelColor(15, 255, 0, 0);
      RGBLED.setPixelColor(14, 255, 0, 0);
    }
    if (val[1]) {
      RGBLED.setPixelColor(10, 255, 0, 0);
      RGBLED.setPixelColor(11, 255, 0, 0);
      RGBLED.setPixelColor(12, 255, 0, 0);
      RGBLED.setPixelColor(13, 255, 0, 0);
      RGBLED.setPixelColor(14, 255, 0, 0);
    }
    if (val[2]) {
      RGBLED.setPixelColor(2, 255, 0, 0);
      RGBLED.setPixelColor(3, 255, 0, 0);
      RGBLED.setPixelColor(4, 255, 0, 0);
      RGBLED.setPixelColor(5, 255, 0, 0);
      RGBLED.setPixelColor(6, 255, 0, 0);
    }
    if (val[3]) {
      RGBLED.setPixelColor(6, 255, 0, 0);
      RGBLED.setPixelColor(7, 255, 0, 0);
      RGBLED.setPixelColor(8, 255, 0, 0);
      RGBLED.setPixelColor(9, 255, 0, 0);
      RGBLED.setPixelColor(10, 255, 0, 0);
    }
  }
}

//LED表示
void _line::showLED(void) {
  for (int i = 0; i <= 15; i++) {
    RGBLED.setPixelColor(i, 255, 255, 0);
  }
  if (show) {
    if (val[0]) {
      RGBLED.setPixelColor(0, 255, 0, 0);
      RGBLED.setPixelColor(1, 255, 0, 0);
      RGBLED.setPixelColor(2, 255, 0, 0);
      RGBLED.setPixelColor(15, 255, 0, 0);
      RGBLED.setPixelColor(14, 255, 0, 0);
    }
    if (val[1]) {
      RGBLED.setPixelColor(10, 255, 0, 0);
      RGBLED.setPixelColor(11, 255, 0, 0);
      RGBLED.setPixelColor(12, 255, 0, 0);
      RGBLED.setPixelColor(13, 255, 0, 0);
      RGBLED.setPixelColor(14, 255, 0, 0);
    }
    if (val[2]) {
      RGBLED.setPixelColor(2, 255, 0, 0);
      RGBLED.setPixelColor(3, 255, 0, 0);
      RGBLED.setPixelColor(4, 255, 0, 0);
      RGBLED.setPixelColor(5, 255, 0, 0);
      RGBLED.setPixelColor(6, 255, 0, 0);
    }
    if (val[3]) {
      RGBLED.setPixelColor(6, 255, 0, 0);
      RGBLED.setPixelColor(7, 255, 0, 0);
      RGBLED.setPixelColor(8, 255, 0, 0);
      RGBLED.setPixelColor(9, 255, 0, 0);
      RGBLED.setPixelColor(10, 255, 0, 0);
    }
  }
}