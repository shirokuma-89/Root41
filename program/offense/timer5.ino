// timer5.ino

ISR(timer5Event) {
  pauseTimer5();   //割り込みを無効化
  enableMillis();  // millis()関数を有効化

  if (device.mode == 0 || device.mode == 1) {  //待機中
    device.rotaryResult = rotary.process();
    if (device.rotaryResult == DIR_CW) {
      device.rotary++;
    } else if (device.rotaryResult == DIR_CCW) {
      device.rotary--;
    }

  } else if (device.mode == 2) {  //動作中
                                  //ライン処理をここに記述
    //ライン動作中はline.flagをtrueにしておくこと
    line.read();
    if (line.stop) {
      if (millis() - line.inTimer >= line.stoptime) {
        line.stop = false;
      }
    } else if (line.touch) {
      line.flag = true;
      if (line.deg == 1000 && line.outMove == 1000) {
        line.stop = true;
        line.near = true;
        line.inTimer = millis();
        if (line.val[0]) {
          line.deg = 180;
          line.highPin = 0;
          line.logs[0] = 0;
        } else if (line.val[1]) {
          line.deg = 135;  //ゴールキーパーは65
          line.highPin = 1;
          line.logs[0] = 1;
        } else if (line.val[2]) {
          line.deg = 225;  //ゴールキーパーは295
          line.highPin = 2;
          line.logs[0] = 2;
        } else if (line.val[3]) {
          line.deg = 0;
          line.highPin = 3;
          line.logs[0] = 3;
        }
      } else if (line.deg != 1000 && line.outMove == 1000) {
        for (int i = 1; i <= 9; i++) {
          if (line.logs[i] == 5) {
            if (line.val[0]) {
              for (int j = 0; j <= i - 1; j++) {
                if (line.logs[j] == 0) {
                  break;
                }
                if (j == i - 1) {
                  line.logs[i] = 0;
                }
              }
            }
            if (line.val[1]) {
              for (int j = 0; j <= i - 1; j++) {
                if (line.logs[j] == 1) {
                  break;
                }
                if (j == i - 1) {
                  line.logs[i] = 1;
                }
              }
            }
            if (line.val[2]) {
              for (int j = 0; j <= i - 1; j++) {
                if (line.logs[j] == 2) {
                  break;
                }
                if (j == i - 1) {
                  line.logs[i] = 2;
                }
              }
            }
            if (line.val[3]) {
              for (int j = 0; j <= i - 1; j++) {
                if (line.logs[j] == 3) {
                  break;
                }
                if (j == i - 1) {
                  line.logs[i] = 3;
                }
              }
            }
          }
        }

        if (line.logs[0] == 0) {
          bool i;
          i = line.check(1);
          bool j;
          j = line.check(2);
          if (i && j == false) {
            line.deg = 150;
          } else if (j && i == false) {
            line.deg = 210;
          } else if (i & j) {
            line.deg = 180;
          }
        } else if (line.logs[0] == 1) {
          for (int i = 0; i <= 9; i++) {
            if (line.logs[i] == 0) {
              line.deg = 135;
            }
          }
          for (int i = 0; i <= 9; i++) {
            if (line.logs[i] == 3) {
              line.deg = 45;
            }
          }
          for (int i = 0; i <= 9; i++) {
            if (line.logs[i] == 0) {
              for (int j = 0; j <= 9; j++) {
                if (line.logs[j] == 3) {
                  // bool k;
                  // k = line.check(2);
                  // if (k) {
                  //   line.deg = 90;
                  // } else {
                  //   line.deg = 135;
                  // }
                  line.deg = 150;
                }
              }
            } else if (line.logs[i] == 3) {
              for (int j = 0; j <= 9; j++) {
                if (line.logs[j] == 0) {
                  // bool k;
                  // k = line.check(2);
                  // if (k) {
                  //   line.deg = 90;
                  // } else {
                  //   line.deg = 135;
                  // }
                  line.deg = 150;
                }
              }
            }
          }
        } else if (line.logs[0] == 2) {
          for (int i = 0; i <= 9; i++) {
            if (line.logs[i] == 0) {
              line.deg = 225;
            }
          }
          for (int i = 0; i <= 9; i++) {
            if (line.logs[i] == 3) {
              line.deg = 315;
            }
          }
          for (int i = 0; i <= 9; i++) {
            if (line.logs[i] == 0) {
              for (int j = 0; j <= 9; j++) {
                if (line.logs[j] == 3) {
                  // bool k;
                  // k = line.check(2);
                  // if (k) {
                  //   line.deg = 270;
                  // } else {
                  //   line.deg = 225;
                  // }
                  line.deg = 210;
                }
              }
            } else if (line.logs[i] == 3) {
              for (int j = 0; j <= 9; j++) {
                if (line.logs[j] == 0) {
                  // bool k;
                  // k = line.check(2);
                  // if (k) {
                  //   line.deg = 270;
                  // } else {
                  //   line.deg = 225;
                  // }
                  line.deg = 210;
                }
              }
            }
          }
        } else if (line.logs[0] == 3) {
          bool i;
          i = line.check(1);
          bool j;
          j = line.check(2);
          if (i && j == false) {
            line.deg = 30;
          } else if (j && i == false) {
            line.deg = 330;
          } else if (i && j) {
            line.deg = 0;
          }
        }
      } else if (line.deg == 1000 && line.outMove != 1000) {
        line.deg = line.outMove;
        line.outMove = 1000;
      }
    } else if (line.flag && line.outMove == 1000) {
      line.outMove = line.deg;
      line.deg = 1000;
      line.outTimer = millis();
    } else if (line.flag && line.outMove != 1000) {
      int i;
      if (line.logs[0] == 0 || line.logs[0] == 3) {
        i = 800;
      } else {
        i = 400;
      }
      if (millis() - line.outTimer >= 400) {
        line.flag = false;
        line.outMove = 1000;
      }
    } else {
      line.flag = false;
      line.deg = 1000;
      line.outMove = 1000;

      for (int i = 0; i <= 9; i++) {
        line.logs[i] = 5;
      }
    }
  }

  //   motor.restart++;

  //   if (motor.restart >= 2000) {
  //     motor.restart = 0;
  //     for (int i = 0; i <= 100; i++) {
  //       digitalWrite(4, LOW);
  //       digitalWrite(5, LOW);
  //       digitalWrite(6, LOW);
  //       digitalWrite(7, LOW);
  //       digitalWrite(8, LOW);
  //       digitalWrite(9, LOW);
  //       digitalWrite(10, LOW);
  //       digitalWrite(11, LOW);
  //       digitalWrite(12, LOW);
  //     }
  //     motor.directDrive(motor.val);
  //   }
  // }
  // if (device.mode != 2 || !device.keeper) {
  startTimer5(50);  //タイマー割り込みを有効化
  // }
}