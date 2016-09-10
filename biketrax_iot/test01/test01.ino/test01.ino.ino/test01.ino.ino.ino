float var_float = 23.232;
int var_int = 1232;
String var_string = "text here";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Serial started!");
  Serial.println("new line");
  Serial.print("acc");
  Serial.print("\t");
  Serial.print(var_float);
  Serial.print("\t");
  Serial.print(var_int);
  Serial.println("");
  
  Serial.print("gps");
  Serial.print("\t");
  Serial.print(var_float);
  Serial.print("\t");
  Serial.print(var_int);
  Serial.println("");
}

void loop() {
  // put your main code here, to run repeatedly:

}
