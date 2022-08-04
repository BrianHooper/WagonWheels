#include "RpmController.h"
#include <Arduino.h>

RpmController::RpmController() {
    Serial.println("RpmController");
    this->FrontLeft = new WheelController(2);
    this->FrontRight = new WheelController(3);
    this->BackLeft = new WheelController(4);
    this->BackRight = new WheelController(5);

    leds = (float) 48;
}

void RpmController::UpdateWheels() {
    uint64_t loopStartTime = micros();
    CheckForTimeout(loopStartTime);
    UpdateRPM();
    AdvanceWheels();
    DelayWithOffset(loopStartTime);
}

void RpmController::AdvanceWheels() {
    if (rpm == 0) {
        return;
    }

    FrontLeft->Advance();
    FrontRight->Advance();
    BackLeft->Advance();
    BackRight->Advance();
}

void RpmController::DelayWithOffset(uint64_t loopStartTime) {
    if (delayTime == 0 || rpm == 0) {
      delay(DEFAULT_DELAY_TIME_MS);
      return;
    }
  
    uint64_t loopEndTime = micros();

    // Check for edge case, micros 64-bit unsigned int rolled over to zero between start and end
    if (loopEndTime < loopStartTime) {
        Serial.println("edge case");
        delay(delayTime);
        return;
    }

    int timeElapsedMs = (loopEndTime - loopStartTime) / 1000;
    int updatedDelayTime = delayTime - timeElapsedMs;
    if (updatedDelayTime > 0) {
        delay(updatedDelayTime);
    }
}

void RpmController::UpdateRPM() {
    if (Serial.available() > 0) {
        last_read_time = micros();

        // #TODO This may need to be a float value in order to be precise enough
        // and may need to switch to delayMicroseconds
        rpm = (int) Serial.read();
        CalculateDelayTime();
    }
}

void RpmController::CalculateDelayTime() {
    if (rpm == 0) {
        return;
    }

    float revolutionsPerSecond = rpm / 60.0;
    delayTime = 1000 * (1 / (revolutionsPerSecond * leds));
}

void RpmController::CheckForTimeout(uint64_t loopStartTime) {
    if (TIMEOUT_SECONDS == 0 || loopStartTime < last_read_time) {
        return;
    }

    int secondsSinceLastUpdate = (loopStartTime - last_read_time) / 1000000;
    if (secondsSinceLastUpdate >= TIMEOUT_SECONDS) {
        rpm = 0;
    }
}