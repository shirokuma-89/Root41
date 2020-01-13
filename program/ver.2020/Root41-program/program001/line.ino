ISR(timer5Event) {
  line.read();
}

void _line::process(void) {
  if (line.flag) {
    ball.lineOut = millis();
    if (line.mode == 1 && line.touch) {
      ball._right = ball.right;
      //通常
      for (int i = 0; i <= 19; i++) {
        if (line.logs[i] == 1 &&
            line.whited <= carpet._lineWhited[carpet.tile]) {
          line.x += line.vector[i][0] * 1.5;
          line.y += line.vector[i][1];
          if (line.first == i) {
            line.x *= 3;
            line.y *= 3;
          }
          line.logs[i] = 2;
        }
      }
      line.deg = atan2(line.x, line.y);
      line.deg = degrees(line.deg);
      if (line.deg < 180) {
        line.deg += 180;
      } else {
        line.deg -= 180;
      }
      if (line.deg <= 45 || line.deg <= 315) {
        line.direction = 1;
      } else if (line.deg <= 135) {
        line.direction = 2;
      } else if (line.direction <= 225) {
        line.direction = 3;
      } else {
        line.direction = 4;
      }
      if (millis() - line.stopTimer <= 50) {
        line.deg = 1000;
      } else if (millis() - line.stopTimer >= 2000) {
        line.deg = atan2(0, line.y);
        line.deg = degrees(line.deg);
        if (line.deg < 180) {
          line.deg += 180;
        } else {
          line.deg -= 180;
        }
      }
    } else if (line.mode == 1 && !line.touch) {
      line.overTimer = millis();
      if (line.deg == 1000) {
        line.deg = atan2(line.x, line.y);
        line.deg = degrees(line.deg);
        if (line.deg < 180) {
          line.deg += 180;
        } else {
          line.deg -= 180;
        }
      }
      line.mode = 2;
    } else if (line.mode == 2) {
      if (millis() - line.overTimer >=
          line.whited * carpet._lineWhitedT[carpet.tile]) {  //カーペットクラスを参照
        if (line.whited <= 10) {
          if (abs(line.deg - ball.top * 22.5) >= 40 ||
              abs(line.deg - ball.top * 22.5) <= 320) {
            if (first >= 2 && first <= 7) {
              line.lock = 2;
              line.lockTimer = millis();
            } else if (first >= 12 && first <= 18) {
              line.lock = 1;
              line.lockTimer = millis();
            }
          }
          line.flag = false;
          line.mode = 0;
        } else {
          line.flag = false;
          line.mode = 0;
        }
      }
    }
    line._deg = (round(line.deg) + 180) % 360;
  } else {
    gyro.offset = 0;
    line.flag = false;
    line.deg = 1000;
    line.autodeg = 1000;
    line.direction = 0;
    line.first = 100;
    line.last = 100;
    line.x = 0;
    line.y = 0;
    line.mode = 0;
    line.whited = 0;
    for (int i = 0; i <= 19; i++) {
      line.logs[i] = 0;
    }
    line.stopTimer = 0;
    line.overTimer = 0;
  }
}

_line::_line(void) {
  for (int i = 0; i <= 19; i++) {
    val[i] = false;
  }
}

void _line::read(void) {
  touch = false;
  for (int i = 0; i <= 19; i++) {
    if (!digitalRead(LINE[i])) {
      if (logs[i] == 0) {
        now = i;
      }
      if (logs[i] == 0) {
        whited++;
        logs[i] = 1;
      }
      if (!flag) {
        sigdeg = gyro.deg;
        first = i;
        motor.integral = 0;
        gyro.offset = -(line.sigdeg);
        stopTimer = millis();
      }
      if (!val[i]) {
        last = i;
      }
      val[i] = true;
      flag = true;
      touch = true;
      mode = 1;
    } else {
      val[i] = false;
    }
  }
}
