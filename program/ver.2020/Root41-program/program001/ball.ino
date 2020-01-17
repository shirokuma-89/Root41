void _ball::read(int* b) {
  digitalWrite(BALL_RESET, HIGH);
  for (int i = 0; i <= 15; i++) {
    *(b + i) += (1 - LPF) * (analogRead(BALL[i]) - *(b + i));
  }

  if (device.robot) {
    val[15] *= 0.95;
    val[13] *= 0.95;
  } else {
    val[7] = (val[6] + val[9] + val[6]) / 3.2;
    val[8] = (val[6] + val[9] + val[9]) / 3.2;
    // val[8] *= 100;
  }

  digitalWrite(BALL_RESET, LOW);
}

void _ball::calc(void) {
  speed = 100;
  top = 0;
  for (int i = 0; i <= 15; i++) {
    if (val[i] <= val[top]) {
      top = i;
    }
  }

  deg = top * 22.5;
  _deg = deg;

  turn = false;
  int turnVal = 50;
  if (top > 1 && top < 15 && millis() - holdTimer >= 200) {
    if (val[top] <= 250) {
      turn = true;
      if (deg >= 180) {
        deg -= turnVal;
      } else {
        deg += turnVal;
      }
    }
  }

  emg = false;
  if (top > 2 && top < 14 && millis() - holdTimer >= 200) {
    if (val[top] < 210) {
      emg = true;
      if (top >= 8) {
        deg -= 45;
      } else {
        deg += 45;
      }
    }
  }

  if ((top <= 3 || top >= 13) && digitalRead(BALL_HOLD) && val[top] <= 260) {
    deg = 0;
    hold = true;
    holdTimer = millis();
  } else {
    hold = false;
  }

  if (top == 0) {
    holdTimer = millis() + 50;
  }

  LCD.data = ball.top;
  LCD.unit = "deg";

  exist = true;
  if (val[top] <= 600) {
    exCount = 0;
  } else {
    exCount++;
  }

  if (exCount >= 3) {
    exist = false;
  }
  if (ball.val[ball.top] >= 400) {
    ball.deg = ball.top * 22.5;
  }

  if (line._deg != 1000 && millis() - ball.lineOut <= 2000) {
    if (abs(line._deg - _deg) >= 300 || abs(line._deg - _deg) <= 60) {
      exist = false;
    } else {
      line._deg = 1000;
    }
  } else {
    line._deg = 1000;
  }
}

void _ball::keeper(void) {
  speed = 100;

  top = 0;
  for (int i = 0; i <= 15; i++) {
    if (val[i] <= val[top]) {
      top = i;
    }
  }

  x = 0;
  y = 0;

  exist = true;

  deg = top * 22.5;
  deg = constrain(deg, -100, 100);

  speed = 120;

  x -= val[1] + val[2] + val[3] + val[4];
  if (device.robot) {
    x += (val[15] + val[14] + val[13] + val[12]) * 1.3;
  } else {
    x += (val[15] + val[14] + val[13] + val[12]) * 0.95;
  }

  // x -= _val[1] + _val[2] + _val[3] + _val[4];
  // x += (_val[15] + _val[14] + _val[13] + _val[12]) * 0.95;
  if (x >= 0) {
    deg = 90;
    right = 1;
  } else {
    deg = 270;
    right = -1;
  }

  if (top == 0 && abs(x) <= 100) {
    exist = false;
    right = 0;
  }

  if (millis() - keeperOut <= 1000 && _right == right) {
    exist = false;
  }

  if (tof.dist >= 500) {
    deg = 180;
    exist = true;
  }

  turn = false;
  emg = false;

  // if (top <= 2 || top >= 14 && tof.dist <= 550) {
  // } else {
  //   device.keeperTimer1 = millis();
  // }

  // if ((top <= 2 || top >= 14) && digitalRead(BALL_HOLD) && tof.dist <= 550) {
  //   device.keeperTimer1 = millis() - 2100;
  // }

  for (int i = 0; i <= 15; i++) {
    _val[i] = val[i];
  }
}
