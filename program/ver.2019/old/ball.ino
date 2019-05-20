// TANUKICHI 5,13  1,3 * 1.1
// KURU 2,4

//センサー値取得
void _ball::read(int* b) {
  // delay(10);
  *b = analogRead(BALL0);
  *(b + 2) = analogRead(BALL2);
  *(b + 1) = (*(b + 0) + *(b + 2)) / 2;
  *(b + 3) = analogRead(BALL3);
  *(b + 3) = analogRead(BALL3);
  *(b + 4) = analogRead(BALL4);
  *(b + 5) = analogRead(BALL5);
  *(b + 6) = analogRead(BALL6);
  *(b + 7) = analogRead(BALL7) * 0.88;
  *(b + 5) = (*(b + 4) + *(b + 6)) / 2;
  *(b + 8) = analogRead(BALL8) * 0.8;   //*0.75
  *(b + 9) = analogRead(BALL9) * 0.88;    //*0.9
  *(b + 10) = analogRead(BALL10);  //*0.9
  *(b + 11) = analogRead(BALL11);
  *(b + 12) = analogRead(BALL12);
  *(b + 13) = analogRead(BALL13);
  *(b + 14) = analogRead(BALL14);
  // *(b + 13) = (*(b + 12) + *(b + 14)) / 2;
  *(b + 15) = analogRead(BALL15);
  // digitalWrite(BALL_RESET, LOW);
}