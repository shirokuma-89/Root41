void _line::process(void) {
  if (line.flag) {
    if (line.mode == 1 && line.touch) {
      //通常
    } else if (line.mode == 1 && !line.touch) {
      //離脱時
    } else if (line.mode == 2) {
      //マージン
      if (millis() - line.overTimer >= 500) {
        line.flag = false;
        line.deg = 1000;
        line.mode = 0;
      }
    } else if (line.mode == 3) {
      //オーバー
      if (millis() - line.overTimer >= 500) {
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
      line.logs[i] = false;
    }
    for (int i = 0; i <= 19; i++) {
      line.order[i] = 100;
    }
    stopTimer = 0;
    overTimer = 0;
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
      if (!val[i]) {
        whited++;
      }
      if (!flag) {
        stopTimer = millis();
      }
      val[i] = true;
      touch = true;
      flag = true;
      mode = 1;
    } else {
      val[i] = false;
    }
  }
  for (int i = 0; i <= 19; i++) {
    if (val[i]) {
      for (int j = 0; j <= 19; j++) {
        if (order[j] != i) {
          if (j == 19) {
            newv = i;
            for (int k = 18; k >= 0; k--) {
              order[k] = order[k + 1];
            }
            order[0] = newv;
          }
        } else {
          break;
        }
      }
    }
  }
  for (int i = 0; i <= 19; i++) {
    if (val[i]) {
      logs[i] = true;
    }
  }
}
