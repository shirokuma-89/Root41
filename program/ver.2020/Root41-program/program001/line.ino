void _line::process(void) {
  flag = false;
}

_line::_line(void) {
  for (int i = 0; i <= 19; i++) {
    val[i] = false;
  }
}

void _line::read(void) {
  for (int i = 0; i <= 19; i++) {
    val[i] = !digitalRead(LINE[i]);
  }
}
