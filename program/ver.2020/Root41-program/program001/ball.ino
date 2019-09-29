void _ball::read(int* b) {
  digitalWrite(BALL_RESET, HIGH);
  for (int i = 0; i <= 15; i++) {
    *(b + i) = analogRead(BALL[i]);
  }

  if (device.robot) {
    val[6] = (val[5] * 2 + val[8]) / 3;
    val[7] = (val[8] * 2 + val[5]) / 3;
    val[14] = (val[13] + val[15]) / 2;
  } else {
    val[12] = (val[11] * 2 + val[14]) / 3;
    val[13] = (val[11] + val[14] * 2) / 3;
    val[8] = (val[7] + val[9]) / 2;
  }

  val[0] *= 0.95;

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
  deg = round((float)deg * (float)sqrt(abs(deg)) * (float)0.106);
  deg += 720;
  deg %= 360;
  // }

  turn = false;
  int turnVal = 40;
  if (top > 0 && top < 16) {
  TURN_PROCESS:
    if (val[top] <= 257) {
      turn = true;
      if (deg >= 180) {
        if (val[top] <= 245) {
          deg -= turnVal * 0.7;
        }
        deg -= turnVal;
        if (top >= 13) {
          if (millis() - holdTimer <= 100) {
            deg += turnVal;  //打消し
            turn = false;
          } else {
            // speed = 35;
            deg -= 25;
          }
        } else if (top >= 10) {
          // speed = 60;
        }
      } else {
        if (val[top] <= 245) {
          deg += turnVal * 0.7;
        }
        deg += turnVal;
        if (top <= 3) {
          if (millis() - holdTimer <= 100) {
            deg -= turnVal;  //打消し
            turn = false;
          } else {
            // speed = 35;
            deg += 25;
          }
        } else if (top <= 6) {
          // speed = 60;
        }
      }
    }
  }

  emg = false;
  if (top > 6 && top < 10) {
    if (val[top] <= 259) {
      emg = true;
      if (top >= 8) {
        deg -= 50;
      } else {
        deg += 50;
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
    holdTimer = millis();
  }

  LCD.data = line.deg;
  LCD.unit = "DEG";

  exist = true;
  if (val[top] <= 600) {
    exCount = 0;
  } else {
    exCount++;
  }

  if (exCount >= 3) {
    exist = false;
  }
}
