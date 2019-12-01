int _tof::read(void) {
  int hogehoge = TOF.readRangeContinuousMillimeters();
  return hogehoge > 1 ? hogehoge : dist;
}

void _tof::keeeper(void) {
  x = 0;
  y = 0;
  ball.degx = 0.2;
  if (dist >= 140) {
    deg = 180;
  } else if (dist <= 100) {
    deg = 0;
  } else {
    deg = 1000;
  }
  if (ball.degx >= 0.3) {
    deg = 90;
  } else if (ball.degx <= -0.3) {
    deg = 270;
  } else if (ball.degx < 0.1 && ball.degx > 0.1) {
    deg = 0;
  }
  if (dist >= 160 || dist <= 80) {
    speed = 100;
  } else if (dist >= 140) {
    speed = map(dist, 140, 160, 20, 60);
  } else if (dist <= 100) {
    speed = map(dist, 100, 80, 20, 60);
  }
}
