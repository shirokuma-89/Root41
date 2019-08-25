void _ball::read(int* b) {
  for (int i = 0; i <= 15; i++) {
    *(b + i) = analogRead(BALL[i]);
  }

  speed = 100;

  if (device.robot) {
    val[6] = (val[5] * 2 + val[8]) / 3;
    val[7] = (val[8] * 2 + val[5]) / 3;
    val[14] = (val[13] + val[15]) / 2;
  } else {
    val[12] = (val[11] * 2 + val[13]) / 2;
  }

  digitalWrite(BALL_RESET, LOW);
}

void _ball::calc(void) {
  x = 0;
  y = 0;
  for (int i = 0; i <= 15; i++) {
    x += sin(radians(22.5 * i)) * val[i];
    y += cos(radians(22.5 * i)) * val[i];
  }
  deg = degrees(atan2(x, y));
  deg += 180;
  deg %= 360;

  int hoge = 0;
  for (int i = 0; i <= 15; i++) {
    if (deg <= (i + 1) * 22.5) {
      hoge = i;
      break;
    }
  }

  deg = hoge * 22.5;

  LCD.data = deg;
  LCD.unit = "DEG";
}
