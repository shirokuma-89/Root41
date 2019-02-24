//ライブラリのインクルード
#include <Adafruit_NeoPixel.h>
#include <FaBoLCDmini_AQM0802A.h>
#include <MPU6050_6Axis_MotionApps20.h>
#include <Root41_Lib.h>
#include <Rotary.h>
#include <Timer5.h>
#include <Wire.h>

//ジャイロセンサのオフセット値
#define Gyro_X 87
#define Gyro_Y -66
#define Gyro_Z 70
#define Accel_X -4775
#define Accel_Y -2060
#define Accel_Z 1456

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
#define LINE_STOP 70

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

//クラス定義＆インスタンス作成
class _ball {
 public:
  void read(int* b);

  bool Reset = true;
  bool exist = true;

  int move = 15;
  int old_top;
  int val[16];
  int top;
  int deg;
  int top_backup;

  float x;
  float y;

  unsigned long timer = 0;
  unsigned long reset = 0;

 private:
} ball;

class _line {
 public:
  _line(void);
  void read(void);
  void showLED(void);

  bool flug = false;
  bool lock = false;
  bool old_val[4];
  bool near = false;
  bool on = true;
  bool start = false;
  bool stop = false;
  bool touch = false;
  bool val[4];
  bool log_val[4];
  bool tell = false;
  bool yokoTell = false;
  bool beginning = true;

  const bool show = true;

  int pin[4] = {24, 25, 26, 27};
  int wait = 1000;
  int deg = 1000;
  int stopDeg;
  int mode = 0;
  int mae = 0;
  int yoko = 0;
  int offset = 0;
  int in = 5;
  int maeNuri = 0;
  int yokoNuri = 0;
  int hold;
  int count;
  int sakki;

  unsigned long timer;
  unsigned long sub_timer;
  unsigned long timer2;
  unsigned long nearTimer;
  unsigned long yokoTimer;
  unsigned long maeTimer;

 private:
} line;

class _motor {
 public:
  _motor(void);
  void directDrive(int* p);
  void drive(int _deg, int _power, bool stop = false, bool correctDeg = false);

  bool on = true;
  bool correct = true;

  int val[3];
  int motor[5];
  int deg = 1000;
  int power;
  int old_gyro;
  int valAbs[3];
  int deg_integral = 0;

  float front;

  unsigned long correctTimer = 0;
  unsigned long timer = 0;
  unsigned long reset = 0;

 private:
  int _val[3];
} motor;

class _gyro {
 public:
  int read(void);
  int dmpGetGyro(void);
  void setting(void);
  void show(void);

  int deg;
  int dmpGyro = 0;
  int offset;

 private:
  // none
} gyro;

class _LCD {
 public:
  void init(void);

  byte out = 0;
  byte character1[8];
  byte character2[8];
  byte character3[8];
  byte character4[8];
  byte character5[8];
  byte character6[8];
  byte character7[8];
  byte character8[8];

  unsigned long timer = 0;

 private:
  // none
} LCD;

class _device {
 public:
  void initialize(void);
  void monitor(void);
  void rainbowLED(void);

  bool boot = false;
  bool monitorSend = false;

  int process = HIGH;  // UIを抑制
  int mode = 0;
  int RGB_bright = 60;
  int rotary = 0;
  int rotaryResult = 0;

  unsigned long timer;

 private:
  // none
} device;

class _kickoff {
 public:
  int deg = 0;
  int go = 0;

  long ledTimer;
  long moveTime;
  bool ledShow = false;

 private:
  // none
} kickoff;

unsigned long timer;

void setup() {
  device.mode = 0;

  Serial.begin(115200);

  Wire.begin();
  TWBR = 12;

  if (digitalRead(SW_LEFT) && digitalRead(SW_RIGHT)) {
    device.monitorSend = true;
  }

  //セットアップ
  LCD.init();
  lcd.print("Root41 starting");
  for (int i = 0; i <= 15; i++) {
    RGBLED.begin();
    RGBLED.setBrightness(device.RGB_bright);

    for (int j = 0; j <= 15; j++) {
      RGBLED.setPixelColor(j, 0, 0, 0);
    }
    for (int k = 0; k <= i; k++) {
      RGBLED.setPixelColor(k, 255, 255, 255);
    }

    RGBLED.show();
    if (digitalRead(SW_LEFT) && digitalRead(SW_RIGHT)) {
      device.monitorSend = true;
    }
    delay(10);
  }

  gyro.setting();
  delay(100);
  gyro.offset += gyro.read();

  device.initialize();

  lcd.clear();
  LCD.out = 0;

  device.monitor();

  startTimer5(50);  //割り込みを有効化
}

