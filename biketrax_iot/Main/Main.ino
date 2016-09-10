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

int thresAcc = 500; //ms
int thresGps = 500; //ms

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
  App_init();

  Gps_init();
  //  Cache_init();
  Acc_init();
  //  Hall_setup();
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
    delay(100);
    errorLed(0);
  }

  //  Hall_read();

  if (millis() > 10000) {
    //    Cache_sendToBackend();
    //Cache_clear();
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
