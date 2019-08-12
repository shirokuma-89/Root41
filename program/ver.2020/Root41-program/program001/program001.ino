//ライブラリのインクルード
#include <Adafruit_NeoPixel.h>
#include <EEPROM.h>
#include <FaBoLCDmini_AQM0802A.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Timer5.h>
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

//インスタンス作成
Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel(16, 28, NEO_GRB + NEO_KHZ800);
FaBoLCDmini_AQM0802A lcd;

class _ball {
 public:
  void read(int* b);
  void calc(void);

  int val[16];
  int top;
  int deg;
  int dist;

 private:
  int _top;
} ball;

class _line {
 public:
  _line(void);
  void read(void);
  void process(void);
  bool flag;
  bool val[20];
  bool logs[20];

  int num = 0;
  int space = 0;
  int number = 0;

  int count = 0;

  float deg = 1000;

 private:
  bool _flag;
} line;

class _motor {
 public:
  _motor(void);

  void directDrive(int* p);
  void drive(int _deg, int _power, bool _stop = false);

  int val[3];

  unsigned long moveTimer = 0;

 private:
  int front = 0;
  int _front;
  float Kp;
  float Ki;
  float Kd;

  int correctionVal = 0;
  int integral = 0;
} motor;

class _gyro {
 public:
  int read(void);
  int differentialRead(void);
  void setting(void);

  int deg;
  int differentialDeg = 0;
  int eeprom[6];
  int offset;

 private:
  // none
} gyro;

class _device {
 public:
  void initialize(void);
  void check(void);

  bool robot;  // trueが宮里

  int process = LOW;
  int mode = 0;

 private:
} device;

class _LED {
 public:
  void gyroShow(unsigned long _color = 'hogehoge');
  void changeAll(int red, int green, int blue);
  void changeAll(unsigned long _color);

  bool white = false;

  int bright = 150;
  int i, j;

  unsigned long defaltColor;
  unsigned long subColor;
  unsigned long RED;
  unsigned long BLUE;
  unsigned long GREEN;
  unsigned long YELLOW;
  unsigned long WHITE;

  unsigned long timer;

 private:
  // none
} LED;

void setup(void) {
  device.initialize();
  device.mode = 0;

  Serial.begin(115200);

  Wire.begin();

  gyro.setting();
  gyro.read();

  //起動イルミネーション
  for (int i = 0; i <= 15; i++) {
    RGBLED.begin();
    RGBLED.setBrightness(LED.bright);

    LED.changeAll(0, 0, 0);

    for (int k = 0; k <= i; k++) {
      RGBLED.setPixelColor(k, LED.WHITE);
    }

    RGBLED.show();

    delay(15);
  }
  gyro.read();

  delay(500);

  gyro.read();

  // startTimer5(50);
}

void loop(void) {
  device.check();

  if (device.mode == 0) {
    gyro.deg = gyro.read();

    LED.gyroShow();

    motor.drive(NULL, NULL, true);

    Serial.println(gyro.deg);
  } else if (device.mode == 1) {
    if (!line.flag) {
      LED.gyroShow(LED.subColor);
      ball.read(ball.val);
      ball.calc();

      if (ball.val[ball.top] <= 570) {
        motor.moveTimer = millis();
        while (millis() - motor.moveTimer <= 20) {
          if (!line.flag) {
            line.process();
            motor.drive(ball.deg, 100);
            if (millis() - motor.moveTimer >= 10) {
              digitalWrite(BALL_RESET, HIGH);
            }
          } else {
            break;
          }
        }
      } else {
        LED.changeAll(LED.GREEN);
        while (millis() - motor.moveTimer <= 60) {
          if (!line.flag) {
            line.process();
            motor.drive(NULL, NULL);
            if (millis() - motor.moveTimer >= 10) {
              digitalWrite(BALL_RESET, HIGH);
            }
          } else {
            break;
          }
        }
      }
    } else {
      LED.changeAll(LED.YELLOW);
      motor.drive(line.deg, 100);
      Serial.print(line.logs[0]);
      Serial.print("\t");
      Serial.println(line.num);
      line.process();
    }
  }
}
