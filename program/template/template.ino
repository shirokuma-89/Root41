// 2019.02.27作成 template.ino
//チームでプログラム開発するときに、構造が同じほうが
//プログラムが見やすくなるから、このテンプレートをベースに
//開発しよう。コーディングはできるだけ、
//見やすくなるように努力しよう。

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
  bool exist = true;

  int val[16];
  int top;
  int deg;
  int old_top;
  int top_backup;

  float x;
  float y;

  unsigned long timer = 0;
  unsigned long resetTimer = 0;

 private:
} ball;

class _line {
 public:
  // _line(void);
  // void read(void);
  // void showLED(void);

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
  // _motor(void);
  // void directDrive(int* p);
  // void drive(int _deg, int _power, bool stop = false, bool correctDeg =
  // false);

  bool on = true;
  bool correct = true;

  int move = 15;
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
  // int read(void);
  // int dmpGetGyro(void);
  // void setting(void);
  // void show(void);

  int deg;
  int dmpGyro = 0;
  int offset;

 private:
  // none
} gyro;

class _LCD {
 public:
  // void init(void);

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
  // void initialize(void);
  // void monitor(void);
  // void rainbowLED(void);

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

void setup(void) {}

void loop(void) {}
