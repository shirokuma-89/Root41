#include <Wire.h>

int process_timer;

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

  Serial.print(usonic.distance);
  Serial.print("\t");
  Serial.println(millis()-process_timer);

  Serial.flush();
}

int _usonic::getDistance(void) {
  process_timer = millis();
  Wire.begin();
  Wire.requestFrom(8, 1);
  Wire.flush();

  timeOut = millis();
  while (timeOut + 5 >= millis()) {
    if (Wire.available()) {
      data = Wire.read();

      break;
    }
  }

  //受信バッファをクリア
  while (Wire.available()) {
    Wire.read();
  }
  Wire.flush();

  return (int)data;
}