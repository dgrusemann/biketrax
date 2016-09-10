void errorLed(bool i) {
  if (i) {
    analogWrite(15, HIGH);
  } else {
    analogWrite(15, LOW);
  }
}

void StatusLED_standby() {
  while (1) {
    errorLed(1);
    delay(1000);
    errorLed(0);
    delay(1000);
  }
}

