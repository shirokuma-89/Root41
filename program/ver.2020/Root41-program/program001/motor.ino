_motor::_motor(void) {
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);

  //モーターを開放
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

void _motor::drive(int _deg, int _power, bool _stop = false) {
  //初期化
  val[0] = 0;
  val[1] = 0;
  val[2] = 0;

  if (!_stop) {
    //角度取得
    gyro.deg = gyro.read();

    //姿勢制御
    Kp = 0.55;  //比例定数
    Ki = 0.02;  //積分定数
    Kd = 0.1;   //微分定数
    Km = 0.5;

    _power *= carpet._motorPower[carpet.tile];

    int correctionMinimum;  //角度補正の最小絶対値
    front = gyro.deg;
    front = front > 180 ? front - 360 : front;
    correctionMinimum = front * Km;
    if (abs(front) <= 20)
      integral += front;
    front *= Kp;             //比例制御
    front += integral * Ki;  //積分制御
    front *= 2.3;
    _front = (gyro.differentialRead() * Kd);
    _front += front;  //微分制御

    //角度補正の最小絶対値をcorrectionMinimumに設定
    if ((gyro.deg > 180 ? gyro.deg - 360 : gyro.deg) > 0) {
      _front = max(correctionMinimum, _front);
    } else {
      _front = min(-correctionMinimum, _front);
    }

    //機体が前を向いたら積分していたものをクリアする
    if (gyro.deg <= 5 || gyro.deg >= 355) {
      integral = 0;
    }

    correctionVal = _front;
    correctionVal = constrain(correctionVal, -100, 100);

    if (!(_deg == NULL && _power == NULL)) {
      float s;
      val[0] = calcVal[0][_deg];
      val[1] = calcVal[1][_deg];
      val[2] = calcVal[2][_deg];

      for (int i = 0; i <= 2; i++) {
        val[i] *= -1;
        val[i] += correctionVal;
      }

      if (abs(val[0]) < abs(val[1])) {
        if (abs(val[1]) < abs(val[2])) {
          s = 100.0 / (float)abs(val[2]);
        } else {
          s = 100.0 / (float)abs(val[1]);
        }
      } else {
        if (abs(val[0]) < abs(val[2])) {
          s = 100.0 / (float)abs(val[2]);
        } else {
          s = 100.0 / (float)abs(val[0]);
        }
      }
      val[0] = round((float)val[0] * s);
      val[1] = round((float)val[1] * s);
      val[2] = round((float)val[2] * s);

      for (int i = 0; i <= 2; i++) {
        val[i] = map(val[i], -100, 100, -_power, _power);
        val[i] = constrain(val[i], -100, 100);
      }

      for (int i = 0; i <= 2; i++) {
        error = 180;  // 25
        if (gyro.deg >= error && gyro.deg <= 360 - error) {
          LED.changeAll(LED.BLUE);
          // goto ERROR;
          error = 1000;
        }
      }
    } else {
      error = 25;
    ERROR:
      for (int i = 0; i <= 2; i++) {
        if (gyro.deg <= 3 || gyro.deg >= 358) {
          val[i] = 0;
        } else {
          if (abs(correctionVal) <= correctionMinimum) {
            if (correctionVal >= 0) {
              val[i] = correctionMinimum;
            } else {
              val[i] = -correctionMinimum;
            }
          } else {
            val[i] = correctionVal;
          }
        }
      }

      // // if (error = 1000) {
      //   for (int i = 0; i <= 2; i++) {
      //     val[i] *= 0.4;
      //   }
      // // }
    }
  }

  if (device.robot) {
    for (int i = 0; i <= 2; i++) {
      val[i] *= -1;
    }
  }

  directDrive(val);
}

void _motor::directDrive(int* p) {
  if (*(p + 1) == 0) {
    PORTH |= _BV(4) | _BV(5);

    digitalWrite(9, HIGH);
  } else if (*(p + 1) < 0) {
    PORTH |= _BV(4);
    PORTH &= ~(_BV(5));

    if (*(p + 1) >= 100) {
      digitalWrite(9, HIGH);
    } else {
      analogWrite(9, constrain(abs(round((float)*(p + 1) * 2.55)), 0, 255));
    }
  } else if (*(p + 1) > 0) {
    PORTH &= ~(_BV(4));
    PORTH |= _BV(5);

    if (*(p + 1) <= -100) {
      digitalWrite(9, HIGH);
    } else {
      analogWrite(9, constrain(abs(round((float)*(p + 1) * 2.55)), 0, 255));
    }
  }
  if (*p == 0) {
    // PORTB |= _BV(4) | _BV(5);
    digitalWrite(10, HIGH);
    digitalWrite(11, HIGH);

    digitalWrite(12, HIGH);
  } else if (*p < 0) {
    // PORTB |= _BV(4);
    // PORTB &= ~(_BV(5));
    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);

    if (*p >= 100) {
      digitalWrite(12, HIGH);
    } else {
      analogWrite(12, constrain(abs(round((float)*p * 2.55)), 0, 255));
    }
  } else if (*p > 0) {
    // PORTB &= ~(_BV(4));
    // PORTB |= _BV(5);
    digitalWrite(10, LOW);
    digitalWrite(11, HIGH);

    if (*p <= -100) {
      digitalWrite(12, HIGH);
    } else {
      analogWrite(12, constrain(abs(round((float)*p * 2.55)), 0, 255));
    }
  }

  if (*(p + 2) == 0) {
    // PORTG |= _BV(5);
    // PORTE |= _BV(3);

    digitalWrite(4, HIGH);
    digitalWrite(5, HIGH);

    digitalWrite(6, HIGH);
  } else if (*(p + 2) < 0) {
    // PORTG |= _BV(5);
    // PORTE &= ~(_BV(3));
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);

    if (*(p + 2) >= 100) {
      digitalWrite(6, HIGH);
    } else {
      analogWrite(6, constrain(abs(round((float)*(p + 2) * 2.55)), 0, 255));
    }
  } else if (*(p + 2) > 0) {
    // PORTG &= ~(_BV(5));
    // PORTE |= _BV(3);
    digitalWrite(4, LOW);
    digitalWrite(5, HIGH);

    if (*(p + 2) <= -100) {
      digitalWrite(6, HIGH);
    } else {
      analogWrite(6, constrain(abs(round((float)*(p + 2) * 2.55)), 0, 255));
    }
  }
}
