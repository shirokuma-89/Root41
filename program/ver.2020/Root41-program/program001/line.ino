void _line::process(void) {
  if (flag) {
    if (mode == 1 && touch) {
      //通常

    } else if (mode == 1 && !touch) {
      //離脱時
      mode = 2;
      overTimer = millis();
    } else if (mode == 2) {
      //マージン
      if (millis() - overTimer >= 500) {
        flag = false;
        deg = 1000;
        mode = 0;
      }
    } else if (mode == 3) {
      //オーバー
      if (millis() - overTimer >= 500) {
        flag = false;
        deg = 1000;
        mode = 0;
      }
    }
  } else {
    flag = false;
    deg = 1000;
    x = 0;
    y = 0;
    mode = 0;
    whited = 0;
    for (int i = 0; i <= 19; i++) {
      logs[i] = false;
    }
    for (int i = 0; i <= 19; i++) {
      order[i] = 100;
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
      if (!logs[i]) {
        whited++;
        if (whited <= 8) {
          x += plus[i][0];
          y += plus[i][1];
        }
      }
    }
    if (!flag) {
      stopTimer = millis();
    }
    val[i] = true;
    touch = true;
    flag = true;
    mode = 1;
  }
  else {
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
