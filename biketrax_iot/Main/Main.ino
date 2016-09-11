#define DEBUG
#define SAVE_DATA

//file read and write
#include "FS.h"

File f;

bool emptyDate = true;
bool emptyTime = true;

void App_init() {
  Serial.begin(9600);
  Serial.println("------------------------");
  Serial.println("bike.trax starting...");
  Serial.println("------------------------");
}

void setup()
{
  App_init();

  Acc_init();
  Gps_init();
  Hall_init();
  Cache_init(0);
}

void loop()
{
  Acc_loop();
  Gps_loop();
  Hall_loop();

  if (millis() > 60000) {
    Cache_sendToBackend();
    Cache_standby();
  }
}
