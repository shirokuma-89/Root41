void _camera::available(void) {
  Wire.beginTransmission(0x54);
  check = Wire.endTransmission();
}

void _camera::read(void) {
  Wire.beginTransmission(0x54);
  check = Wire.endTransmission();

  if (check == 0) {
    for (int i = 1; i <= 7; i++) {
      exist[i] = false;
      x[i] = 0;
      y[i] = 0;
    }

    pixy.ccc.getBlocks();

    for (int i = 0; i <= pixy.ccc.numBlocks - 1; i++) {
      x[pixy.ccc.blocks[i].m_signature] = pixy.ccc.blocks[i].m_x - 158;
      y[pixy.ccc.blocks[i].m_signature] =
          pixy.ccc.blocks[pixy.ccc.blocks[i].m_signature].m_y - 108;
      exist[pixy.ccc.blocks[i].m_signature] = true;
    }
  }
}