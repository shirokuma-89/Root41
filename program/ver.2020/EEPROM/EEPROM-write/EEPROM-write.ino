#include <EEPROM.h>

void setup() {
  Serial.begin(115200);
}

void loop() {
  Serial.println("Please enter a number:");

  while (!Serial.available()) {
  }

  int val = Serial.read() - 48;

  EEPROM.write(0, val);
  
  Serial.print("Completed!:");
  Serial.println(val);
  Serial.println("");
}
