//acc-gyro imports
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS0.h>

void Acc_init() {
  //acc-gyro init:
  Serial.println("acc-gyro init");
  if (!lsm.begin())
  {
    /* There was a problem detecting the LSM9DS0 ... check your connections */
    Serial.print(F("Ooops, no LSM9DS0 detected ... Check your wiring or I2C ADDR!"));
    digitalWrite(8, HIGH);
    while (1);
  }
  configureSensor();
  Serial.println(F("LSM9DS0 init successfull"));
}

void saveAcc() {
  //acc-gyro def:
  sensors_event_t accel, mag, gyro, temp;
  lsm.getEvent(&accel, &mag, &gyro, &temp);

  f.print(millis());
  f.print("\t");
  f.print("acc");
  f.print("\t");
  f.print(accel.acceleration.x);
  f.print("\t");
  f.print(accel.acceleration.y);
  f.print("\t");
  f.print(accel.acceleration.z);
  f.println("");

  f.print(millis());
  f.print("\t");
  f.print("gyro");
  f.print("\t");
  f.print(gyro.gyro.x);
  f.print("\t");
  f.print(gyro.gyro.y);
  f.print("\t");
  f.print(gyro.gyro.z);
  f.println("");
}

void displayAcc() {

  //acc-gyro def:
  sensors_event_t accel, mag, gyro, temp;
  lsm.getEvent(&accel, &mag, &gyro, &temp);

  // print out accelleration data
  Serial.print("Accel X: "); Serial.print(accel.acceleration.x); Serial.print(" ");
  Serial.print("  \tY: "); Serial.print(accel.acceleration.y);       Serial.print(" ");
  Serial.print("  \tZ: "); Serial.print(accel.acceleration.z);     Serial.println("  \tm/s^2");

  // print out magnetometer data
  Serial.print("Magn. X: "); Serial.print(mag.magnetic.x); Serial.print(" ");
  Serial.print("  \tY: "); Serial.print(mag.magnetic.y);       Serial.print(" ");
  Serial.print("  \tZ: "); Serial.print(mag.magnetic.z);     Serial.println("  \tgauss");

  // print out gyroscopic data
  Serial.print("Gyro  X: "); Serial.print(gyro.gyro.x); Serial.print(" ");
  Serial.print("  \tY: "); Serial.print(gyro.gyro.y);       Serial.print(" ");
  Serial.print("  \tZ: "); Serial.print(gyro.gyro.z);     Serial.println("  \tdps");

  // print out temperature data
  Serial.print("Temp: "); Serial.print(temp.temperature); Serial.println(" *C");

  Serial.println("**********************\n");
}

void configureSensor()
{
  // 1.) Set the accelerometer range
  lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_2G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_4G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_6G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_8G);
  //lsm.setupAccel(lsm.LSM9DS0_ACCELRANGE_16G);

  // 2.) Set the magnetometer sensitivity
  lsm.setupMag(lsm.LSM9DS0_MAGGAIN_2GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_4GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_8GAUSS);
  //lsm.setupMag(lsm.LSM9DS0_MAGGAIN_12GAUSS);

  // 3.) Setup the gyroscope
  lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_245DPS);
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_500DPS);
  //lsm.setupGyro(lsm.LSM9DS0_GYROSCALE_2000DPS);
}
