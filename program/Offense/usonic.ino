// usonic.ino

int _usonic::getDistance(void) {
  Wire.requestFrom(8, 1);
  while (Wire.available()) {
    data = Wire.read();
  }

  return (int)data;
}