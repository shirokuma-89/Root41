//device.ino

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

void _device::monitor(void) {
  if (digitalRead(SW_LEFT) || digitalRead(SW_RIGHT) || monitorBegin) {
    startTimer5(100);
    lcd.print("Root41 monitor");

    LED.timer = millis();
    RGBLED.setBrightness(LED.bright + 30);

    digitalWrite(BALL_RESET, HIGH);
    
    while (true) {
      for (LED.j = 0; LED.j < 256; LED.j++) {
        RGBLED.begin();
        for (LED.i = 0; LED.i < RGBLED.numPixels(); LED.i++) {
          RGBLED.setPixelColor(LED.i, Wheel((LED.i + LED.j) & 255));
        }
        RGBLED.show();

        LED.timer = millis();
        while (millis() - LED.timer <= 20) {
          gyro.deg = gyro.read();

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
                          digitalWrite(BALL_RESET, LOW);
                        } else {
                          digitalWrite(BALL_RESET, HIGH);
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