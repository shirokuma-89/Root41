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

    //試験的実装
    line.read();

    if (line.touch) {
      line.flag = true;
      if (line.deg == 1000 && line.outMove == 1000) {
        //初めて反応
        if (line.val[0]) {
          line.deg = 180;
        } else if (line.val[1]) {
          line.deg = 90;
        } else if (line.val[2]) {
          line.deg = 270;
        } else if (line.val[3]) {
          line.deg = 0;
        }
      } else if (line.deg != 1000 && line.outMove == 1000) {
        //反応した後
        if (line.offset <= 20 || line.offset >= 340) {
          if (line.deg == 90) {
            if (line.val[0]) {
              line.deg = 135;
            } else if (line.val[3]) {
              line.deg = 45;
            }
          } else if (line.deg == 270) {
            if (line.val[0]) {
              line.deg = 225;
            } else if (line.val[3]) {
              line.deg = 315;
            }
          }
        }
      } else if (line.deg == 1000 && line.outMove != 1000) {
        //継続中に反応
        line.deg = line.outMove;
        line.outMove = 1000;
        if (line.deg == 0) {
          if (line.val[1]) {
            line.deg = 45;
          } else if (line.val[2]) {
            line.deg = 315;
          }
        } else if (line.deg == 180) {
          if (line.val[1]) {
            line.deg = 135;
          } else if (line.val[2]) {
            line.deg = 225;
          }
        }
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

  startTimer5(50);  //タイマー割り込みを有効化
}