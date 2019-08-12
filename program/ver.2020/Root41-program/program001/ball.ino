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
  top = 0;

  for (int i = 0; i <= 15; i++) {
    if (val[top] >= val[i]) {
      top = i;
    }
  }

  dist = 0;
  for (int i = 0; i <= 15; i++) {
    if (val[i] <= 420) {
      dist++;
    }
  }

  _top = top;

  // Serial.println(dist);

  if (top > 1 && top < 15) {
    if (val[top] <= 280 || (top <= 9 && top >= 7 && val[8] <= 310)) {
      if (top <= 8) {
        _top += 4;
      } else {
        _top -= 4;
      }
    }
  }

  deg = _top * 22.5;
  Serial.println(top);
}
