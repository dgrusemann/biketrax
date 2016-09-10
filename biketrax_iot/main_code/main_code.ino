/*
 * acc sensor integrated
 * 
 * open:
 * - gps
 * - wifi
 * - write file
 * 
 */

//acc-gyro imports
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS0.h>

//gps imports
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

#define DEBUG
//acc-gyro functions:
void displaySensordDetails();
void configureSensor();
void displayAcc();
void saveAcc();

//gps functions:
void displayGps();
void saveGps();

long t = 0;
long tAcc = 0;
long tGps = 0;

int thresAcc = 500; //ms
int thresGps = 500; //ms

//acc-gyro def:
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(1000);  // Use I2C, ID #1000

//gps def:
static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;
// The TinyGPS++ object
TinyGPSPlus gps;
// The serial connection to the GPS device
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
  displaySensorDetails();
#endif
}

void loop() 
{  
  t = millis();
  
//acc-gyro def:
  sensors_event_t accel, mag, gyro;
  lsm.getEvent(&accel, &mag, &gyro); 
  
#ifndef DEBUG
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
          analogWrite(13,HIGH);
          delay(100);
          analogWrite(13,LOW);
      }
    }
  }
 
#else
  displayGps();
  displayAcc();
#endif
}

void saveAcc(){
  Serial.print(accel.acceleration.x);
  Serial.print(accel.acceleration.y);
  Serial.print(accel.acceleration.z);

  // print out magnetometer data
  Serial.print(mag.magnetic.x);
  Serial.print(mag.magnetic.y);
  Serial.print(mag.magnetic.z);
  
  // print out gyroscopic data
  Serial.print(gyro.gyro.x);
  Serial.print(gyro.gyro.y);
  Serial.print(gyro.gyro.z);
}

void saveGps(){
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(gps.location.lng(), 6);
  } else {
    //save default value -1000
  }

  if (gps.date.isValid())
  {
    Serial.print(gps.date.month());
    Serial.print(gps.date.day());
    Serial.print(gps.date.year());
  }
  else
  {
    //save default values
  }

  if (gps.time.isValid())
  {
    Serial.print(gps.time.hour());
    Serial.print(gps.time.minute());
    Serial.print(gps.time.second());
    Serial.print(gps.time.centisecond());
  }
  else
  {
    //save default value
  }

  if (gps.altitude.isValid())
  {
    Serial.print(gps.altitude.meters());
  }
  else
  {
    //save default value -1000
  }

  if (gps.speed.isValid())
  {
    Serial.print(gps.speed.kmph());
  }
  else
  {
    //save default value
  }
}

void displayAcc() {
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

void displayGps()
{
#ifdef DEBUG
  sensor_t accel, mag, gyro;  
  lsm.getSensor(&accel, &mag, &gyro);

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
