// usonic.ino

int _usonic::getDistance(void) {
  pauseTimer5();
  Wire.requestFrom(8, 1);

  timeOut = millis();
  while (timeOut + 5 >= millis()) {
    if (Wire.available()) {
      data = Wire.read();

      break;
    }
  }
  startTimer5(50);

  return (int)data;
}