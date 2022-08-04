#include "RpmController.h"
#include <Arduino.h>

RpmController::RpmController() {
    this->FrontLeft = new WheelController(2);
    this->FrontRight = new WheelController(3);
    this->BackLeft = new WheelController(4);
    this->BackRight = new WheelController(5);

    leds = (float) 48;
    InitializePixels();

    if (START_RPM > 0) {
        rpm = START_RPM;
        CalculateDelayTime();
    }
}

void RpmController::InitializePixels() {
    for(int i = 0; i < LED_COUNT; i++) {
        if (i < spoke1start + spokeSize && i >= spoke1start) {
            pixels[i] = 0x00FF00;
        }
        else if (i < spoke2start + spokeSize && i >= spoke2start) {
            pixels[i] = 0x0000FF;
        }
        else if (i < spoke3start + spokeSize && i >= spoke3start) {
            pixels[i] = 0xFF0000;
        }
        else if (i < spoke4start + spokeSize && i >= spoke4start) {
            pixels[i] = 0x00FFFF;
        }
        else {
            pixels[i] = 0;
        }
        SetPixel(i, pixels[i]);
    }
    Show();
}

void RpmController::UpdateWheels() {
    uint64_t loopStartTime = micros();
    CheckForTimeout(loopStartTime);
    CheckForSerialRead();
    AdvanceWheels();
    DelayWithOffset(loopStartTime);
}

void RpmController::AdvanceWheels() {
    if (rpm == 0) {
        return;
    }

    advance_counter++;

    uint32_t first = pixels[0];
    for(int i = 0; i < LED_COUNT - 1; i++) {
        pixels[i] = pixels[i + 1];
        SetPixel(i, pixels[i]);
    }
    pixels[LED_COUNT - 1] = first;
    SetPixel(LED_COUNT - 1, first);
    Show();

}

void RpmController::SetPixel(int index, uint32_t color) {
    FrontLeft->SetPixel(index, color);
    FrontRight->SetPixel(index, color);
    BackLeft->SetPixel(index, color);
    BackRight->SetPixel(index, color);
}

void RpmController::Show() {
    FrontLeft->Show();
    FrontRight->Show();
    BackLeft->Show();
    BackRight->Show();
}

void RpmController::DelayWithOffset(uint64_t loopStartTime) {
    if (delayTime == 0 || rpm == 0) {
      delay(DEFAULT_DELAY_TIME_MS);
      return;
    }
  
    uint64_t loopEndTime = micros();

    // Check for edge case, micros 64-bit unsigned int rolled over to zero between start and end
    if (loopEndTime < loopStartTime) {
        delay(delayTime);
        return;
    }

    int timeElapsedMs = (loopEndTime - loopStartTime) / 1000;
    int updatedDelayTime = delayTime - timeElapsedMs;
    if (updatedDelayTime > 0) {
        delay(updatedDelayTime);
    }
}

void RpmController::CheckForSerialRead() {
    if (Serial.available() > 0) {
        last_read_time = micros();

        if (USE_FLOAT_SERIAL) {
            serialReadBuffer.bytes[serialReadIndex++] = Serial.read();
            if (serialReadIndex >= SERIAL_BUFFER_SIZE) {
                serialReadIndex = 0;
                
                if (serialReadBuffer.rpm >= 0 && serialReadBuffer.rpm <= MAX_RPM) {
                    rpm = serialReadBuffer.rpm;
                    CalculateDelayTime();
                }
            }
        }
        else {
            rpm = (int) Serial.read();
            CalculateDelayTime();
        }
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
    if (loopStartTime < last_read_time || (TIMEOUT_SECONDS == 0 && serialReadIndex == 0)) {
        return;
    }

    int msSinceLastUpdate = (loopStartTime - last_read_time) / 1000;

    if (msSinceLastUpdate > READ_BUFFER_CLEAR_TIME_MS && serialReadIndex > 0) {
        serialReadIndex = 0;
        serialReadBuffer.rpm = 0;
    }

    if (TIMEOUT_SECONDS > 0 && msSinceLastUpdate >= TIMEOUT_SECONDS * 1000) {
        rpm = 0;
    }
}