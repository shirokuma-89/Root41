void _ball::read(int* b) {
  digitalWrite(BALL_RESET, HIGH);
  delay(5);

  for (int i = 0; i <= 15; i++) {
    *(b + i) = analogRead(BALL[i]);
  }

  digitalWrite(BALL_RESET, LOW);
}

void _ball::calc(void) {
  top = 0;
  for (int i = 0; i <= 15;i++){
    if(val[top] >= val[i]){
      top = i;
    }
  }

  Serial.println(top);
}
