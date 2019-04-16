// ino

void _ball::read(int* b) {
  *b = analogRead(BALL0);
  *(b + 1) = analogRead(BALL1);
  *(b + 2) = analogRead(BALL2);
  *(b + 3) = analogRead(BALL3);
  *(b + 4) = analogRead(BALL4);
  *(b + 5) = analogRead(BALL5);
  *(b + 6) = analogRead(BALL6);
  *(b + 7) = round((float)analogRead(BALL8) * 0.7);
  *(b + 8) = round((float)analogRead(BALL8) * 0.65);
  *(b + 9) = round((float)analogRead(BALL9) * 0.7);
  *(b + 10) = analogRead(BALL10);
  *(b + 11) = analogRead(BALL11);
  *(b + 12) = analogRead(BALL12);
  *(b + 13) = analogRead(BALL13);
  *(b + 14) = analogRead(BALL14);
  *(b + 15) = analogRead(BALL15);

  if (ROBOT == 2) {
    *(b + 4) = (*(b + 3) + *(b + 5)) / 2;
    *(b + 12) = (*(b + 11) + *(b + 13)) / 2;
  }
}

void _ball::calc(void) {
  // ball.degは deg = round((float)top * 22.5);まで使用不可
  // motor.power -= 15;

  deg = 1000;

  // motor.power -= 25;

  top = 0;

  for (int i = 0; i <= 15; i++) {
    if (val[top] >= val[i]) {
      top = i;
    }
  }
  top_backup = top;

  if (val[top] > 540) {
    exist = false;
    device.attack = false;
  } else {
    exist = true;
  }

  if (millis() - device.keeperTimeout >= 5000) {
    device.attack = true;
  }

  if (device.attack) {
    if (millis() - device.attackTimeout >= 5000) {
      if (val[0] >= 240) {
        device.attack = false;
        device.keeperTimeout = millis();
        device.attackTimeout = millis();
      }
    }
  }

  if (!device.keeper || device.attack) {
    //回り込み
    if (top > 2 + turn && top < 14 - turn) {
      if (val[top] < 256) {
        turnTimer = millis();
        motor.power -= 20;
        if (top > 8) {
          if (top >= 13) {
            top -= 3;
          } else {
            top -= 4;
          }
        } else {
          if (top <= 3) {
            top += 3;
          } else {
            top += 4;
          }
        }

        top += 16;
        top %= 16;

        turn = 1;
      } else {
        turn = 0;
        // if (turnTimer + 2000 <= millis()) {
        //   turn = 0;
        // }
      }
    } else {
      if (turnTimer + 2000 <= millis()) {
        turn = 0;
      }
    }

    deg = round((float)top * 22.5);

    if (line.near && !line.flag) {
      if (line.inTimer + 1500 > millis()) {
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

    if (line.near) {
      exist = false;
    }
  } else {
    device.attack = false;
    line.near = false;
    line.touch = false;

    line.val[0] = !digitalRead(LINE1);
    line.val[1] = !digitalRead(LINE2);
    line.val[2] = !digitalRead(LINE3);
    line.val[3] = !digitalRead(LINE4);

    //どれか一つでも反応しているならば
    if (line.val[0] | line.val[1] | line.val[2] | line.val[3]) {
      line.touch = true;
    }
    line.flag = false;

    motor.power -= 20;

    pauseTimer5();
    usonic.distance = usonic.getDistance();
    startTimer5(50);

    if (val[top] > 520) {
      exist = false;
    } else {
      exist = true;
    }
    if (top <= 8) {
      if (usonic.distance >= 20) {
        deg = 113;
      } else if (usonic.distance <= 25) {
        deg = 68;
      } else {
        deg = 90;
      }
    } else {
      if (usonic.distance >= 20) {
        deg = 248;
      } else if (usonic.distance <= 25) {
        deg = 293;
      } else {
        deg = 270;
      }
    }

    // if (top <= 1 || top >= 15) {
    //   exist = false;
    // }

    if (top == 0) {
      exist = false;
    }

    if (top == (device.keeperExit + 17) % 16) {
      device.attackTimeout = millis();
    } else if (top == device.keeperExit) {
      device.attackTimeout = millis();
    } else if (top == (device.keeperExit + 1) % 16) {
      device.attackTimeout = millis();
    } else if (top == 9 || top == 8) {
      device.attackTimeout = millis();
    } else {
      device.keeperExit = top;
      device.keeperTimeout = millis();
    }

    if (top >= 5 && top <= 11) {
      exist = false;
    }

    if (usonic.distance >= 40) {
      exist = true;
      if (top >= 6 && top <= 10) {
        if (top >= 8) {
          deg = 135;
        } else {
          deg = 225;
        }
      } else {
        deg = 180;
      }
    }
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
