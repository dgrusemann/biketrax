#include <TinyGPS++.h>
#include <SoftwareSerial.h>
/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

long tGps = 0;
int thresGps = 500; //ms

void Gps_init()
{
  ss.begin(GPSBaud);
}

void Gps_loop()
{
  if (millis() - tGps > thresGps) {

    while (ss.available() > 0)
      if (gps.encode(ss.read()))
        displayInfo();

    tGps = millis();
  }
}

void displayInfo()
{
  Serial.print(F("Location: "));
  if (gps.location.isValid())
  {
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);

    f.print(millis());
    f.print("\t");
    f.print("gps");
    f.print("\t");
    f.print(gps.location.lat(), 6);
    f.print("\t");
    f.print(gps.location.lng(), 6);
    f.println("");
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
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
