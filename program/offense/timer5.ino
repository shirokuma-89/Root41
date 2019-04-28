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
    } else if (line.touch) {
      line.flag = true;

      //触ってるとき
      if (line.deg == 1000 && line.outMove == 1000) {
        //初めて
        line.stop = true;
        line.near = true;
        line.inTimer = millis();
        line.holdTimer = millis();
        if (ball.exist == false) {
          line.deg = 1000;
        } else if (motor.memory <= 180) {
          line.deg = motor.memory + 180;
        } else if (motor.memory > 180) {
          line.deg = motor.memory - 180;
        }
        if (line.val[0]) {
          line.step = 180;
          line.highPin = 0;
          line.logs[0] = 0;
        } else if (line.val[1]) {
          line.step = 120;  //ゴールキーパーは65
          line.highPin = 1;
          line.logs[0] = 1;
        } else if (line.val[2]) {
          line.step = 240;  //ゴールキーパーは295
          line.highPin = 2;
          line.logs[0] = 2;
        } else if (line.val[3]) {
          line.step = 0;
          line.highPin = 3;
          line.logs[0] = 3;
        }
        if (line.deg >= 180) {
          line.deg = map(line.deg, 180, 360, -180, 0);
        }
        if (line.step >= 180) {
          line.step = map(line.step, 180, 360, -180, 0);
        }
        //一時的にline.degとline.stepを-180~180にする
        if (line.deg == 1000) {
          line.deg = line.step;
        } else if (abs(line.deg - line.step) <= 70) {
          line.deg = (line.deg + line.step) / 2;
        } else {
          if (line.highPin == 1 || line.highPin == 2) {
            line.deg = (line.deg + line.step) / 2;
            if (line.highPin == 1) {
              if (line.deg <= 0) {
                line.deg += 180;
              }
            } else {
              if (line.deg >= 0) {
                line.deg -= 180;
              }
            }
          } else if (line.highPin == 0) {
            if (line.deg >= 0) {
              line.deg = 150;
            } else {
              line.deg = 210;
            }
          } else if (line.highPin == 3) {
            if (ball.turn == 0) {
              line.deg = (line.deg + line.step) / 2;
            } else {
              line.deg = 0;
            }
          }
        }
        //戻す
        if (line.deg <= 0) {
          line.deg = map(line.deg, 0, -180, 360, 180);
        }
        if (line.step <= 0) {
          line.step = map(line.step, 0, -180, 360, 180);
        }
      } else if (line.deg != 1000 && line.outMove == 1000) {
        //連続
        if (line.highPin == 1 || line.highPin == 2) {
          if (line.logs[1] == 0) {
            if (line.logs[2] == 3) {
              if (line.highPin == 1) {
                line.deg = 135;
              } else if (line.highPin == 2) {
                line.deg = 225;
              }
            } else {
              if (line.highPin == 1) {
                line.deg = 90;
              } else if (line.highPin == 2) {
                line.deg = 270;
              }
            }
          }
          if (line.logs[1] == 0) {
            if (line.logs[2] == 3) {
              if (line.highPin == 1) {
                line.deg = 45;
              } else if (line.highPin == 2) {
                line.deg = 315;
              }
            } else {
              if (line.highPin == 1) {
                line.deg = 90;
              } else if (line.highPin == 2) {
                line.deg = 270;
              }
            }
          }
          if (millis() - line.holdTimer >= 2000) {
            if (line.logs[1] == 0) {
              line.deg = 180;
            } else if (line.logs[1] == 3) {
              line.deg = 0;
            }
          }
        }
        if (line.highPin == 0) {
          if (millis() - line.holdTimer >= 2000) {
            line.deg = 180;
          }
        }
        if (line.highPin == 3) {
          if (line.logs[1] == 1 && line.logs[2] == 0) {
            line.deg = 50;
          } else if (line.logs[1] == 2 && line.logs[2] == 0) {
            line.deg = 330;
          } else {
            line.deg = 0;
          }
          if (millis() - line.holdTimer >= 2000) {
            line.deg = 0;
          }
        }
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
      if (line.outMove <= 60 || line.outMove >= 300) {
        i = 400;
      } else {
        i = 300;
      }
      if (millis() - line.outTimer >= i) {
        line.flag = false;
        line.outMove = 1000;
      }
    } else {
      line.flag = false;
      line.deg = 1000;
      line.outMove = 1000;
      line.step = 1000;
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
  // }
}