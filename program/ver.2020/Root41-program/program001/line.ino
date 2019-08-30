ISR(timer5Event) {
  enableMillis();
  line.read();
  line.process();
  startTimer5(1000);
}

void _line::process(void) {
  if (line.flag) {
    if (line.mode == 1 && line.touch) {
      //通常
      line.deg = atan2(line.x, line.y);
      line.deg = line.deg / 0.0174533;
      if (line.deg < 180) {
        line.deg += 180;
      } else {
        line.deg -= 180;
      }
      if (line._millis - line.stopTimer <= 0) {
        line.deg = 1000;
      }
    } else if (line.mode == 1 && !line.touch) {
      //離脱時
      if (abs(line.deg - line.last * 18) <= 90 || abs(line.deg - line.last * 18) >= 270) {
        line.mode = 3;
      } else {
        line.mode = 2;
      }
      line.overTimer = line._millis;
    } else if (line.mode == 2) {
      //マージン
      if (line._millis - line.overTimer >= 300) {
        line.flag = false;
        line.deg = 1000;
        line.mode = 0;
      }
    } else if (line.mode == 3) {
      //オーバー
      if (line._millis - line.overTimer >= 3000) {
        line.flag = false;
        line.deg = 1000;
        line.mode = 0;
      }
    }
  } else {
    line.flag = false;
    line.deg = 1000;
    line.x = 0;
    line.y = 0;
    line.mode = 0;
    line.whited = 0;
    for (int i = 0; i <= 19; i++) {
      logs[i] = false;
    }
    for (int i = 0; i <= 19; i++) {
      order[i] = 100;
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
  line.touch = false;

  for (int i = 0; i <= 19; i++) {
    if (!digitalRead(LINE[i])) {
      if (!logs[i]) {
        logs[i] = true;
        line.whited++;
        if (line.whited <= 10) {
          line.x += plus[i][0];
          line.y += plus[i][1];
        }
      }
      if (!line.flag) {
        line.stopTimer = line._millis;
      }
      val[i] = true;
      line.last = i;
      line.touch = true;
      line.flag = true;
      line.mode = 1;
    } else {
      val[i] = false;
    }
  }
  for (int i = 0; i <= 19; i++) {
    if (val[i]) {
      for (int j = 0; j <= 19; j++) {
        if (order[j] != i) {
          if (j == 19) {
            line.newv = i;
            for (int k = 18; k >= 0; k--) {
              order[k] = order[k + 1];
            }
            order[0] = line.newv;
          }
        } else {
          break;
        }
      }
    }
  }
}
