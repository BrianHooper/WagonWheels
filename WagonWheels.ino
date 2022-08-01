//#include "LedControl.h"

float rpm = 0.0;
float leds = ((float)48);
float ratio = 7.0 / 30.0;
 
void setup() {
  Serial.begin(115200);      // Initialize Serial Port
}
 
void loop() {
    if (Serial.available() > 0) {
        rpm = Serial.parseFloat();
    }

    if (rpm > 0.0) {
        float revolutionsPerSecond = rpm / 60.0;
        float delayTime = (revolutionsPerSecond) / leds;
        delay(delayTime);
        //advance(1);
    }
  }