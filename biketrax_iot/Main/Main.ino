/*
   acc sensor integrated
   gps integrated

   open:
   - wifi GRUSI
   - write file

*/

#include <Adafruit_Sensor.h>
#include <Adafruit_LSM9DS0.h>
//file read and write
#include "FS.h"

File f;

//acc-gyro def:
Adafruit_LSM9DS0 lsm = Adafruit_LSM9DS0(1000);  // Use I2C, ID #1000

//acc-gyro functions:
void configureSensor();
void displayAcc();
void saveAcc();

#define DEBUG

//general functions:
void initDrive();
void sendToBackend();
void standby();

//gps functions:
void displayGpsSensordDetails();
void displayGps();
void saveGps();

long t = 0;
long tAcc = 0;
long tGps = 0;
long tHall = 0;

int thresAcc = 500; //ms
int thresGps = 500; //ms
int thresHallRead = 100 //ms
int thresHall = 5000; /ms

bool emptyDate = true;
bool emptyTime = true;

void setup()
{
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  Serial.begin(9600);
  Serial.println("------------------------");
  Serial.println("bike.trax SW starting..........");
  Serial.println("------------------------");

  Cache_initDrive();
  Acc_init();
  Hall_setup();

#ifdef DEBUG
  //acc-gyro info
  displayGpsSensorDetails();
#endif
}

void loop()
{
  t = millis();

  //#ifndef DEBUG
  if (millis() - tAcc > thresAcc) {
    //TODO: save data to file here
    saveAcc();
    displayAcc();
    tAcc = millis();
  }
  if (millis() - tGps > thresGps) {
    saveGps();
    displayGps();
    tGps = millis();
    errorLed(1);
    errorLed(0);
  }

  if(millis() - tHallRead > thresHallRead) {
      Hall_read();
      tHallRead = millis();
  }
  if(millis() - tHall > thresHall){
    tHall = millis();
    Hall_calcValues();
    Hall_saveHall();
  }

  if (millis() > 10000) {
    Cache_sendToBackend();
    Cache_standby();
  }

  //#else
  
  
  //#endif
}

void errorLed(bool i) {
  if (i) {
    analogWrite(15, HIGH);
  } else {
    analogWrite(15, LOW);
  }
}
