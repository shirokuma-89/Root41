// gyro.ino

MPU6050 mpu;
static uint8_t mpuIntStatus;
static bool dmpReady = false;
static uint16_t packetSize;
int16_t Gyro_Now = 0, Gyro = 0, Gyro_Offset = 0;
uint16_t fifoCount;
uint8_t fifoBuffer[64];
Quaternion q;
VectorFloat gravity;
float ypr[3];

uint8_t devStatus;

VectorInt16 aa;
VectorInt16 aaReal;
VectorInt16 dmpgyro;
float euler[3];

volatile bool mpuInterrupt = false;

//割り込み
void dmpDataReady() {
  mpuInterrupt = true;
}

//初期化
void _gyro::setting(void) {
RESTART:
  mpu.initialize();
  if (mpu.testConnection() != true) {
    goto RESTART;  //接続失敗
  }
  devStatus = mpu.dmpInitialize();
  if (devStatus != 0) {
    goto RESTART;  //初期化失敗
  }

  mpu.setXGyroOffset(gyro.eeprom[0]);
  mpu.setYGyroOffset(gyro.eeprom[1]);
  mpu.setZGyroOffset(gyro.eeprom[2]);
  mpu.setXAccelOffset(gyro.eeprom[3]);
  mpu.setYAccelOffset(gyro.eeprom[4]);
  mpu.setZAccelOffset(gyro.eeprom[5]);
  mpu.setDMPEnabled(true);

  attachInterrupt(0, dmpDataReady, RISING);
  mpuIntStatus = mpu.getIntStatus();

  dmpReady = true;

  packetSize = mpu.dmpGetFIFOPacketSize();
}

//角度取得
int _gyro::read(void) {
  mpuIntStatus = false;
  mpuIntStatus = mpu.getIntStatus();
  fifoCount = mpu.getFIFOCount();
  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();
  } else if (mpuIntStatus & 0x02) {
    while (fifoCount < packetSize) {
      fifoCount = mpu.getFIFOCount();
    }
    mpu.getFIFOBytes(fifoBuffer, packetSize);
    fifoCount -= packetSize;
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    Gyro_Now = degrees(ypr[0]);  // + 180;
    Gyro = Gyro_Now - offset;
    while (Gyro <= 0) {
      Gyro += 360;
    }
    Gyro %= 360;
  }
  return (360 - Gyro) % 360;
}

//角速度取得
int _gyro::differentialRead(void) {
  if (!dmpReady)
    return;

  while (!mpuInterrupt && fifoCount < packetSize) {
    // 処理するデータを待ちながら、他の処理をする
  }

  mpuInterrupt = false;
  mpuIntStatus = mpu.getIntStatus();

  fifoCount = mpu.getFIFOCount();

  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    mpu.resetFIFO();
    return;  // 処理終了
  }

  while (fifoCount < packetSize) {
    fifoCount = mpu.getFIFOCount();
  }

  mpu.getFIFOBytes(fifoBuffer, packetSize);
  fifoCount -= packetSize;

  mpu.dmpGetGyro(&dmpgyro, fifoBuffer);

  return dmpgyro.z;
}

bool _gyro::pickUp(void) {
  bool foo;

  if (true) {
    foo = false;
    mpuIntStatus = false;
    mpuIntStatus = mpu.getIntStatus();
    fifoCount = mpu.getFIFOCount();
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
      mpu.resetFIFO();
    } else if (mpuIntStatus & 0x02) {
      while (fifoCount < packetSize) {
        fifoCount = mpu.getFIFOCount();
      }
      mpu.getFIFOBytes(fifoBuffer, packetSize);
      fifoCount -= packetSize;
      mpu.dmpGetQuaternion(&q, fifoBuffer);
      mpu.dmpGetGravity(&gravity, &q);
      mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
      if (abs(degrees(ypr[1])) >= 20 || abs(degrees(ypr[2])) >= 20){
        foo = true;
      }
    }
  } else {
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetAccel(&aa, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);

    if (aa.z >= 9500) {
      foo = true;
    } else {
      foo = false;
    }
  }
  return foo;
}
