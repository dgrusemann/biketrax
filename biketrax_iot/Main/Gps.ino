//gps imports
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

//gps def:
static const int RXPin = 12, TXPin = 13;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void Gps_init() {
  ss.begin(GPSBaud);
}

void saveGps() {
  if (ss.available() > 0) {
    if (gps.encode(ss.read())) {
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
  }
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
#endif
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
