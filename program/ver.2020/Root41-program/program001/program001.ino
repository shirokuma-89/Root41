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

//インスタンス作成
Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel(16, 28, NEO_GRB + NEO_KHZ800);
FaBoLCDmini_AQM0802A lcd;
VL53L0X TOF;

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
  int _deg;
  int exCount = true;

  float x;
  float y;

  unsigned long holdTimer;
} ball;

class _line {
 public:
  _line(void);
  void read(void);
  void process(void);

  bool flag;
  bool val[20];
  bool touch;

  float deg = 1000;

  int whited;
  int logs[20];
  int first;
  int last;
  int mode;
  int weight = 10;
  int sigdeg;
  int _deg;

  float vector[20][2];
  float x;
  float y;
  float offsetX = 1.3;
  float offsetY = 1;

  unsigned long stopTimer;
  unsigned long overTimer;
  unsigned long _millis;

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
  int integral = 0;

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

class _tof {
 public:
  int read(void);
  int dist;

 private:
} tof;

class _device {
 public:
  void initialize(void);
  void check(void);
  void buz(void);
  void mute(void);

  bool robot;
  bool keeper;

  int process = LOW;
  int mode = 0;

  unsigned long buzTimer1 = 0;
  unsigned long buzTimer2 = 0;

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
  device.buz();
  device.mode = 0;

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("STARTING...");
  lcd.setCursor(0, 1);
  delay(7);
  device.mute();

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
    line.vector[i][0] = sin(radians(i * 18)) * line.offsetX;
    line.vector[i][1] = cos(radians(i * 18)) * line.offsetY;
  }

  delay(500);

  gyro.read();
  startTimer5(50);
}

void loop(void) {
  device.check();

  if (device.mode == 0) {  //待機中
    if (millis() - device.buzTimer2 <= 20) {
      device.buz();
    } else {
      device.mute();
    }
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
      lcd.setCursor(9, 1);
      lcd.print("mm");

      LCD.timer = millis();
      LCD.output = 1;
    }
    device.buzTimer1 = millis();
  } else if (device.mode == 1) {
    ball.read(ball.val);
    ball.calc();
    line.read();
    line.process();
    tof.dist = tof.read();

    if (millis() - device.buzTimer1 <= 20) {
      device.buz();
    } else {
      device.mute();
    }
    //駆動
    if (line.flag) {
      device.buz();
      motor.moveTimer = millis();
      LED.degShow(line.deg, LED.PURPLE);
      if (line.deg == 1000) {
        motor.drive(NULL, NULL, true);
      } else {
        motor.drive(line.deg, 100);
      }
    } else if (ball.exist) {
      device.mute();
      motor.moveTimer = millis();
      if (ball.hold) {
        LED.changeAll(LED.subColor);
      } else {
        if (ball.emg) {
          LED.degShow(ball.deg, LED.YELLOW);
        } else if (ball.turn) {
          LED.degShow(ball.deg, LED.GREEN);
        } else {
          LED.degShow(ball.deg);
        }
      }
      while (millis() - motor.moveTimer <= 15) {
        if (millis() - device.buzTimer1 <= 20) {
          device.buz();
        } else {
          device.mute();
        }
        line.read();
        line.process();
        if (line.flag) {
          break;
        }
        motor.drive(ball.deg, ball.speed);
        if (millis() - motor.moveTimer >= 3) {
          digitalWrite(BALL_RESET, HIGH);
        }
      }
    } else {
      LED.changeAll(LED.PURPLE);
      motor.moveTimer = millis();
      while (millis() - motor.moveTimer <= 20) {
        if (millis() - device.buzTimer1 <=20) {
          device.buz();
        } else {
          device.mute();
        }
        line.read();
        line.process();
        if (line.flag) {
          break;
        }
        motor.drive(NULL, NULL);
        // motor.drive(90, ball.speed);
        if (millis() - motor.moveTimer >= 5) {
          digitalWrite(BALL_RESET, HIGH);
        }
      }
    }

    // LCD
    if (device.process == LOW) {
      if (millis() - LCD.timer >= 200) {
        lcd.clear();
        if (!device.keeper) {
          lcd.print("RUNNING! OFFENCE");
        } else {
          lcd.print("RUNNING! KEEPER");
        }

        lcd.setCursor(0, 1);
        if (LCD.unit != "NULL") {
          lcd.print("INFO:");
          lcd.print(line.x);
          lcd.print(line.y);
          // lcd.print(LCD.unit);
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
    device.buzTimer2 = millis();
  }
  Serial.print(ball.top);
  Serial.print(" ");
  Serial.print(ball.val[ball.top]);
  Serial.print(" ");
  Serial.println("");
}
