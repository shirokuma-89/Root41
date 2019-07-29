#include <Wire.h>

void setup() {
  Serial.begin(115200);
  Wire.begin();
}

void loop() {
  Wire.beginTransmission(8);
  Wire.write((byte)60);//data * 3 + 262
  Wire.endTransmission();
  delay(1000);
  Wire.beginTransmission(8);
  Wire.write((byte)255);//data * 3 + 262
  Wire.endTransmission();
  delay(1000);
}
