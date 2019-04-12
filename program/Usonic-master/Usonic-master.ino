#include <Wire.h>

unsigned long process_timer;

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
  Serial.println(micros() - process_timer);

  Serial.flush();
}

int _usonic::getDistance(void) {
  process_timer = micros();
  Wire.begin();
  TWBR = 12;
  Wire.requestFrom(8, 1);
  Wire.flush();

  timeOut = millis();
  while (timeOut + 20 >= millis()) {
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