// usonic.ino

int _usonic::getDistance(void) {
  pauseTimer5();
  try {
    Wire.requestFrom(8, 1);

    timeOut = millis();
    while (timeOut + 5 >= millis()) {
      if (Wire.available()) {
        data = Wire.read();

        break;
      }
    }
    startTimer5(50);
  } catch {
    Wire.begin();
    TWBR = 12;
  }

  return (int)data;
}