#include "RpmController.h"
#include "Logger.h"
#include "Constants.h"

RpmController* rpmController;

void setup() {
    Serial.begin(BAUD_RATE);
    Logger::println("Starting", 5000);
    rpmController = new RpmController();
}

void loop() {
    Logger::println("Not crashed", 1000);
    rpmController->UpdateWheels();
}