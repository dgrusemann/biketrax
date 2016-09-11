const int hall = A0;
const int changeAvg = 0.05;
int hallAvg = 0;
int Hall_count = 0;
bool hallDetected = false;

long tHall = 0;
long tHallRead = 0;

int thresHallRead = 100; //ms
int thresHall = 5000; //ms

void Hall_init() {
  pinMode(hall, INPUT);
  //Hall_calcAvg();
  hallAvg = 530;
}

void Hall_loop() {
  if (millis() - tHallRead > thresHallRead) {
    Hall_read();
    tHallRead = millis();
  }
  if (millis() - tHall > thresHall) {
    tHall = millis();
    Hall_calcValues();
    Hall_saveHall();
  }
}

void Hall_calcValues() {
  Hall_distance = WHEELLEN * Hall_count;
}

void Hall_read() {
  int sum = 0;
  int avg = 10;
  for (int i = 0; i < avg; i++) {
    int value = analogRead(hall);
    sum += value;
  }
  sum = sum / avg;
  //Serial.print("Hall_sum: ");
  //Serial.println(sum);
  if (sum - hallAvg > hallAvg * changeAvg && !hallDetected) {
    hallDetected = true;
    Hall_count++;
  }
  else {
    hallDetected = false;
  }
}

//not great, because if hall sensor is close to magnet while averaging, counting wont work..
void Hall_calcAvg() {
  int t = millis();
  int sum = 0;
  int count = 0;
  while (millis() - t < 500) {
    sum += analogRead(hall);
    count++;
  }
  hallAvg = sum / count;
}

void Hall_saveHall() {
  f.print(millis());
  f.print("\t");
  f.print("hall");
  f.print("\t");
  f.print(Hall_distance);
  f.println("");
}

void Hall_display(){
  Serial.print("Hall Distance: ");
  Serial.print(Hall_distance);
  Serial.println(" meter");
}

