ISR(timer5Event) {
  line.read();
}

void _line::process(void) {
  if (line.flag) {
    ball.lineOut = millis();
    if (line.mode == 1 && line.touch) {
      ball._right = ball.right;
      //通常
      if (!line.rootsave) {
        for (int i = 0; i <= 19; i++) {
          if (line.logs[i] == 1 &&
              line.whited <= carpet._lineWhited[carpet.tile]) {
            line.x += line.vector[i][0] * 1.5;
            line.y += line.vector[i][1];
            line.logs[i] = 2;
          }
        }
      }
      line.deg = atan2(line.x, line.y);
      line.deg = degrees(line.deg);
      if (line.deg < 180) {
        line.deg += 180;
      } else {
        line.deg -= 180;
      }
      // if (line.whited >= 6) {
      //   if (line.root1[0] >= 1 && line.root2[0] <= -1 && line.root1[1] >= 10 &&
      //       line.root2[1] <= 9) {
      //     if (line.deg <= 20 || line.deg >= 340) {
      //       if (line.deg <= 180) {
      //         line.deg = 180 - line.deg;
      //       } else {
      //         line.deg = 180 + (360 - line.deg);
      //       }
      //       line.rootsave = true;
      //     }
      //   } else if (line.root2[0] >= 1 && line.root1[0] <= -1 &&
      //              line.root2[1] >= 10 && line.root1[1] <= 9) {
      //     if (line.deg <= 20 || line.deg >= 340) {
      //       if (line.deg <= 180) {
      //         line.deg = 180 - line.deg;
      //       } else {
      //         line.deg = 180 + (360 - line.deg);
      //       }
      //       line.rootsave = true;
      //     }
      //   }
      //   if (line.root1[0] <= -1 && line.root2[0] <= 1 && line.root1[1] >= 10 &&
      //       line.root2[1] <= 9) {
      //     if (line.deg >= 130 && line.deg <= 230) {
      //       if (line.deg <= 180) {
      //         line.deg = 180 - line.deg;
      //       } else {
      //         line.deg = 180 + (360 - line.deg);
      //       }
      //       line.rootsave = true;
      //     }
      //   } else if (line.root2[0] <= -1 && line.root1[0] >= 1 &&
      //              line.root2[1] >= 10 && line.root1[1] <= 9) {
      //     if (line.deg >= 130 && line.deg >= 230) {
      //       if (line.deg <= 180) {
      //         line.deg = 180 - line.deg;
      //       } else {
      //         line.deg = 180 + (360 - line.deg);
      //       }
      //       line.rootsave = true;
      //     }
      //   }
      // }

      //時間での変更
      if (millis() - line.stopTimer <= 50) {
        line.deg = 1000;
        //急ブレーキ
      } else if (millis() - line.stopTimer >= 1000) {
        if (line.deg <= 20 || line.deg >= 340) {
          if (line.right > line.left) {
            line.deg = 300;
          } else if (line.right < line.left) {
            line.deg = 60;
          }
        }
        //前に進むときにライントレースしちゃう防止
      } else if (millis() - line.stopTimer >= 4500) {
        if (!line.rootsave) {
          line.deg = atan2(0, line.y);
          line.deg = degrees(line.deg);
          if (line.deg < 180) {
            line.deg += 180;
          } else {
            line.deg -= 180;
          }
        } else {
          line.deg = 180;
        }
        //ゴールポストにつっかえ脱出
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
      //ですぎ防止
      if (!line.rootsave) {
        if (abs(line.deg - line.last * 18) <= 65 ||
            abs(line.deg - line.last * 18) >= 295) {
          line.mode = 3;
        }
      }
    } else if (line.mode == 2) {
      if (millis() - line.overTimer >=
          line.whited *
              carpet._lineWhitedT[carpet.tile]) {  //カーペットクラスを参照
        if (line.whited <= 10) {
          if (first >= 2 && first <= 7) {
            line.lock = 2;
            line.lockTimer = millis();
          } else if (first >= 12 && first <= 18) {
            line.lock = 1;
            line.lockTimer = millis();
          }
          line.flag = false;
          line.mode = 0;
        } else {
          line.flag = false;
          line.mode = 0;
        }
      }
    } else if (line.mode == 3) {
      if (millis() - line.overTimer >= 2000) {  //カーペットクラスを参照
        line.flag = false;
        line.mode = 0;
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
    line.now = 100;
    line.root1[0] = 100;
    line.root1[1] = 100;
    line.root1[2] = 100;
    line.root2[0] = 100;
    line.root2[1] = 100;
    line.root2[2] = 100;
    line.rootsave = false;
    line.last = 100;
    line.right = 0;
    line.left = 0;
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
        whited++;
        if (i <= 9) {
          right++;
        } else {
          left++;
        }
        now = i;
        logs[i] = 1;
        if (!flag) {
          sigdeg = gyro.deg;
          first = i;
          root1[1] = i;
          motor.integral = 0;
          if (line.sigdeg >= 300) {
            gyro.offset = -((360 + line.sigdeg) / 2);
          } else if (line.sigdeg <= 60) {
            gyro.offset = -(line.sigdeg / 2);
          } else {
            gyro.offset = -(line.sigdeg);
          }
          stopTimer = millis();
        }
        if (root2[1] == 100) {
          if (abs(first - i) >= 5 && abs(first - i) <= 15) {
            if (first <= 9 && i >= 10) {
              root2[1] = i;
            } else if (first >= 10 && i <= 9) {
              root2[1] = i;
            }
          }
        } else {
          if (abs(root2[1] - i) == 1 || abs(root2[1] - i) == 19) {
            root2[2] = i;
            root2[0] = root2[1] - root2[2];
          }
        }
        if (abs(root1[1] - i) == 1 || abs(root1[1] - i) == 19) {
          root1[2] = i;
          root1[0] = root1[1] - root1[2];
        }
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
