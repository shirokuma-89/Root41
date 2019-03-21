// Usonic.ino

#include <TinyWireS.h>

#define PIN1 1

class _usonic {
 public:
  byte read(void);

  byte distance;

 private:
  unsigned long timer;

  int ans;

} usonic;

void setup(void) {
  TinyWireS.begin(8);
  TinyWireS.onRequest(requestEvent);
}

void loop(void) {
  usonic.distance = usonic.read();
}

byte _usonic::read(void) {
  // 超音波に5usのパルスを出力
  pinMode(PIN1, OUTPUT);  // ピンを出力モードにする
  digitalWrite(PIN1, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN1, HIGH);
  delayMicroseconds(5);
  digitalWrite(PIN1, LOW);

  pinMode(PIN1, INPUT);
  timer = pulseIn(PIN1, HIGH);  // パルス幅を計測

  if (timer < 18000) {
    //距離を計算
    ans = int(((float)timer / 29.0) / 2.0);
  } else {
    // error!
    ans = 0;
  }

  return ans;
}

void requestEvent() {
  TinyWireS.send((byte)usonic.distance);
}