#define BAUD_RATE 9600

void setup() {
    Serial.begin(BAUD_RATE);
}

void loop() {
//  if (Serial.available()) {
//    Serial.write(Serial.read());
//  }
  Serial.write(20);
  delay(2000);
  Serial.write(60);
  delay(2000);
}
