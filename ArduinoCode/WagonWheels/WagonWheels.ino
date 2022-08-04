#include "RpmController.h"

#define BAUD_RATE 115200

RpmController* rpmController;

void setup() {
    Serial.begin(BAUD_RATE);
    // delay(5000);
    // Serial.println("Starting");
    rpmController = new RpmController();
}
 
void loop() {
    //Serial.println("Not Crashed");
    rpmController->UpdateWheels();
    //delay(1000);
}