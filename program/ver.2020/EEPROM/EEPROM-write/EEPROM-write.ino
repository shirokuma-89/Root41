#include <EEPROM.h>

void setup() {
  Serial.begin(115200);
  int val = 50;
  EEPROM[0] = val;      // address「0」の保存値は50に
  int num = EEPROM[0];  // 変数numの値は50に
  Serial.println(num);
}

void loop() {}
