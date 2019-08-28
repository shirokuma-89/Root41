void _ball::read(int* b) {
  digitalWrite(BALL_RESET, HIGH);
  for (int i = 0; i <= 15; i++) {
    *(b + i) = analogRead(BALL[i]);
  }

  if (device.robot) {
    val[6] = (val[5] * 2 + val[8]) / 3;
    val[7] = (val[8] * 2 + val[5]) / 3;
    val[14] = (val[13] + val[15]) / 2;
  } else {
    val[12] = (val[11] * 2 + val[14]) / 3;
    val[13] = (val[11] + val[14] * 2) / 3;
    val[8] = (val[7] + val[9]) / 2;
  }

  for (int i = 0; i <= 15; i++) {
    Serial.print(val[i]);
    Serial.print("\t");
  }
  Serial.println("");

  digitalWrite(BALL_RESET, LOW);
}

void _ball::calc(void) {
  top = 0;
  for (int i = 0; i <= 15; i++) {
    if (val[i] <= val[top]) {
      top = i;
    }
  }

  deg = top * 22.5;
  if (top > 1 || top < 15) {
    if (deg >= 180) {
      deg -= 360;
    }
    deg = round((float)deg * (float)sqrt(abs(deg)) * (float)0.115);
    deg += 720;
    deg %= 360;
    
    // if(top >= 6 && top <= 10 && val[top] <= 300){
    //   if (deg >= 180){
    //     deg += -90;
    //   } else {
    //     deg += 90;
    //   }
    // }
  }

  LCD.data = deg;
  LCD.unit = "DEG";

  exist = true;
  if (val[top] <= 640) {
    exCount = 0;
  } else {
    exCount++;
  }

  if (exCount >= 3) {
    exist = false;
  }
}
