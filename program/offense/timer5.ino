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
    // line.count += 1;
    if (line.stop) {
      if (millis() - line.inTimer >= 50) {
        line.stop = false;
      }
    } else if (line.touch) {
      if (line.count[0] >= 100 || line.count[1] >= 100 ||
          line.count[2] >= 100 || line.count[3] >= 100 ||) {
        line.flag = true;
        if (line.deg == 1000 && line.outMove == 1000) {
          // first
          line.near = true;
          line.inTimer = millis();
          if (line.val[0]) {
            line.deg = 180;
            line.highPin = 0;
            line.first = 0;
          } else if (line.val[1]) {
            line.deg = 90;
            line.highPin = 1;
            line.first = -5;
          } else if (line.val[2]) {
            line.deg = 270;
            line.highPin = 2;
            line.first = 5;
          } else if (line.val[3]) {
            line.deg = 0;
            line.highPin = 3;
            line.first = 10;
          }
          if (line.highPin == 1 || line.highPin == 2) {
            line.stop = true;
          }
        } else if (line.deg != 1000 && line.outMove == 1000) {
          if (line.second == 100) {
            if (line.val[0] && line.first != 0) {
              line.second = 0;
            } else if (line.val[1] && line.first != -5) {
              line.second = -5;
            } else if (line.val[2] && line.first != 5) {
              line.second = 5;
            } else if (line.val[3] && line.first != 10 &&
                       line.count[3] >= 100) {
              line.second = 10;
            }
          } else if (line.third == 100) {
            if (line.val[0] && line.first != 0 && line.second != 0) {
              line.third = 0;
            } else if (line.val[1] && line.first != -5 && line.second != -5) {
              line.third = -5;
            } else if (line.val[2] && line.first != 5 && line.second != 5) {
              line.third = 5;
            } else if (line.val[3] && line.first != 10 && line.second != 10 &&
                       line.count[3] >= 100) {
              line.third = 10;
            }
          } else if (line.forth == 100) {
            if (line.val[0] && line.first != 0 && line.second != 0 &&
                line.third != 0) {
              line.forth = 0;
            } else if (line.val[1] && line.first != -5 && line.second != -5 &&
                       line.third != -5) {
              line.forth = -5;
            } else if (line.val[2] && line.first != 5 && line.second != 5 &&
                       line.third != 5) {
              line.forth = 5;
            } else if (line.val[3] && line.first != 10 && line.second != 10 &&
                       line.third != 10 && line.count[3] >= 100) {
              line.forth = 10;
            }
          }
          if (line.first == 0) {
            if (line.second == -5) {
              line.deg = 150;
            } else if (line.second == 5) {
              line.deg = 210;
            } else {
              line.deg = 180;
            }
          } else if (line.first == -5) {
            if (line.second == 0) {
              line.deg = 150;
            } else {
              line.deg = 90;
            }
          } else if (line.first == 5) {
            if (line.second == 0) {
              line.deg = 210;
            } else {
              line.deg = 270;
            }
          } else if (line.first == 10) {
            if (line.second == -5) {
              line.deg = 30;
            } else if (line.second == 5) {
              line.deg = 330;
            } else {
              line.deg = 0;
            }
            line.deg = 0;
          }
          // if (line.deg == 180) {
          //   if (line.val[1]) {
          //     line.deg = 135;
          //   } else if (line.val[2]) {
          //     line.deg = 225;
          //   }
          // } else if (line.deg == 90) {
          //   if (line.val[0]) {
          //     line.deg = 135;
          //   } else if (line.val[3]) {
          //     line.deg = 45;
          //   }
          // } else if (line.deg == 270) {
          //   if (line.val[0]) {
          //     line.deg = 225;
          //   } else if (line.val[3]) {
          //     line.deg = 315;
          //   }
          // } else if (line.deg == 0) {
          //   if (line.val[1]) {
          //     line.deg = 45;
          //   } else if (line.val[2]) {
          //     line.deg = 315;
          //   }
          // }
        } else if (line.deg == 1000 && line.outMove != 1000) {
          line.deg = line.outMove;
          line.outMove = 1000;
        }
      }
    } else if (line.flag && line.outMove == 1000) {
      line.outMove = line.deg;
      line.deg = 1000;
      line.outTimer = millis();
    } else if (line.flag && line.outMove != 1000) {
      if (line.first == 0) {
        if (millis() - line.outTimer >= 500) {
          line.flag = false;
          line.outMove = 1000;
        }
      } else {
        if (millis() - line.outTimer >= 300) {
          line.flag = false;
          line.outMove = 1000;
        }
      }
    } else {
      // line.count = 0;
      line.flag = false;
      line.deg = 1000;
      line.outMove = 1000;
      line.first = 100;
      line.second = 100;
      line.third = 100;
      line.forth = 100;
    }

    motor.restart++;

    if (motor.restart >= 2000) {
      motor.restart = 0;
      for (int i = 0; i <= 100; i++) {
        digitalWrite(4, LOW);
        digitalWrite(5, LOW);
        digitalWrite(6, LOW);
        digitalWrite(7, LOW);
        digitalWrite(8, LOW);
        digitalWrite(9, LOW);
        digitalWrite(10, LOW);
        digitalWrite(11, LOW);
        digitalWrite(12, LOW);
      }
      motor.directDrive(motor.val);
    }
  }

  // if (device.mode != 2 || !device.keeper) {
  startTimer5(50);  //タイマー割り込みを有効化
  // }
}