/*
   acc sensor integrated
   gps integrated

   open:
   - wifi GRUSI
   - write file

*/

#define DEBUG 1

//file read and write
#include "FS.h"

File f;

void Acc_display();
void Acc_save();

long tAcc = 0;
long tGps = 0;
long tHall = 0;
long tHallRead = 0;

int thresAcc = 500; //ms
int thresGps = 500; //ms
int thresHallRead = 100; //ms
int thresHall = 5000; //ms

bool emptyDate = true;
bool emptyTime = true;

void App_init() {
#ifndef ESP8266
  while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
#endif
  Serial.begin(9600);
  Serial.println("------------------------");
  Serial.println("bike.trax starting...");
  Serial.println("------------------------");
}

void setup()
{
  errorLed(1);
  
  App_init();

  Gps_init();
  Cache_init();
  Acc_init();
  Hall_setup();
  
  errorLed(0);
}

void loop()
{
  if (millis() - tAcc > thresAcc) {
    Acc_save();

#ifdef DEBUG
    Acc_display();
#endif

    tAcc = millis();
  }

  if (millis() - tGps > thresGps) {
    Gps_save();
    Gps_display();

    tGps = millis();
    errorLed(1);
    errorLed(0);
  }

  if (millis() - tHallRead > thresHallRead) {
    Hall_read();
    tHallRead = millis();
  }
  if (millis() - tHall > thresHall) {
    tHall = millis();
    Hall_calcValues();
    Hall_saveHall();
#ifdef DEBUG
    Hall_display();
#endif
  }

  if (millis() > 10000) {
    //Cache_sendToBackend();
    //Cache_standby();
  }
}

void errorLed(bool i) {
  if (i) {
    analogWrite(15, HIGH);
  } else {
    analogWrite(15, LOW);
  }
}
