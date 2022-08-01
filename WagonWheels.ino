#include "LedControl.h"

int delayTime = 0;
float leds = (float) LED_COUNT;
uint64_t last_read_time;

void setup() {
  Serial.begin(115200);
  initialize();
  last_read_time = micros();
}
 
void loop() {
    if (Serial.available() > 0) {
        last_read_time = micros();
        float rpm = Serial.parseFloat();
        float revolutionsPerSecond = rpm / 60.0;
        delayTime = 1000 * (1 / (revolutionsPerSecond * leds));
    }

    if ((micros() - last_read_time) / 1000000.0 >= 10.0) {
        delayTime = 0;
    }

    if (delayTime > 0) {
        delay(delayTime);
        advance(1);
    }
  }