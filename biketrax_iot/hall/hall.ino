const int hall = A0;
int hall_value= 0;

void setup() {
  pinMode(hall, INPUT);
  Serial.begin(9600);
}

void loop() {
  hall_read();
  Serial.print("hall = ");
  Serial.print(hall_value);
  Serial.print("\n");
  delay(100);
}

void hall_read(){
  int sum = 0;
  for(int i = 0; i < 10; i++){
      int value = analogRead(hall);
      sum += value;
  }
  hall_value = sum / 10;
}

