const int hall = A0;
int hall_value = 0;

void Hall_setup() {
  pinMode(hall, INPUT);
}

void Hall_read() {
  int sum = 0;
  for (int i = 0; i < 10; i++) {
    int value = analogRead(hall);
    sum += value;
  }
  hall_value = sum / 10;
}
