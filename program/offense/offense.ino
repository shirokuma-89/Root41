//ライブラリのインクルード
#include <Adafruit_NeoPixel.h>
#include <FaBoLCDmini_AQM0802A.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Root41_Lib.h>
#include <Rotary.h>
#include <Timer5.h>
#include <Wire.h>

#define ROBOT 2  // 1:宮里　2:久留

#if ROBOT == 1

#define Gyro_X 87
#define Gyro_Y -66
#define Gyro_Z 70
#define Accel_X -4775
#define Accel_Y -2060
#define Accel_Z 1456

#else  //久留君

#define Gyro_X 91
#define Gyro_Y 43
#define Gyro_Z -39
#define Accel_X -447
#define Accel_Y -646
#define Accel_Z 1647

#endif

//ピン番号定義
#define BALL0 1
#define BALL1 0
#define BALL2 3
#define BALL3 2
#define BALL4 5
#define BALL5 4
#define BALL6 7
#define BALL7 6
#define BALL8 9
#define BALL9 8
#define BALL10 11
#define BALL11 10
#define BALL12 13
#define BALL13 12
#define BALL14 15
#define BALL15 14
#define BALL_RESET 22

#define LINE1 24
#define LINE2 25
#define LINE3 26
#define LINE4 27

#define SW_TOGGLE 28
#define SW_RIGHT 29
#define SW_LEFT 30

#define ROTARY1 31
#define ROTARY2 32

#define RGBLED_OUTPIN 23
#define NUMRGBLED 16

//インスタンス作成
Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel(16, 23, NEO_GRB + NEO_KHZ800);
FaBoLCDmini_AQM0802A lcd;
Rotary rotary = Rotary(ROTARY1, ROTARY2);
Root41_Lib Root41;

class _ball {
 public:
  void read(int* b);
  void reset(void);
  void calc(void);

  bool exist = true;

  int val[16];
  int top;
  int deg;
  int old_top;
  int top_backup;

  const int move = 20;

  float x;
  float y;

  unsigned long resetTimer = 0;
  unsigned long resettingTimer = 0;

 private:
} ball;

class _line {
 public:
  _line(void);
  void read(void);

  bool flag = false;
  bool near = false;
  bool touch = false;
  bool val[4];

  int deg = 1000;
  int outMove = 1000;
  int mode = 0;
  int offset = 0;

  int allval = 0;

  int first = 5;
  int second = 5;

  unsigned long inTimer;
  unsigned long outTimer;

 private:
} line;

class _motor {
 public:
  _motor(void);
  void directDrive(int* p);
  void drive(int _deg,
             int _power,
             bool stop = false,
             bool correctionDeg = false);

  bool correction = true;

  int val[3];

  int deg;
  int power;
  int move = 15;

  unsigned long moveTimer;

 private:
  int correctionVal;
  int integral = 0;

  float front;

  unsigned long integralTimer = 0;

} motor;

class _gyro {
 public:
  int read(void);
  int differentialRead(void);
  void setting(void);

  int deg;
  int differentialDeg = 0;
  int offset;

 private:
  // none
} gyro;

class _usonic {
 public:
  int getDistance(void);

  int distance = 0;

 private:
  byte data;

  unsigned long timeOut;

} usonic;

class _LCD {
 public:
  void init(void);

  int output = 0;

  unsigned long timer = 0;

 private:
  byte character1[8];
  byte character2[8];
  byte character3[8];
  byte character4[8];
  byte character5[8];
  byte character6[8];
  byte character7[8];
  byte character8[8];

} LCD;

class _device {
 public:
  void initialize(void);
  void monitor(void);

  bool boot = true;
  bool monitorBegin = false;
  bool error = false;

  int process = HIGH;
  int mode = 0;
  int rotary = 0;
  int rotaryResult = 0;
  int errorCode = 0;

 private:
  // none
} device;

class _LED {
 public:
  void gyroShow(void);
  void lineShow(void);
  void ballShow(int deg);
  void changeAll(int red, int green, int blue);

  int bright = 60;
  int i, j;

  unsigned long timer;

 private:
  // none
} LED;

class _kickoff {
 public:
  int deg = 0;
  int go = 0;

  long moveTime;

 private:
  // none
} kickoff;

void setup(void) {
  device.initialize();
  device.mode = 0;

  Serial.begin(115200);

  Wire.begin();
  TWBR = 12;  // ATtiny85との通信エラーが起きるならコメントアウトする

  LCD.init();
  lcd.print("Root41 starting");
  LCD.output = 0;

  //起動イルミネーション
  for (int i = 0; i <= 15; i++) {
    RGBLED.begin();
    RGBLED.setBrightness(LED.bright);

    LED.changeAll(0, 0, 0);

    for (int k = 0; k <= i; k++) {
      RGBLED.setPixelColor(k, 255, 255, 255);
    }

    RGBLED.show();
    if (digitalRead(SW_LEFT) && digitalRead(SW_RIGHT)) {
      device.monitorBegin = true;
    }
    delay(5);
  }

  //ジャイロセンサをセットアップ
  gyro.setting();
  delay(50);
  gyro.offset += gyro.read();  //エラー値を保存

  lcd.clear();

  device.monitor();

  startTimer5(50);
}

