// usonic.ino

int _usonic::getDistance(void) {
  Wire.requestFrom(8, 1);

  timeOut = millis();
  while (timeOut + 5 >= millis()) {
    if (Wire.available()) {
      data = Wire.read();

      break;
    }
  }

  return (int)data;
}