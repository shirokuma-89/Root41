// IOピンの初期化
void _device::initialize(void) {
  pinMode(BALL0, INPUT);
  pinMode(BALL1, INPUT);
  pinMode(BALL2, INPUT);
  pinMode(BALL3, INPUT);
  pinMode(BALL4, INPUT);
  pinMode(BALL5, INPUT);
  pinMode(BALL6, INPUT);
  pinMode(BALL7, INPUT);
  pinMode(BALL8, INPUT);
  pinMode(BALL9, INPUT);
  pinMode(BALL10, INPUT);
  pinMode(BALL11, INPUT);
  pinMode(BALL12, INPUT);
  pinMode(BALL13, INPUT);
  pinMode(BALL15, INPUT);
  pinMode(BALL_RESET, OUTPUT);

  pinMode(LINE1, INPUT_PULLUP);
  pinMode(LINE2, INPUT_PULLUP);
  pinMode(LINE3, INPUT_PULLUP);
  pinMode(LINE4, INPUT_PULLUP);

  pinMode(SW_TOGGLE, INPUT);
  pinMode(SW_RIGHT, INPUT);
  pinMode(SW_LEFT, INPUT);

  pinMode(ROTARY1, INPUT);
  pinMode(ROTARY2, INPUT);

  digitalWrite(BALL_RESET, HIGH);
}

//タイマー割り込み
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
    //ライン動作中はline.flugをtrueにしておくこと
  }

  startTimer5(30);  //タイマー割り込みを有効化
}

void _device::monitor(void) {
  if ((digitalRead(SW_LEFT) && digitalRead(SW_RIGHT)) || device.monitorSend) {
    startTimer5(100);
    lcd.print("Root41 monitor");
    uint16_t i, j;
    unsigned long led_timer = millis();
    RGBLED.setBrightness(RGB_bright + 30);
    while (true) {
      for (j = 0; j < 256; j++) {
        RGBLED.begin();
        for (i = 0; i < RGBLED.numPixels(); i++) {
          RGBLED.setPixelColor(i, Wheel((i + j) & 255));
        }
        RGBLED.show();
        // RGBLED.begin();
        // RGBLED.setBrightness(100);
        // for (int i = 0; i <= 15; i++) {
        //   RGBLED.setPixelColor(i, 170 * 0.7, 0, 255 * 0.7);
        // }
        // RGBLED.show();
        led_timer = millis();
        while (millis() - led_timer <= 20) {
          gyro.deg = gyro.read();

          if (!ball.Reset) {
            digitalWrite(BALL_RESET, HIGH);
          } else {
            digitalWrite(BALL_RESET, LOW);
          }
          ball.val[0] = analogRead(BALL0);
          ball.val[1] = analogRead(BALL1);
          ball.val[2] = analogRead(BALL2);
          ball.val[3] = analogRead(BALL3);
          ball.val[4] = analogRead(BALL4);
          ball.val[5] = analogRead(BALL5);
          ball.val[6] = analogRead(BALL6);
          ball.val[7] = analogRead(BALL7);
          ball.val[8] = analogRead(BALL8);
          ball.val[9] = analogRead(BALL9);
          ball.val[10] = analogRead(BALL10);
          ball.val[11] = analogRead(BALL11);
          ball.val[12] = analogRead(BALL12);
          ball.val[13] = analogRead(BALL13);
          ball.val[14] = analogRead(BALL14);
          ball.val[15] = analogRead(BALL15);
          for (int i = 0; i <= 15; i++) {
            ball.val[i] = round((float)ball.val[i] / 10.24);
          }

          Serial.write('R');
          Serial.write('o');
          Serial.write('o');
          Serial.write('t');
          Serial.write('4');
          Serial.write('1');

          Serial.write(highByte(int(round(gyro.deg))));
          Serial.write(lowByte(int(round(gyro.deg))));
          if (!digitalRead(LINE1)) {
            Serial.write(1);
          } else {
            Serial.write(2);
          }
          if (!digitalRead(LINE2)) {
            Serial.write(1);
          } else {
            Serial.write(2);
          }
          if (!digitalRead(LINE3)) {
            Serial.write(1);
          } else {
            Serial.write(2);
          }
          if (!digitalRead(LINE4)) {
            Serial.write(1);
          } else {
            Serial.write(2);
          }
          for (int i = 0; i <= 15; i++) {
            Serial.write(ball.val[i]);
          }
          Serial.write(digitalRead(SW_TOGGLE));
          Serial.write(digitalRead(SW_RIGHT));
          Serial.write(digitalRead(SW_LEFT));
          Serial.write(highByte(device.rotary));
          Serial.write(lowByte(device.rotary));

          if (Serial.available() > 9) {
            if (Serial.read() == 'r') {
              if (Serial.read() == 'o') {
                if (Serial.read() == 'o') {
                  if (Serial.read() == 't') {
                    motor.val[0] = Serial.read();
                    motor.val[1] = Serial.read();
                    motor.val[2] = Serial.read();
                    if (Serial.read() == '4') {
                      if (Serial.read() == '1') {
                        motor.val[0] -= 100;
                        motor.val[1] -= 100;
                        motor.val[2] -= 100;
                        if (Serial.read() == 1) {
                          ball.Reset = true;
                        } else {
                          ball.Reset = false;
                        }

                        motor.directDrive(motor.val);
                      }
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    pauseTimer5();
  }
}

void _device::rainbowLED(void) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < RGBLED.numPixels(); i++) {
      RGBLED.setPixelColor(i, Wheel((i + j) & 255));
    }
    RGBLED.show();
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return RGBLED.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return RGBLED.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return RGBLED.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void stoping() {
  // Root41.mabc(0, 0, 0);
  motor.drive(NULL, NULL, true);
  delay(400);
}
