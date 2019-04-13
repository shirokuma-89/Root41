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
  cm[2] = UsonicMeasurRead(pingPin2);  // センサーから距離を調べる
  if (cm[2] > 255) {
    usonic[2] = 255;
  } else {
    usonic[2] = cm[2];
  }
  cm[3] = UsonicMeasurRead(pingPin3);  // センサーから距離を調べる
  if (cm[3] > 255) {
    usonic[3] = 255;
  } else {
    usonic[3] = cm[3];
  }
  _go = usonic[1] / 5;
  wgo[1] = ceil(_go);
  wgo[1] = constrain(wgo[1], 0, 31);
  wgo[1] = wgo[1] << 3;

  _go = usonic[2] / 5;
  wgo[2] = ceil(_go);
  wgo[2] = constrain(wgo[2], 0, 31);
  wgo[0] = wgo[2] & 3;
  wgo[0] = wgo[0] << 6;
  wgo[2] = wgo[2] >> 2;

  _go = usonic[3] / 5;
  wgo[3] = ceil(_go);
  wgo[3] = constrain(wgo[3], 0, 31);
  wgo[3] = wgo[3] << 1;

  go[0] = wgo[1] + wgo[2];
  go[1] = wgo[0] + wgo[3];
}

void requestEvent() {
  TinyWireS.send(byte(go[0]));
  TinyWireS.send(byte(go[1]));
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