// NeoPixelを簡単に使うためのプログラムを読み込み
#include <Adafruit_NeoPixel.h>

// NeoPixelを接続したPIN番号を設定
#define PIN 28
// NeoPixelのLEDの数を設定
#define NUMPIXELS 16
// NeoPixelを使うためのオブジェクト生成
// このプログラム内で "pixels" という名前で使えるようにしている
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
  // NeoPixelの初期設定
  //  - pinMode() に相当する命令
  pixels.begin();
  pixels.setBrightness(255);
  pixels.show();

}
void loop() {
  // while は繰り返しの命令
  // i の値が NUMPIXELS 未満の間は繰り返す
  int i = 0;
  while ( i < NUMPIXELS ) {
    // Ringの i番目 の LED を赤く光るようにセット
    pixels.begin();
    pixels.clear();
    pixels.setBrightness(255);
    pixels.setPixelColor(i, 255, 0, 0);

    // セットしたものを反映し実際に光らせる
    pixels.show();
    delay(300);

    // 次の LED を光らせるために i の数を 1つ 増やす
    i = i + 1;
  }
  pixels.begin();
  pixels.clear();
  pixels.show();
  i = 0;
  while ( i < NUMPIXELS ) {
    // Ringの i番目 の LED を赤く光るようにセット
    pixels.begin();
    pixels.clear();
    pixels.setBrightness(255);
    pixels.setPixelColor(i, 0, 255, 0);

    // セットしたものを反映し実際に光らせる
    pixels.show();
    delay(300);
    // 次の LED を光らせるために i の数を 1つ 増やす
    i = i + 1;
  }
  pixels.begin();
  pixels.clear();
  pixels.show();
  i = 0;
  while ( i < NUMPIXELS ) {
    // Ringの i番目 の LED を赤く光るようにセット
    pixels.begin();
    pixels.clear();
    pixels.setPixelColor(i, 0, 0, 255);

    // セットしたものを反映し実際に光らせる
    pixels.show();
    delay(300);


    // 次の LED を光らせるために i の数を 1つ 増やす
    i = i + 1;
  }
  pixels.begin();
  pixels.clear();
  pixels.show();
  i = 0;
  while ( i < NUMPIXELS ) {
    // Ringの i番目 の LED を赤く光るようにセット
    pixels.begin();
    pixels.clear();
    pixels.setPixelColor(i, 255, 255, 255);

    // セットしたものを反映し実際に光らせる
    pixels.show();
    delay(300);

    // 全てのLEDを消灯


    // 次の LED を光らせるために i の数を 1つ 増やす
    i = i + 1;
  }
  pixels.begin();
  pixels.clear();
  pixels.show();
}
