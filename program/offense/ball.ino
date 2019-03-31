// ino

void _ball::read(int* b) {
  *b = analogRead(BALL0);
  *(b + 1) = analogRead(BALL1);
  *(b + 2) = analogRead(BALL2);
  *(b + 3) = analogRead(BALL3);
  *(b + 4) = analogRead(BALL4);
  *(b + 5) = analogRead(BALL5);
  *(b + 6) = analogRead(BALL6);
  *(b + 7) = analogRead(BALL7);
  *(b + 8) = round((float)analogRead(BALL8) * 0.82);
  *(b + 9) = round((float)analogRead(BALL9) * 0.75);
  *(b + 10) = round((float)analogRead(BALL10) * 0.82);
  *(b + 11) = analogRead(BALL11);
  *(b + 12) = analogRead(BALL12);
  *(b + 13) = analogRead(BALL13);
  *(b + 14) = analogRead(BALL14);
  *(b + 15) = analogRead(BALL15);
}

void _ball::calc(void) {
  // ball.degは deg = round((float)top * 22.5);まで使用不可

  motor.power -= 10;
  deg = 1000;

  top = 0;

  for (int i = 0; i <= 15; i++) {
    if (val[top] >= val[i]) {
      top = i;
    }
  }
  top_backup = top;

  if (val[top] > 540) {
    exist = false;
  } else {
    exist = true;
  }

  //回り込み
  if (top > 3 && top < 13) {
    if (val[top] < 252) {
      motor.power -= 10;

      if (top <= 6 || top >= 12) {
        motor.power -= 20;
      }

      if (top > 8) {
        // if (top >= 12) {
        //   top -= 1;
        // } else
        if (top >= 12) {
          top -= 3;
        } else {
          top -= 4;
        }
      } else {
        // if (top <= 4) {
        //   top += 1;
        // } else
        if (top <= 4) {
          top += 3;
        } else {
          top += 4;
        }
      }

      top += 16;
      top %= 16;
    }
  }

  deg = round((float)top * 22.5);

  if (line.near && !line.flag) {
    if (line.inTimer + 5000 > millis()) {
      if (line.highPin == 0) {
        if (ball.top_backup <= 2 || ball.top_backup >= 14) {
          line.near = true;
        } else {
          if (ball.exist) {
            line.near = false;
          }
        }
      } else if (line.highPin == 2) {
        if (ball.top_backup >= 2 && ball.top_backup <= 6) {
          line.near = true;
        } else {
          if (ball.exist) {
            line.near = false;
          }
        }
      } else if (line.highPin == 3) {
        if (ball.top_backup >= 6 && ball.top_backup <= 10) {
          line.near = true;
        } else {
          if (ball.exist) {
            line.near = false;
          }
        }
      } else {
        if (ball.top_backup >= 10 && ball.top_backup <= 14) {
          line.near = true;
        } else {
          if (ball.exist) {
            line.near = false;
          }
        }
      }
    } else {
      line.near = false;
    }
  }

  if (line.near){
    ball.exist = false;
  }
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
