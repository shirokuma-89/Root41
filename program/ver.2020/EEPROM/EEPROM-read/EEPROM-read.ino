#include <EEPROM.h>

void setup() {
  Serial.begin(115200);

  Serial.print("I'm ");

  int val = EEPROM[0];

  Serial.println(val);
}

void loop() {}
