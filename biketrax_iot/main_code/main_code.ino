/*
 * acc sensor integrated
 * gps integrated
 * 
 * open:
 * - wifi GRUSI
 * - write file
 * 
 */

//file read and write
#include "FS.h"

//acc-gyro imports
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS0.h>

//gps imports
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define DEBUG

//general functions:
void initDrive();
void sendToBackend();
void standby();

//acc-gyro functions:
void configureSensor();
void displayAcc();
void saveAcc();

//gps functions:
void displayGpsSensordDetails();
void displayGps();
void saveGps();

File f;
long t = 0;
long tAcc = 0;
long tGps = 0;

int thresAcc = 500; //ms
int thresGps = 500; //ms

bool emptyDate = true;
bool emptyTime = true;

//acc-gyro def:
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(1000);  // Use I2C, ID #1000

//gps def:
static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);


void setup() 
{
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  Serial.begin(9600);
  Serial.println("------------------------");
  Serial.println("bike.trax SW starting..........");
  Serial.println("------------------------");

  initDrive();
  
  //acc-gyro init:
  Serial.println("acc-gyro init");
  if(!lsm.begin())
  {
    /* There was a problem detecting the LSM9DS0 ... check your connections */
    Serial.print(F("Ooops, no LSM9DS0 detected ... Check your wiring or I2C ADDR!"));
    digitalWrite(8,HIGH);
    while(1);
  }
  configureSensor();
  Serial.println(F("LSM9DS0 init successfull"));

  //gps init:
  ss.begin(GPSBaud);
  
#ifdef DEBUG
  //acc-gyro info
  displayGpsSensorDetails();
#endif
}

void loop() 
{  
  t = millis();
  
//#ifndef DEBUG
  if (millis()-tAcc > thresAcc){
    //TODO: save data to file here
    saveAcc();
    tAcc = millis();
  }
  if (millis()-tGps > thresGps){
    if (ss.available()>0){
      if (gps.encode(ss.read())){
          saveGps();
          tGps = millis();
          errorLed(1);
          delay(100);
          errorLed(0);
      }
    }
  }

 if (millis()>10000){
    sendToBackend();
    standby();
 }
  
//#else
  displayGps();
  displayAcc();
//#endif
}

void initDrive(){
  // File mounting
  bool result = SPIFFS.begin();
  Serial.println("SPIFFS opened: " + result);

  // this opens the file "db.txt" in read-mode
  f = SPIFFS.open("/db.txt", "a+");
  
  if (!f) {
    Serial.println("File doesn't exist yet. Creating it");
    // open the file in append mode
    f = SPIFFS.open("/db.txt", "a+");
    if (!f) {
      Serial.println("file creation failed");
      for (int i=0; i<10;i++){
      errorLed(1);
      delay(100);
      errorLed(0);
      delay(100);
      }
      while(1);
    }
  } else {
      f = SPIFFS.open("/db.txt", "a+");
  }
}

void sendToBackend(){
  while(f.available()) {
    //Lets read line by line from the file
    String line = f.readStringUntil('\n');
    //TODO send line to backend here  
    Serial.println(line);
  }
}

void errorLed(bool i){
  if (i){
      analogWrite(15,HIGH);
  } else {
      analogWrite(15,LOW);
  }
}

void standby(){
    f.close();
    Serial.println("file closed, shutdown");
    while(1){
          errorLed(1);
          delay(1000);
          errorLed(0);
          delay(1000);
    }    
}

void saveAcc(){
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

void saveGps(){
  if (gps.location.isValid())
  {  
    f.print(millis());
    f.print("\t");
    f.print("gps");
    f.print("\t");
    f.print(gps.location.lat(), 6);
    f.print("\t");
    f.print(gps.location.lng(), 6);
    f.println("");
  } else {
    f.print(millis());
    f.print("\t");
    f.print("gps");
    f.print("\t");
    f.print(-1000);
    f.print("\t");
    f.print(-1000);
    f.println("");
  }

  if (gps.date.isValid() && emptyDate)
  {
    f.print(millis());
    f.print("\t");
    f.print("date");
    f.print("\t");
    f.print(gps.date.year());
    f.print("\t");
    f.print(gps.date.month());
    f.print("\t");
    f.print(gps.date.day());
    f.println("");
    emptyDate = false;
  }

  if (gps.time.isValid() && emptyTime)
  {
    f.print(millis());
    f.print("\t");
    f.print("time");
    f.print("\t");
    f.print(gps.time.hour());
    f.print("\t");
    f.print(gps.time.minute());
    f.print("\t");
    f.print(gps.time.second());
    f.println("");
    emptyTime = false;
  }

  if (gps.altitude.isValid())
  {
    f.print(millis());
    f.print("\t");
    f.print("alt");
    f.print("\t");
    f.print(gps.altitude.meters());
    f.println("");
  }
  else
  {
    f.print(millis());
    f.print("\t");
    f.print("alt");
    f.print("\t");
    f.print(-1000);
    f.println("");
  }

  if (gps.speed.isValid())
  {
    f.print(millis());
    f.print("\t");
    f.print("speed");
    f.print("\t");
    f.print(gps.speed.kmph());
    f.println("");
  }
  else
  {
    f.print(millis());
    f.print("\t");
    f.print("speed");
    f.print("\t");
    f.print(-1000);
    f.println("");
  }
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

  delay(250);
}

void displayGpsSensorDetails()
{
#ifdef DEBUG
  sensor_t accel, mag, gyro, temp;  
  lsm.getSensor(&accel, &mag, &gyro, &temp);

  Serial.println(F("------------------------------------"));
  Serial.print  (F("Sensor:       ")); Serial.println(accel.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(accel.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(accel.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(accel.max_value); Serial.println(F(" m/s^2"));
  Serial.print  (F("Min Value:    ")); Serial.print(accel.min_value); Serial.println(F(" m/s^2"));
  Serial.print  (F("Resolution:   ")); Serial.print(accel.resolution); Serial.println(F(" m/s^2"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));

  Serial.println(F("------------------------------------"));
  Serial.print  (F("Sensor:       ")); Serial.println(mag.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(mag.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(mag.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(mag.max_value); Serial.println(F(" uT"));
  Serial.print  (F("Min Value:    ")); Serial.print(mag.min_value); Serial.println(F(" uT"));
  Serial.print  (F("Resolution:   ")); Serial.print(mag.resolution); Serial.println(F(" uT"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));

  Serial.println(F("------------------------------------"));
  Serial.print  (F("Sensor:       ")); Serial.println(gyro.name);
  Serial.print  (F("Driver Ver:   ")); Serial.println(gyro.version);
  Serial.print  (F("Unique ID:    ")); Serial.println(gyro.sensor_id);
  Serial.print  (F("Max Value:    ")); Serial.print(gyro.max_value); Serial.println(F(" rad/s"));
  Serial.print  (F("Min Value:    ")); Serial.print(gyro.min_value); Serial.println(F(" rad/s"));
  Serial.print  (F("Resolution:   ")); Serial.print(gyro.resolution); Serial.println(F(" rad/s"));  
  Serial.println(F("------------------------------------"));
  Serial.println(F(""));
  
  delay(500);
#endif
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

void displayGps()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(F("/"));
    Serial.print(gps.date.day());
    Serial.print(F("/"));
    Serial.print(gps.date.year());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.altitude.isValid())
  {
    Serial.print(gps.altitude.meters());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

    Serial.print(F(" "));
  if (gps.speed.isValid())
  {
    Serial.print(gps.speed.kmph());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
