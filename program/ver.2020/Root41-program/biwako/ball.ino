void _ball::read(int *b){
  digitalWrite(BALL_RESET, HIGH);
  delay(5);

  for (int i = 0; i <= 15;i++){
    *(b + i) = analogRead(BALL[i]);
  }

  digitalWrite(BALL_RESET, HIGH);
}
