ISR(timer5Event) {
  line.read();
}

void _line::process(void) {
  if (line.flag) {
    if (line.touch) {
      //通常
      for (int i = 0; i <= 19; i++) {
        if (line.logs[i] == 1 && line.whited <= 14) {
          line.whited++;
          line.x += line.vector[i][0];
          line.y += line.vector[i][1];
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
      line.deg -= line.gapdeg;
      if (line.deg > 360) {
        line.deg -= 360;
      } else if (line.deg < 0) {
        line.deg += 360;
      }
      if (millis() - line.stopTimer <= 100) {
        line.deg = 1000;
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
      if (millis() - line.overTimer >= 400) {
        // if (first >= 3 && first <= 6) {
        //   lockside = 2;
        //   lockTimer = millis();
        // } else if (first >= 13 && first <= 16) {
        //   lockside = 1;
        //   lockTimer = millis();
        // }
        line.flag = false;
        line.mode = 0;
      }
    }
    line._deg = (round(line.deg) + 180) % 360;
  } else {
    gyro.offset = 0;
    line.flag = false;
    line.deg = 1000;
    line.gapdeg = 1000;
    line.first = 100;
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
        logs[i] = 1;
      }
      if (!flag) {
        sigdeg = gyro.deg;
        if (gyro.deg <= 180) {
          gapdeg = gyro.deg;
        } else {
          gapdeg = gyro.deg - 360;
        }
        first = i;
        motor.integral = 0;
        gyro.offset = -(line.sigdeg);
        stopTimer = millis();
        mode = 1;
      }
      val[i] = true;
      flag = true;
      touch = true;
    } else {
      val[i] = false;
    }
  }
}
