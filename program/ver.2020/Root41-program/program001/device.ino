void _device::initialize(void) {
  LED.RED = RGBLED.Color(255, 0, 0);
  LED.BLUE = RGBLED.Color(0, 0, 255);
  LED.GREEN = RGBLED.Color(0, 255, 0);
  LED.YELLOW = RGBLED.Color(255, 255, 0);
  LED.WHITE = RGBLED.Color(255, 255, 255);

  Wire.begin();

  RGBLED.begin();
  RGBLED.show();

  RGBLED.begin();
  RGBLED.setBrightness(LED.bright);
  LED.changeAll(LED.GREEN);
  RGBLED.show();

  for (int i = 0; i <= 15; i++) {
    pinMode(BALL[i], INPUT);
  }
  pinMode(BALL_RESET, OUTPUT);
  pinMode(BALL_HOLD, INPUT);

  for (int i = 0; i <= 19; i++) {
    pinMode(LINE[i], INPUT);
  }

  pinMode(SW_1, INPUT_PULLUP);
  pinMode(SW_2, INPUT_PULLUP);
  pinMode(SW_RESET, INPUT_PULLUP);

  if (EEPROM[0] == 1) {
    device.robot = true;
    LED.defaltColor = RGBLED.Color(0, 150, 255);
    LED.subColor = RGBLED.Color(0, 0, 255);
  } else {
    device.robot = false;
    LED.defaltColor = RGBLED.Color(255, 100, 0);
    LED.subColor = RGBLED.Color(255, 0, 0);
  }

  gyro.eeprom[0] = (EEPROM[1] * 256) + EEPROM[2];
  gyro.eeprom[1] = (EEPROM[3] * 256) + EEPROM[4];
  gyro.eeprom[2] = (EEPROM[5] * 256) + EEPROM[6];
  gyro.eeprom[3] = (EEPROM[7] * 256) + EEPROM[8];
  gyro.eeprom[4] = (EEPROM[9] * 256) + EEPROM[10];
  gyro.eeprom[5] = (EEPROM[11] * 256) + EEPROM[12];
  
  lcd.begin();
  lcd.command(0x38);
  lcd.command(0x39);
  lcd.command(0x14);
  lcd.command(0x73);
  lcd.command(0x51);
  lcd.command(0x6c);
  lcd.command(0x38);
  lcd.command(0x01);
  lcd.command(0x0c);
}

void _device::check(void) {
  RGBLED.show();
  RGBLED.begin();
  RGBLED.clear();

  if (!digitalRead(SW_RESET)) {
    device.mode = 0;
  } else if (!digitalRead(SW_1)) {
    device.mode = 1;
  } else if (!digitalRead(SW_2)) {
    // asm volatile("  jmp 0");
    device.mode = 1;
    LED.bright = 0;
  }

  gyro.differentialDeg = gyro.differentialRead();
  gyro.pickUpAns = gyro.pickUp();
}
