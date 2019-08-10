ISR(timer5Event) {
  // pauseTimer5();           //割り込みを無効化
  enableMillis();          // millis()関数を有効化
  if (device.mode == 1) {  //動作中
    line.flag = false;
    line.num = 0;
    for (int i = 0; i <= 19; i++) {
      line.logs[i] = 0;
    }
    for (int i = 0; i <= 19; i++) {
      if (!digitalRead(LINE[i])) {
        line.val[i] = true;
        line.flag = true;
        line.logs[line.num] = i;
      } else {
        line.val[i] = false;
      }
    }
  }

  // startTimer5(50);  //タイマー割り込みを有効化
}

void _line::process(void) {
  flag = false;
  _flag = false;
  num = 0;
  for (int i = 0; i <= 19; i++) {
    logs[i] = 1000;
  }

  for (int i = 0; i <= 19; i++) {
    if (!digitalRead(LINE[i])) {
      val[i] = true;
      _flag = true;

      logs[num] = i;
      num++;
    } else {
      val[i] = false;
    }
  }

  if (_flag) {
    count++;
  } else if (!flag) {
    count = 0;
  }

  if (count >= 5) {
    flag = true;
  }

  if (flag) {
    if (num != 1) {
      space = 0;
      for (int i = 0; i < 20; i++) {
        if (logs[i + 1] != 1000) {
          if (logs[i + 1] - logs[i] >= space) {
            space = logs[i + 1] - logs[i];
            number = logs[i] + (logs[i + 1] - logs[i]) / 2;
          }
        }
      }
      if ((logs[0] + 20) - logs[num - 1] >= space) {
        space = (logs[0] + 20) - logs[num - 1];
        number = logs[num - 1] + ((logs[0] + 20) - logs[num - 1]) / 2;
      }
    }
  }
}

_line::_line(void) {
  for (int i = 0; i <= 19; i++) {
    val[i] = false;
  }
}

void _line::read(void) {
  //ほげほげ
}
