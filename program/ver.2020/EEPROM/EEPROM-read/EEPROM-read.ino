#include <EEPROM.h>

void setup() {
  Serial.begin(115200);

  Serial.print("I'm ");

  int val = EEPROM.read(0);

  if (val == 1) {
    Serial.println("Hogehoge.");
  } else if (val == 2) {
    Serial.println("Fugafuga.");
  }
}

void loop() {}
