void Cache_initDrive() {
  // File mounting
  bool result = SPIFFS.begin();
  Serial.println("SPIFFS opened: " + result);

  // this opens the file "db.txt" in read-mode
  f = SPIFFS.open("/db.txt", "r");

  if (!f) {
    Serial.println("File doesn't exist yet. Creating it");
    // open the file in append mode
    f = SPIFFS.open("/db.txt", "a+");
    if (!f) {
      Serial.println("file creation failed");
      for (int i = 0; i < 10; i++) {
        errorLed(1);
        delay(100);
        errorLed(0);
        delay(100);
      }
      while (1);
    }
  } else {
    f = SPIFFS.open("/db.txt", "a+");
  }
}

void Cache_sendToBackend() {
  f.seek(0, fs::SeekSet);
  while (f.available()) {
    //Lets read line by line from the file
    String line = f.readStringUntil('\n');
    //TODO send line to backend here
    Serial.println(line);
  }
}

void Cache_standby() {
  f.close();
  Serial.println("file closed, shutdown");
  while (1) {
    errorLed(1);
    delay(1000);
    errorLed(0);
    delay(1000);
  }
}
