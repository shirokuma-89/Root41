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

    if (line.touch) {
      line.flag = true;
      if (line.deg == 1000 && line.outMove == 1000) {
        line.near = true;
        line.inTimer = millis();
        if (line.val[0]) {
          line.deg = 180;
          line.highPin = 0;
        } else if (line.val[1]) {
          line.deg = 90;
          line.highPin = 1;
        } else if (line.val[2]) {
          line.deg = 270;
          line.highPin = 2;
        } else if (line.val[3]) {
          line.deg = 0;
          line.highPin = 3;
        }
      } else if (line.deg != 1000 && line.outMove == 1000) {
        if (line.deg == 90) {
          if (line.val[0]) {
            if (line.offset >= 330) {
              line.deg = 135;
            }
          } else if (line.val[3]) {
            if (line.offset <= 30) {
              line.deg = 45;
            }
          }
        } else if (line.deg == 270) {
          if (line.val[0]) {
            if (line.offset <= 30) {
              line.deg = 225;
            }
          } else if (line.val[3]) {
            if (line.offset >= 330) {
              line.deg = 315;
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
      if (millis() - line.outTimer >= 400) {
        line.flag = false;
        line.outMove = 1000;
      }
    } else {
      line.flag = false;
      line.deg = 1000;
      line.outMove = 1000;
    }
  }

  motor.restart++;

  if(motor.restart >= 4000){
    motor.restart = 0;
    motor.directDrive(motor.subVal);
    motor.directDrive(motor.val);
  }

  startTimer5(50);  //タイマー割り込みを有効化
}