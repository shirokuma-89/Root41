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
      if (millis() - line.inTimer >= 300) {
        line.stop = false;
      }
    } else if (line.touch) {
      if (line.count >= 100) {
        line.flag = true;
        if (line.deg == 1000 && line.outMove == 1000) {
          line.near = true;
          line.inTimer = millis();
          if (line.val[0]) {
            line.deg = 180;
            line.highPin = 0;
            line.highPin = 0;
          } else if (line.val[1]) {
            line.deg = 90;
            line.highPin = 1;
            line.highPin = 1;
          } else if (line.val[2]) {
            line.deg = 270;
            line.highPin = 2;
            line.highPin = 2;
          } else if (line.val[3]) {
            line.deg = 0;
            line.highPin = 3;
            line.highPin = 3;
          }
          if (line.highPin == 1 || line.highPin == 2) {
            line.stop = true;
          }
        } else if (line.deg != 1000 && line.outMove == 1000) {
          if (line.first == 0) {
            if (line.val[1]) {
              line.deg = 135;
            } else if (line.val[2]) {
              line.deg = 225;
            }
          } else if (line.first == 1) {
            if (line.val[0]) {
              line.deg = 135;
            } else if (line.val[3]) {
              line.deg = 45;
            }
          } else if (line.first == 2) {
            if (line.val[0]) {
              line.deg = 225;
            } else if (line.val[3]) {
              line.deg = 315;
            }
          } else if (line.first == 3) {
            if (line.val[1]) {
              line.deg = 45;
            } else if (line.val[2]) {
              line.deg = 315;
            }
          }
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
      if (millis() - line.outTimer >= 300) {
        line.flag = false;
        line.outMove = 1000;
      }
    } else {
      // line.count = 0;
      line.flag = false;
      line.deg = 1000;
      line.outMove = 1000;
      line.first = 5;
    }
  }

  motor.restart++;

  if (motor.restart >= 2000) {
    motor.restart = 0;
    motor.directDrive(motor.subVal);
    motor.directDrive(motor.val);
  }

  startTimer5(50);  //タイマー割り込みを有効化
}