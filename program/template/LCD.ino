//LCD.ino

void _LCD::init(void) {
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
  lcd.clear();
  lcd.home();
}