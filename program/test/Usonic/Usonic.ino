// Usonic.ino

#define TRIG 46
#define ECHO 46
#define PIN 46

class _usonic {
 public:
  byte read(void);

  byte distance;
  unsigned long process;

 private:
  unsigned long timer;

  int ans;


} usonic;

void setup(void) {
  Serial.begin(115200);

  pinMode(TRIG, OUTPUT);
  // pinMode(ECHO, INPUT);
}

void loop(void) {
  usonic.process = millis();
  usonic.distance = usonic.read();

  Serial.print((String)usonic.distance);
  Serial.print("\t");
  Serial.println(millis()- usonic.process);
  usonic.process = millis();
}

byte _usonic::read(void) {
  //超音波に5usのパルスを出力
  pinMode(TRIG, OUTPUT);
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(5);
  digitalWrite(TRIG, LOW);

  pinMode(TRIG, INPUT);
  timer = pulseIn(TRIG, HIGH);  // パルス幅を計測

  if (timer < 18000) {
    //距離を計算
    ans = int(((float)timer / 29.0) / 2.0);
  } else {
    // error!
    ans = 0;
  }

  return (byte)ans;
}