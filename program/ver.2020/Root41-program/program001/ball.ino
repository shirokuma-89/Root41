void _ball::read(int* b) {
  digitalWrite(BALL_RESET, HIGH);
  for (int i = 0; i <= 15; i++) {
    *(b + i) = analogRead(BALL[i]);
  }

  if (device.robot) {  // error:378
    val[8] = (val[6] + val[9]) / 2;
    val[7] = val[8];
  } else {
    val[12] = (val[11] * 2 + val[14]) / 3;
    val[13] = (val[11] + val[14] * 2) / 3;
    val[8] = (val[6] + val[9]) / 2;
    val[7] = val[8];
  }

  // val[0] *= 0.95;

  digitalWrite(BALL_RESET, LOW);
}

void _ball::calc(void) {
  speed = 100;
  top = 0;
  val[6] = 1000;
  for (int i = 0; i <= 15; i++) {
    if (val[i] <= val[top]) {
      top = i;
    }
  }

  deg = top * 22.5;
  _deg = deg;

  dist = 0;
  for (int i = 0; i < 16; i++) {
    if (val[i] <= 400) {
      dist++;
    }
  }
  dist = constrain(dist, 3, 5);

  // if (top > 1 && top < 15) {
  if (deg >= 180) {
    deg -= 360;
  }
  deg = round((float)deg * (float)sqrt(abs(deg)) * (float)0.11);
  deg += 720;
  deg %= 360;
  // }

  turn = false;
  int turnVal = 58;
  if (top == 1) {
    if (val[2] >= val[0]) {
      goto TURN_PROCESS;
    }
  }
  if (top == 15) {
    if (val[14] >= val[0]) {
      goto TURN_PROCESS;
    }
  }
  if (top > 0 && top < 16 && millis() - holdTimer >= 200) {
  TURN_PROCESS:
    if (val[top] <= 270) {
      turn = true;
      if (deg >= 180) {
        deg -= turnVal;
        speed = 55;
      } else {
        deg += turnVal;
        speed = 55;
      }
    }
  }

  emg = false;
  if (top > 6 && top < 10) {
    if (val[top] <= 256) {
      emg = true;
      speed -= 20;
      if (top >= 8) {
        deg -= 35;
      } else {
        deg += 35;
      }
    }
  }
  if ((top <= 2 || top >= 14) && digitalRead(BALL_HOLD)) {
    deg = 0;
    hold = true;
    holdTimer = millis();
  } else {
    hold = false;
  }

  if (top == 0) {
    holdTimer = millis() + 50;
  }

  // if (ball.top >= 1 && ball.top <= 5) {
  //   ball.speed -= 20;
  // } else if (ball.top <= 15 && ball.top >= 11) {
  //   ball.speed -= 20;
  // }

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

  // if (line._deg != 1000 && millis() - ball.lineOut <= 2000) {
  //   if (abs(line._deg - _deg) >= 300 || abs(line._deg - _deg) <= 60) {
  //     exist = false;
  //   } else {
  //     line._deg = 1000;
  //   }
  // } else {
  //   line._deg = 1000;
  // }
}

void _ball::keeper(void) {
  speed = 100;

  top = 0;
  for (int i = 0; i <= 15; i++) {
    if (val[i] <= val[top]) {
      top = i;
    }
  }

  if (val[top] > 600) {
    exist = false;
  } else {
    exist = true;
  }
  if (top <= 5 || top >= 11) {
    if (top <= 8) {
      if (tof.dist >= 200) {
        deg = 113;
      } else if (tof.dist <= 250) {
        deg = 68;
      } else {
        deg = 90;
      }
    } else {
      if (tof.dist >= 200) {
        deg = 248;
      } else if (tof.dist <= 250) {
        deg = 293;
      } else {
        deg = 270;
      }
    }
  } else {
    exist = false;
  }

  if (tof.dist >= 500) {
    deg = 180;
    exist = true;
  }
}
