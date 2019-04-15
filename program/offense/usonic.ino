// usonic.ino

int _usonic::getDistance(void) {
  if (ROBOT == 1) {
    //超音波に5usのパルスを出力
    pinMode(USONIC, OUTPUT);
    digitalWrite(USONIC, LOW);
    delayMicroseconds(2);
    digitalWrite(USONIC, HIGH);
    delayMicroseconds(5);
    digitalWrite(USONIC, LOW);

    pinMode(USONIC, INPUT);
    timer = pulseIn(USONIC, HIGH);  // パルス幅を計測

    if (timer < 18000) {
      //距離を計算
      data = constrain(int(((float)timer / 29.0) / 2.0), 0, 255);
    } else {
      // error!
      data = 0;
    }

    return (byte)data;
  } else {
    Wire.begin();
    TWBR = 12;
    Wire.requestFrom(8, 1);
    Wire.flush();

    timeOut = millis();
    while (timeOut + 35 >= millis()) {
      if (Wire.available()) {
        data = Wire.read();

        break;
      }
    }

    //受信バッファをクリア
    while (Wire.available()) {
      Wire.read();
    }
    Wire.flush();

    return (int)data;
  }
}