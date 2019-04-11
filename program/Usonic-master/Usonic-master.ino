#include <Wire.h>

class _usonic {
 public:
  int getDistance(void);

  int distance = 0;
  unsigned long timer;

 private:
  byte data;

  unsigned long timeOut;

} usonic;

void setup(void) {
  Serial.begin(115200);

  Wire.begin();
  TWBR = 12;
}

void loop(void) {
  usonic.distance = usonic.getDistance();

  Serial.println(usonic.distance);
}

int _usonic::getDistance(void) {
  Wire.begin();
  Wire.requestFrom(8, 1);

  timeOut = millis();
  while (timeOut + 5 >= millis()) {
    if (Wire.available()) {
      data = Wire.read();

      break;
    }
  }

  //受信バッファをクリア
  while (Serial.available()) {
    Serial.read();
  }
  Serial.flush();

  return (int)data;
}