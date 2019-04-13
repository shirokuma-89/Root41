#include <TinyWireS.h>
#define pingPin1 1
#define pingPin2 3
#define pingPin3 4

byte usonic[] = {0, 0, 0, 0};
float _go;
int go[] = {0, 0};
int wgo[] = {0, 0, 0, 0};

void setup() {
  TinyWireS.begin(8);
  TinyWireS.onRequest(requestEvent);
}

void loop() {
  int cm[4];
  cm[1] = UsonicMeasurRead(pingPin1);  // センサーから距離を調べる
  if (cm[1] > 255) {
    usonic[1] = 255;
  } else {
    usonic[1] = cm[1];
  }
}

void requestEvent() {
  TinyWireS.send(byte(usonic[1]));
}

int UsonicMeasurRead(int pin) {
  long t;
  int ans;

  // 超音波センサーに5usのパルスを出力する
  pinMode(pin, OUTPUT);  // ピンを出力モードにする
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pin, LOW);
  // センサーからの反射パルスを受信する
  pinMode(pin, INPUT);     // ピンを入力モードにする
  t = pulseIn(pin, HIGH);  // パルス幅の時間を測る
  if (t < 18000) {         // 3m以上の距離は計算しない
    ans = (t / 29) / 2;    // 往復なので2で割る
  } else
    ans = 0;

  return ans;
}