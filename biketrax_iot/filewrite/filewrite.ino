#include "FS.h"

void setup() {
  Serial.begin(9600);

  // always use this to "mount" the filesystem
  bool result = SPIFFS.begin();
  Serial.println("SPIFFS opened: " + result);

  // this opens the file "f.txt" in read-mode
  File f = SPIFFS.open("/f.txt", "r");
  
  if (!f) {
    Serial.println("File doesn't exist yet. Creating it");

    // open the file in write mode
    File f = SPIFFS.open("/f.txt", "a+");
    if (!f) {
      Serial.println("file creation failed");
    }
    // now write two lines in key/value style with  end-of-line characters
    f.println("ssid=abc");
    f.println("password=123455secret");
    Serial.println("lines created");
  } else {
    // we could open the file
    File f = SPIFFS.open("/f.txt", "a+");
    f.print(millis());
    f.println("ms: newline");
    delay(1000);
    f.print(millis());
    f.println("ms: nextline");
    Serial.println("lines created with existing file");

    while(f.available()) {
      //Lets read line by line from the file
      String line = f.readStringUntil('\n');
      Serial.println(line);
    }

  }
  f.close();
}

void loop() {
  // put your main code here, to run repeatedly:

}
