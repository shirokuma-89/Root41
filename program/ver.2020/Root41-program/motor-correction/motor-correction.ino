// Note:59行目へGO！！！！！！！！！！！

//ライブラリのインクルード
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <FaBoLCDmini_AQM0802A.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Timer5.h>
#include <VL53L0X.h>
#include <Wire.h>

//ピン番号定義
int BALL[16] = {A0, A1, A2,  A3,  A4,  A5,  A6,  A7,
                A8, A9, A10, A11, A12, A13, A14, A15};
#define BALL_RESET 26
#define BALL_HOLD 29

int LINE[20] = {30, 31, 32, 33, 34, 35, 36, 37, 38, 39,
                40, 41, 42, 43, 44, 45, 46, 47, 48, 49};

#define SW_2 22
#define SW_1 23
#define SW_RESET 25

#define BUZ_1 14
#define BUZ_2 15
#define BUZ_3 16

class _motor {
 public:
  _motor(void);

  void directDrive(int* p);

  int val[3];
  float cValue[3];

  const int move = 60;

  unsigned long moveTimer = 0;
  int integral = 0;

  int calcVal[3][360];

 private:
  int front = 0;
  int _front;
  float Kp;
  float Ki;
  float Kd;
  float Km;

  int correctionVal = 0;
  int error;

} motor;

void setup(void) {
  //この三つの値を調整するべし。必ず1.00以下にするべし。
  // step1,2,3すべてにおいてちょうどいい値をがんばって探すべし。
  motor.cValue[0] = 1.00;
  motor.cValue[1] = 1.00;
  motor.cValue[2] = 1.00;
}

void loop(void) {
  // // step1
  // motor.val[0] = 100 * motor.cValue[0];   // 0.98
  // motor.val[1] = -100 * motor.cValue[1];  // 1.00
  // motor.val[2] = 0 * motor.cValue[2];
  // motor.directDrive(motor.val);

  // step2
  motor.val[0] = 100 * motor.cValue[0];
  motor.val[1] = 0 * motor.cValue[1];
  motor.val[2] = -100 * motor.cValue[2];
  motor.directDrive(motor.val);

  // // step3
  // motor.val[0] = 0 * motor.cValue[0];
  // motor.val[1] = -100 * motor.cValue[1];
  // motor.val[2] = 100 * motor.cValue[2];
  // motor.directDrive(motor.val);
}

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

void _motor::directDrive(int* p) {
  if (*p == 0) {
    PORTB |= _BV(4) | _BV(5);

    digitalWrite(12, HIGH);
  } else if (*p < 0) {
    PORTB |= _BV(4);
    PORTB &= ~(_BV(5));

    if (*p >= 100) {
      digitalWrite(12, HIGH);
    } else {
      analogWrite(12, constrain(abs(round((float)*p * 2.55)), 0, 255));
    }
  } else if (*p > 0) {
    PORTB &= ~(_BV(4));
    PORTB |= _BV(5);

    if (*p <= -100) {
      digitalWrite(12, HIGH);
    } else {
      analogWrite(12, constrain(abs(round((float)*p * 2.55)), 0, 255));
    }
  }

  if (*(p + 2) == 0) {
    PORTG |= _BV(5);
    PORTE |= _BV(3);

    digitalWrite(6, HIGH);
  } else if (*(p + 2) < 0) {
    PORTG |= _BV(5);
    PORTE &= ~(_BV(3));

    if (*(p + 2) >= 100) {
      digitalWrite(6, HIGH);
    } else {
      analogWrite(6, constrain(abs(round((float)*(p + 2) * 2.55)), 0, 255));
    }
  } else if (*(p + 2) > 0) {
    PORTG &= ~(_BV(5));
    PORTE |= _BV(3);

    if (*(p + 2) <= -100) {
      digitalWrite(6, HIGH);
    } else {
      analogWrite(6, constrain(abs(round((float)*(p + 2) * 2.55)), 0, 255));
    }
  }

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
}
