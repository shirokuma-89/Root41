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
  int speed = 100;
  int top;
  int deg;
  int dist;

  bool exist;
  bool hold = false;
  bool turn;
  bool emg;

 private:
  int _top;
  int exCount = true;

  float x;
  float y;
} ball;

class _line {
 public:
  _line(void);
  void read(void);
  void process(void);

  bool flag;
  bool val[20];
  bool logs[20];
  bool touch;

  float deg = 1000;

  int whited;
  int mode;
  int newv;
  int order[20] = {100, 100, 100, 100, 100, 100, 100, 100, 100, 100,
                   100, 100, 100, 100, 100, 100, 100, 100, 100, 100};

  float plus[20][2];
  float x;
  float y;

  unsigned long stopTimer;
  unsigned long overTimer;

 private:
  bool _flag;
} line;

class _motor {
 public:
  _motor(void);

  void directDrive(int* p);
  void drive(int _deg, int _power, bool _stop = false);

  int val[3];

  const int move = 60;

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
  bool pickUp(void);
  void setting(void);

  int deg;
  int differentialDeg = 0;
  int eeprom[6];
  int offset;

  bool pickUpAns = false;

 private:
  unsigned long pickUpTimer;
  bool p_az;
} gyro;

class _device {
 public:
  void initialize(void);
  void check(void);

  bool robot;

  int process = LOW;
  int mode = 0;

 private:
} device;

class _LED {
 public:
  void gyroShow(unsigned long _color = 'hogehoge');
  void changeAll(int red, int green, int blue);
  void changeAll(unsigned long _color);
  void degShow(int d, unsigned long _color = 'hogehoge');

  bool white = false;

  int bright = 150;

  unsigned long defaltColor;
  unsigned long subColor;
  unsigned long RED;
  unsigned long BLUE;
  unsigned long GREEN;
  unsigned long YELLOW;
  unsigned long WHITE;
  unsigned long PURPLE;
  unsigned long LIME;

  unsigned long timer;

 private:
  // none
} LED;

class _LCD {
 public:
  int output = 0;

  int data = NULL;
  String unit = "NULL";

  unsigned long timer;

 private:
} LCD;

void setup(void) {
  device.initialize();
  device.mode = 0;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("STARTING...");
  lcd.setCursor(0, 1);

  Serial.begin(115200);

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

  for (int i = 0; i <= 19; i++) {
    line.plus[i][0] = sin(i * 18 * 0.0174533);
    line.plus[i][1] = cos(i * 18 * 0.0174533);
  }

  delay(500);

  gyro.read();
}

void loop(void) {
  device.check();

  if (device.mode == 0) {  //待機中
    gyro.deg = gyro.read();

    LED.gyroShow();

    motor.drive(NULL, NULL, true);

    if (millis() - LCD.timer >= 200) {
      lcd.clear();
      lcd.print("WAITING...");

      lcd.setCursor(0, 1);
      lcd.print("DEG:");
      lcd.print(gyro.deg);
      lcd.write(B11011111);

      LCD.timer = millis();
      LCD.output = 1;
    }
  } else if (device.mode == 1) {
    ball.read(ball.val);
    ball.calc();
    line.read();
    line.process();

    //駆動
    if (line.flag) {
      motor.moveTimer = millis();
      while (millis() - motor.moveTimer <= 15) {
        LED.degShow(line.deg, LED.GREEN);
        motor.drive(line.deg, 100);
      }
    } else if (ball.exist) {
      motor.moveTimer = millis();
      while (millis() - motor.moveTimer <= 15) {
        if (ball.hold) {
          LED.changeAll(LED.subColor);
        } else {
          if (ball.emg) {
            LED.degShow(ball.deg, LED.LIME);
          } else if (ball.turn) {
            LED.degShow(ball.deg, LED.GREEN);
          } else {
            LED.degShow(ball.deg);
          }
        }
        motor.drive(ball.deg, ball.speed);
        if (millis() - motor.moveTimer >= 2) {
          digitalWrite(BALL_RESET, HIGH);
        }
      }
    } else {
      LED.changeAll(LED.PURPLE);
      motor.moveTimer = millis();
      while (millis() - motor.moveTimer <= 20) {
        motor.drive(NULL, NULL);
        if (millis() - motor.moveTimer >= 5) {
          digitalWrite(BALL_RESET, HIGH);
        }
      }
    }

    // LCD
    if (device.process == LOW) {
      if (millis() - LCD.timer >= 200) {
        lcd.clear();
        lcd.print("RUNNING! Offence");

        lcd.setCursor(0, 1);
        if (LCD.unit != "NULL") {
          lcd.print("INFO:");
          lcd.print(LCD.data);
          lcd.print(LCD.unit);
        }

        LCD.timer = millis();
        LCD.output = 1;
      }
    } else {
      if (LCD.output != 1) {
        lcd.clear();
        lcd.print("RUNNING!");
        LCD.output = 1;
      }
    }
  }
}