void loop() {
  if (digitalRead(28) && device.boot) {
    //変数の初期化
    device.mode = 2;
    ball.move = 20;

    //ボールセンサリセット処理
    if (millis() - ball.reset >= 1000) {
      digitalWrite(BALL_RESET, LOW);
      ball.timer = millis();
      while (millis() - ball.timer <= 10) {
        Serial.println(ball.exist);
        if (line.deg == 1000 && line.wait == 1000) {
          if (ball.exist) {
            motor.drive(motor.deg, motor.power);
          } else {
            motor.drive(NULL, NULL, true, true);
          }
        } else {
          break;
        }
      }
      digitalWrite(BALL_RESET, HIGH);
      ball.timer = millis();
      while (millis() - ball.timer <= 10) {
        Serial.println(ball.exist);
        if (line.deg == 1000 && line.wait == 1000) {
          if (ball.exist) {
            motor.drive(motor.deg, motor.power);
          } else {
            motor.drive(NULL, NULL, true, true);
          }
        } else {
          break;
        }
      }
      ball.reset = millis();
      ball.move -= 15;
    }

    // LED制御
    RGBLED.begin();
    RGBLED.setBrightness(device.RGB_bright);

    for (int i = 0; i <= 15; i++) {
      RGBLED.setPixelColor(i, 30, 150, 255);
    }

    timer = millis();  //タイマー

    motor.deg = ball.deg;

    //キックオフゴール
    if (millis() - kickoff.moveTime <= 430) {
      if (kickoff.go == 1 || kickoff.go == 2) {
        kickoff.go += 5;
      }
      if (kickoff.go == 6) {
        motor.deg = 1000;
      } else if (kickoff.go == 7) {
        motor.deg = 1000;
      }
      motor.power = 90;
    }


    if (line.deg == 1000 && line.wait == 1000) {
      line.flug = false;
      line.mae = 0;
      line.lock = false;
      motor.correct = true;
      line.tell = 0;
      line.offset = 0;
    } else {
      line.flug = true;
      if (line.deg != 1000) {
        motor.deg = line.deg;
      } else {
        motor.deg = line.wait;
      }
      if (line.beginning) {
        line.offset = gyro.read();
        line.beginning = false;
      }
    }

    //モーター駆動
    if (ball.exist || line.flug) {
      if (line.flug) {
        motor.timer = millis();
        motor.drive(motor.deg, 100);
        {
          for (int i = 0; i <= 15; i++) {
            if (line.lock) {
              RGBLED.setPixelColor(i, 255, 255, 0);
            } else {
              RGBLED.setPixelColor(i, 0, 0, 255);
            }
          }
          if (line.hold == 0) {
            RGBLED.setPixelColor(0, 255, 0, 0);
            RGBLED.setPixelColor(1, 255, 0, 0);
            RGBLED.setPixelColor(2, 255, 0, 0);
            RGBLED.setPixelColor(15, 255, 0, 0);
            RGBLED.setPixelColor(14, 255, 0, 0);
          }
          if (line.hold == 1) {
            RGBLED.setPixelColor(10, 255, 0, 0);
            RGBLED.setPixelColor(11, 255, 0, 0);
            RGBLED.setPixelColor(12, 255, 0, 0);
            RGBLED.setPixelColor(13, 255, 0, 0);
            RGBLED.setPixelColor(14, 255, 0, 0);
          }
          if (line.hold == 2) {
            RGBLED.setPixelColor(2, 255, 0, 0);
            RGBLED.setPixelColor(3, 255, 0, 0);
            RGBLED.setPixelColor(4, 255, 0, 0);
            RGBLED.setPixelColor(5, 255, 0, 0);
            RGBLED.setPixelColor(6, 255, 0, 0);
          }
          if (line.hold == 3) {
            RGBLED.setPixelColor(6, 255, 0, 0);
            RGBLED.setPixelColor(7, 255, 0, 0);
            RGBLED.setPixelColor(8, 255, 0, 0);
            RGBLED.setPixelColor(9, 255, 0, 0);
            RGBLED.setPixelColor(10, 255, 0, 0);
          }
        }
        for (int i = 0; i <= 3; i++) {
          line.old_val[i] = false;
        }
        line.stop = true;
      } else {
        motor.correct = true;
        // if (line.stop) {
        //   line.stop = false;
        //   motor.drive(NULL, NULL, true);
        //   delay(500);
        // }
        line.in = 5;
        RGBLED.setPixelColor(ball.top, 255, 0, 0);
        RGBLED.setPixelColor((ball.top + 15) % 16, 255, 0, 0);
        RGBLED.setPixelColor((ball.top + 1) % 16, 255, 0, 0);

        motor.timer = millis();
        line.offset = 0;
        while (millis() - motor.timer <= ball.move && ball.exist) {
          if (line.deg == 1000 && line.wait == 1000) {
            motor.drive(motor.deg, motor.power);
          } else {
            break;
          }
        }
      }
    } else {
      line.in = 5;
      if (!line.near) {
        for (int i = 0; i <= 15; i++) {
          RGBLED.setPixelColor(i, 0, 255, 0);
        }
      } else {
        for (int i = 0; i <= 15; i++) {
          RGBLED.setPixelColor(i, 180, 0, 255);
        }
      }
      motor.drive(NULL, NULL, false, true);
    }

    RGBLED.show();

    // LCD表示
    if (device.process == LOW) {
      if (millis() - LCD.timer >= 100) {
        lcd.clear();
        lcd.print("Root41 running!");

        lcd.setCursor(0, 1);
        lcd.print(motor.val[0]);
        lcd.setCursor(5, 1);
        lcd.print(motor.val[1]);
        lcd.setCursor(10, 1);
        lcd.print(motor.val[2]);
        // lcd.print(gyro.deg);
        // lcd.print(" deg");
        // lcd.print(round(motor.front));
        // lcd.print('%');
        LCD.timer = millis();
        LCD.out = 2;
      }
    } else {
      if (LCD.out != 2) {
        lcd.clear();
        lcd.print("Root41 running!");
        LCD.out = 2;
      }
    }
  } else {
    if (!digitalRead(28)) {
      device.boot = true;
    }
    //待機中
    device.mode = 1;
    LCD.out = 1;
    line.near = false;

    gyro.deg = gyro.read();            //角度取得
    gyro.dmpGyro = gyro.dmpGetGyro();  //角速度取得
    motor.front = 0;                   //姿勢制御値をリセット

    kickoff.moveTime = millis();
    if (device.mode != 1) {
      kickoff.deg = device.rotary;
    }

    motor.drive(NULL, NULL, true);  //停止

    // LED制御
    RGBLED.begin();
    RGBLED.setBrightness(device.RGB_bright);
    for (int i = 0; i <= 15; i++) {
      RGBLED.setPixelColor(i, 0, 0, 0);
    }
    gyro.show();

    // LCD表示
    if (millis() - LCD.timer >= 200) {
      lcd.clear();
      if (device.boot) {
        lcd.print("Root41 waiting");
      } else {
        lcd.print("Root41 Boot ERR!");
      }

      lcd.setCursor(0, 1);
      // lcd.print(gyro.dmpGyro);
      lcd.print(gyro.deg);
      lcd.print(" deg");
      lcd.setCursor(8, 1);
      if (kickoff.go == 0) {
        lcd.print("CENTER");
      } else if (kickoff.go == 1) {
        lcd.print("RIGHT");
      } else if (kickoff.go == 2) {
        lcd.print("LEFT");
      } else if (kickoff.go == 5) {
        lcd.print("FALSE");
      }
      // lcd.print(" ");
      LCD.out = 1;
      LCD.timer = millis();
    }

    //キックオフゴール
    if (kickoff.ledTimer + 300 <= millis()) {
      kickoff.ledShow = !kickoff.ledShow;
      kickoff.ledTimer = millis();
    }
    if (!device.boot) {
      if (kickoff.ledShow) {
        for (int i = 0; i <= 15; i++) {
          RGBLED.setPixelColor(i, 255, 255, 255);
        }
      }
    } else if (kickoff.deg - 2 > device.rotary) {
      kickoff.go = 2;

      if (kickoff.ledShow) {
        RGBLED.setPixelColor(12, 255, 255, 255);
        RGBLED.setPixelColor((12 + 1) % 16, 255, 255, 255);
        RGBLED.setPixelColor((12 + 15) % 16, 255, 255, 255);
      }
    } else if (kickoff.deg + 2 < device.rotary) {
      kickoff.go = 1;

      if (kickoff.ledShow) {
        RGBLED.setPixelColor(4, 255, 255, 255);
        RGBLED.setPixelColor((4 + 1) % 16, 255, 255, 255);
        RGBLED.setPixelColor((4 + 15) % 16, 255, 255, 255);
      }
    } else {
      kickoff.go = 5;
    }

    //ジャイロセンサリセット
    if (digitalRead(SW_LEFT) && digitalRead(SW_RIGHT)) {
      // LED制御
      RGBLED.begin();
      RGBLED.setBrightness(device.RGB_bright);
      for (int i = 0; i <= 15; i++) {
        RGBLED.setPixelColor(i, 0, 100, 255);
      }
      RGBLED.show();

      lcd.clear();
      lcd.print("gyro sensor");
      lcd.setCursor(0, 1);
      lcd.print("initializing...");

      gyro.setting();
      delay(100);
      gyro.offset += gyro.read();
    }

    // LED表示
    RGBLED.show();
  }

  Serial.println(ball.exist);
}