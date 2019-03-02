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
  }

  startTimer5(50);  //タイマー割り込みを有効化
}