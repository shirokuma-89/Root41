// Usonic.ino

#define TRIG 10
#define ECHO 11

class _usonic {
 public:
  byte read(void);

  byte distance;

 private:
  unsigned long timer;

  int ans;

} usonic;

void setup(void) {
  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
}

void loop(void) {
  usonic.distance = usonic.read();

  Serial.println((String)usonic.distance + " cm");
}

byte _usonic::read(void) {
  //超音波に5usのパルスを出力
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG, LOW);

  timer = pulseIn(ECHO, HIGH);  // パルス幅を計測

  if (timer < 18000) {
    //距離を計算
    ans = int(((float)timer / 29.0) / 2.0);
  } else {
    // error!
    ans = 0;
  }

  return (byte)ans;
}