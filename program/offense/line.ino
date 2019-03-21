// line.ino

_line::_line(void) {
  for (int i = 0; i <= 3; i++) {
    val[i] = false;
  }
}

void _line::read(void) {
  touch = false;

  val[0] = !digitalRead(LINE1);
  val[1] = !digitalRead(LINE2);
  val[2] = !digitalRead(LINE3);
  val[3] = !digitalRead(LINE4);

  //どれか一つでも反応しているならば
  if (val[0] | val[1] | val[2] | val[3]) {
    touch = true;
    if (val[0]) {
      if (first == 5 | first == 0) {
        first = 0;
      } else if (second == 5 | second == 0) {
        second = 0;
      }
    }
    if (val[1]) {
      if (first == 5 | first == 1) {
        first = 1;
      } else if (second == 5 | second == 1) {
        second = 1;
      }
    }
    if (val[2]) {
      if (first == 5 | first == 2) {
        first = 2;
      } else if (second == 5 | second == 2) {
        second = 2;
      }
    }
    if (val[3]) {
      if (first == 5 | first == 3) {
        first = 3;
      } else if (second == 5 | second == 3) {
        second = 3;
      }
    }
  }
}