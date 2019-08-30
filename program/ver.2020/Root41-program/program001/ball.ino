void _ball::read(int* b) {
  digitalWrite(BALL_RESET, HIGH);
  for (int i = 0; i <= 15; i++) {
    *(b + i) = analogRead(BALL[i]);
  }

  if (device.robot) {
    val[6] = (val[5] * 2 + val[8]) / 3;
    val[7] = (val[8] * 2 + val[5]) / 3;
    val[14] = (val[13] + val[15]) / 2;
  } else {
    val[12] = (val[11] * 2 + val[14]) / 3;
    val[13] = (val[11] + val[14] * 2) / 3;
    val[8] = (val[7] + val[9]) / 2;
  }
  
  val[14] *= 0.9;
  val[2] *= 0.9;

  for (int i = 0; i <= 15; i++) {
    Serial.print(val[i]);
    Serial.print("\t");
  }
  Serial.println("");

  digitalWrite(BALL_RESET, LOW);
}

void _ball::calc(void) {
  speed = 100;
  top = 0;
  for (int i = 0; i <= 15; i++) {
    if (val[i] <= val[top]) {
      top = i;
    }
  }

  deg = top * 22.5;

  if (top > 1 || top < 15) {
    if (deg >= 180) {
      deg -= 360;
    }
    deg = round((float)deg * (float)sqrt(abs(deg)) * (float)0.108);
    deg += 720;
    deg %= 360;
  }

  turn = false;
  if (top > 1 && top < 15) {
    if (val[top] <= 290) {
      turn = true;
      if (deg >= 180) {
        if (top >= 13) {
          speed = 80;
          deg += -44;
        } else {
          deg += -37;
        }
      } else {
        if (top <= 3) {
          speed = 80;
          deg += 44;
        } else {
          deg += 37;
        }
      }
    }
  }

  emg = false;
  if (top > 6 && top < 10) {
    if (val[top] <= 245) {
      emg = true;
      if (top >= 8) {
        deg -= 50;
      } else {
        deg += 50;
      }
    }
  }

  if ((top <= 2 || top >= 14) && digitalRead(BALL_HOLD)) {
    deg = 0;
    hold = true;
  } else {
    hold = false;
  }

  LCD.data = deg;
  LCD.unit = "DEG";

  exist = true;
  if (val[top] <= 620) {
    exCount = 0;
  } else {
    exCount++;
  }

  if (exCount >= 3) {
    exist = false;
  }
}
