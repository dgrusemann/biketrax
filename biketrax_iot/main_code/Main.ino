

void setup() {
  Serial.begin(115200);
  delay(10);
  Network_connectToWifi();
}

void loop() {
  Network_sendData("57", "57");
}

