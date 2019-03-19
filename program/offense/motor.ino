// motor.ino

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

void _motor::drive(int _deg,
                   int _power,
                   bool stop = false,
                   bool correctionDeg = false) {
  if (stop) {  //完全停止
    val[0] = 0;
    val[1] = 0;
    val[2] = 0;
    directDrive(val);
  } else {  //角度が指定された時の動作

    //初期化
    val[0] = 0;
    val[1] = 0;
    val[2] = 0;

    //角度取得
    gyro.deg = gyro.read();

    //姿勢制御
    static float Kp;  // PD制御の場合 Kp = 0.85
    static float Ki;  //             Ki = 0
    static float Kd;  //             Kd = 0.15

    //各自調整
    if (ROBOT == 1) {
      Kp = 0.753;
      Ki = 0.00037;
      Kd = 0.175;
    } else {
      Kp = 0.753;
      Ki = 0.00037;
      Kd = 0.175;
    }

    front = gyro.deg;
    front -= line.offset;
    front = front + 360;
    front = front >= 360 ? front - 360 : front;

    front = front > 180 ? front - 360 : front;

    integral += front;
    // deg_integral = constrain(deg_integral, -350, 350);
    front *= Kp * -1;                       //比例制御
    front += gyro.differentialRead() * Kd;  //微分制御
    front -= integral * Ki;                 //積分制御

    if (integralTimer + 5000 <= millis()) {
      integralTimer = millis();
      integral = 0;
    }

    // motor
    correctionVal = round(front);
    correctionVal = constrain(correctionVal, -45, 45);
    if (!correction) {
      correctionVal = 0;
    }

    // front = constrain(front, -40, 40);

    float s;
    if (_deg == 0) {
      val[0] = 97;
      val[1] = -100;
      val[2] = 0;
    } else if (_deg == 23 || _deg == 22) {
      val[0] = 100;
      val[1] = -36;
      val[2] = -23;
    } else if (_deg == 45) {
      val[0] = 100;
      val[1] = -15;
      val[2] = -60;
    } else if (_deg == 68 || _deg == 67) {
      val[0] = 100;
      val[1] = 13;
      val[2] = -90;
    } else if (_deg == 90) {
      val[0] = 40;  // 34
      val[1] = 40;
      val[2] = -100;
    } else if (_deg == 113 || _deg == 112) {
      val[0] = 13;
      val[1] = 100;
      val[2] = -90;
    } else if (_deg == 135) {
      val[0] = -20;
      val[1] = 100;
      val[2] = -48;
    } else if (_deg == 158 || _deg == 157) {
      val[0] = -34;
      val[1] = 100;
      val[2] = -27;
    } else if (_deg == 180) {
      val[0] = -90;
      val[1] = 100;
      val[2] = 0;
    } else if (_deg == 203 || _deg == 202) {
      val[0] = -100;
      val[1] = 34;
      val[2] = 27;
    } else if (_deg == 225) {
      val[0] = -100;
      val[1] = 20;
      val[2] = 48;
    } else if (_deg == 248 || _deg == 247) {
      val[0] = -100;
      val[1] = -13;
      val[2] = 90;
    } else if (_deg == 270) {
      val[0] = -40;  //-34
      val[1] = -40;  //-34
      val[2] = 100;
    } else if (_deg == 293 || _deg == 292) {
      val[0] = -13;
      val[1] = -100;
      val[2] = 90;
    } else if (_deg == 315) {
      val[0] = 15;
      val[1] = -100;
      val[2] = 60;
    } else if (_deg == 338 || _deg == 337) {
      val[0] = 34;
      val[1] = -100;
      val[2] = 23;
    } else {
      val[0] = int(sin(radians(_deg - 300)) * 100.0);
      val[1] = int(sin(radians(_deg - 60)) * 100.0);
      val[2] = int(sin(radians(_deg - 180)) * 100.0);

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
    }

    for (int i = 0; i <= 2; i++) {
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
      val[i] = constrain(val[i], -98, 98);
    }

    if (correctionDeg) {
      for (int i = 0; i <= 2; i++) {
        val[i] = correctionVal;
      }
    }
    directDrive(val);
  }
}

void _motor::directDrive(int* p) {
  if (*(p + 2) == 0) {
    digitalWrite(10, LOW);
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
  } else if (*(p + 2) >= 1) {
    if (*(p + 2) >= 100) {
      digitalWrite(10, HIGH);
      digitalWrite(11, LOW);
      digitalWrite(12, HIGH);
    }

    digitalWrite(10, HIGH);
    digitalWrite(11, LOW);
    analogWrite(12, abs(round((float)*(p + 2) * 2.55)));
  } else if (*(p + 2) <= -1) {
    if (*(p + 2) <= -100) {
      digitalWrite(10, LOW);
      digitalWrite(11, HIGH);
      digitalWrite(12, HIGH);
    }

    digitalWrite(11, HIGH);
    digitalWrite(10, LOW);
    analogWrite(12, abs(round((float)*(p + 2) * 2.55)));
  }

  if (*p == 0) {
    digitalWrite(4, LOW);
    digitalWrite(5, LOW);
    digitalWrite(6, HIGH);
  } else if (*p >= 1) {
    if (*p >= 100) {
      digitalWrite(4, HIGH);
      digitalWrite(5, LOW);
      digitalWrite(6, HIGH);
    }

    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
    analogWrite(6, round((float)*p * 2.55));
  } else if (*p <= -1) {
    if (*p <= -100) {
      digitalWrite(5, HIGH);
      digitalWrite(4, LOW);
      digitalWrite(6, HIGH);
    }

    digitalWrite(5, HIGH);
    digitalWrite(4, LOW);
    analogWrite(6, abs(round((float)*p * 2.55)));
  }

  if (*(p + 1) == 0) {
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, HIGH);
  } else if (*(p + 1) >= 1) {
    if (*(p + 1) >= 100) {
      digitalWrite(7, HIGH);
      digitalWrite(8, LOW);
      digitalWrite(9, HIGH);
    }

    digitalWrite(7, HIGH);
    digitalWrite(8, LOW);
    analogWrite(9, abs(round((float)*(p + 1) * 2.55)));
  } else if (*(p + 1) <= -1) {
    if (*(p + 1) <= -100) {
      digitalWrite(7, LOW);
      digitalWrite(8, HIGH);
      digitalWrite(9, HIGH);
    }

    digitalWrite(8, HIGH);
    digitalWrite(7, LOW);
    analogWrite(9, abs(round((float)*(p + 1) * 2.55)));
  }
}