void loop(void) {
  Serial.print(line.deg);
  Serial.print(" ");
  Serial.println(line.outMove);

  device.error = false;

  gyro.deg = gyro.read();

  if (line.touch) {
    line.offset = gyro.deg;
  } else {
    line.offset = 0;
  }

  RGBLED.begin();
  RGBLED.setBrightness(LED.bright);

  if (digitalRead(SW_TOGGLE) && !device.boot) {
    device.mode = 2;

    motor.move = 10;

    ball.reset();

    motor.power = 100;

    // ボール処理
    ball.read(ball.val);
    if (ROBOT == 2) {
      //久留マシン設定
      ball.val[4] = (ball.val[3] + ball.val[5]) / 2;
      ball.val[12] = (ball.val[11] + ball.val[13]) / 2;
    }
    ball.calc();
    // Serial.println(ball.top);

    LED.changeAll(0, 135, 255);

    if (!line.flag) {
      if (ball.exist) {
        motor.deg = ball.deg;
      } else {
        motor.deg = 1000;
      }
    } else {
      if (line.deg != 1000) {
        motor.deg = line.deg;
      } else if (line.outMove != 1000) {
        motor.deg = line.outMove;
      } else {
        device.error = true;
        device.errorCode = 1;
      }
    }
    if (motor.deg != 1000) {
      if (line.flag) {
        LED.lineShow();
        motor.drive(motor.deg, 100);
      } else {
        LED.ballShow(motor.deg);

        motor.correction = true;

        motor.moveTimer = millis();
        while (millis() - motor.moveTimer <= motor.move) {
          if (!line.flag) {
            motor.drive(motor.deg, motor.power);
          } else {
            break;
          }
        }
      }
    } else {
      motor.drive(NULL, NULL, false, true);

      if (line.near) {
        LED.changeAll(135, 0, 255);
      } else {
        LED.changeAll(0, 255, 0);
      }
    }

    // LCD表示
    if (!device.error) {
      if (device.process == HIGH) {
        if (LCD.output != 2) {
          lcd.clear();

          lcd.print("Root41 running");

          lcd.setCursor(0, 1);  //改行

          lcd.print("No problem");

          LCD.output = 2;
        }
      } else {
        if (millis() - LCD.timer >= 100) {
          lcd.clear();

          lcd.print("Root41 running");

          lcd.setCursor(0, 1);  //改行

          lcd.print(gyro.deg);
          lcd.print(" deg");

          LCD.output = 2;
          LCD.timer = millis();
        }
      }
    }

    if (device.error) {
      LED.changeAll(255, 0, 0);

      if (millis() - LCD.timer >= 100) {
        lcd.clear();

        lcd.print("Root41 running");

        lcd.setCursor(0, 1);  //改行

        lcd.print("ErrorCode = ");
        lcd.print(device.errorCode);

        LCD.output = 3;
        LCD.timer = millis();
      }
    }
  } else {
    device.mode = 1;

    motor.drive(NULL, NULL, true);

    //起動エラーを検知
    if (!digitalRead(SW_TOGGLE)) {
      device.boot = false;
    }

    if (!device.boot) {
      if (ROBOT == 1) {
        LED.changeAll(255, 135, 0);
      } else {
        LED.changeAll(100, 255, 255);
      }
    } else {
      LED.changeAll(0, 0, 0);
    }

    LED.gyroShow();

    if (millis() - LCD.timer >= 300) {
      // usonic.distance = usonic.getDistance();

      lcd.clear();

      if (!device.boot) {
        lcd.print("Root41 waiting");
      } else {
        lcd.print("Root41 Boot ERR!");
      }

      lcd.setCursor(0, 1);  //改行

      lcd.print(gyro.deg);
      lcd.print(" deg");

      lcd.setCursor(9, 1);  //改行

      // lcd.print(usonic.distance);
      // lcd.print(" cm");

      // lcd.print(gyro.differentialRead());
      // lcd.print(" deg");

      LCD.output = 1;
      LCD.timer = millis();
    }

    //ジャイロセンサリセット
    if (digitalRead(SW_LEFT) && digitalRead(SW_RIGHT)) {
      RGBLED.begin();
      RGBLED.setBrightness(LED.bright);

      LED.changeAll(255, 0, 0);

      RGBLED.show();

      lcd.clear();
      lcd.print("gyro sensor");
      lcd.setCursor(0, 1);  //改行
      lcd.print("initializing...");

      gyro.setting();
      delay(50);
      gyro.offset += gyro.read();
    }
  }

  RGBLED.show();
}