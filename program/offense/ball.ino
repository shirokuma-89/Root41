// ball.ino

void _ball::read(int* b) {
  *b = analogRead(BALL0);
  *(b + 1) = analogRead(BALL1);
  *(b + 2) = analogRead(BALL2);
  *(b + 3) = analogRead(BALL3);
  *(b + 4) = analogRead(BALL4);
  *(b + 5) = analogRead(BALL5);
  *(b + 6) = analogRead(BALL6);
  *(b + 7) = round((float)analogRead(BALL7) * 0.82);
  *(b + 8) = round((float)analogRead(BALL8) * 0.82);
  *(b + 9) = round((float)analogRead(BALL9) * 0.82);
  *(b + 10) = analogRead(BALL10);
  *(b + 11) = analogRead(BALL11);
  *(b + 12) = analogRead(BALL12);
  *(b + 13) = analogRead(BALL13);
  *(b + 14) = analogRead(BALL14);
  *(b + 15) = analogRead(BALL15);
}

void _ball::calc(void) {
  deg = 1000;

  top = 0;

  for (int i = 0; i <= 15; i++) {
    if (val[top] >= val[i]) {
      top = i;
    }
  }

  if (val[top] > 540) {
    exist = false;
  } else {
    exist = true;
  }

  //回り込み
  if (top > 2 && top < 14) {
    if (val[top] < 255) {
      if (deg > 180) {
        if (top <= 4 || top >= 12) {
          top -= 3;
        } else if (top <= 6 || top >= 10) {
          top -= 3;
        } else {
          top -= 4;
        }
      } else {
        if (top <= 4 || top >= 12) {
          top += 3;
        } else if (top <= 6 || top >= 10) {
          top += 3;
        } else {
          top += 4;
        }
      }

      top += 16;
      top %= 16;
    }

    motor.power -= 30;

    if (top <= 6 || top >= 10) {
      motor.power += 15;
    }

    // if (top <= 4 || top >= 12) {
    //   motor.power -= 25;
    // }
  }

  deg = round((float)top * 22.5);
}

void _ball::reset(void) {
  if (millis() - resetTimer >= 1000) {
    digitalWrite(BALL_RESET, LOW);
    resettingTimer = millis();
    while (millis() - resettingTimer <= 7) {
      if (!line.flag) {
        if (exist) {
          motor.drive(motor.deg, motor.power);
        } else {
          motor.drive(NULL, NULL, false, true);
        }
      } else {
        break;
      }
    }
    digitalWrite(BALL_RESET, HIGH);
    resettingTimer = millis();
    while (millis() - resettingTimer <= 7) {
      if (!line.flag) {
        if (exist) {
          motor.drive(motor.deg, motor.power);
        } else {
          motor.drive(NULL, NULL, false, true);
        }
      } else {
        break;
      }
    }
    resetTimer = millis();
    motor.move -= 10;
  } else {
    return;
  }
}
