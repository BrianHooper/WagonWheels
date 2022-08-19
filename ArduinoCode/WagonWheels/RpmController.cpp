#include "RpmController.h"
#include <Arduino.h>

RpmController::RpmController() {
    FastLED.addLeds<WS2811, PIN_FRONT_LEFT, RGB>(pixels, LED_COUNT);
    FastLED.addLeds<WS2811, PIN_FRONT_RIGHT, RGB>(pixels, LED_COUNT);
    FastLED.addLeds<WS2811, PIN_BACK_LEFT, RGB>(pixels, LED_COUNT);
    FastLED.addLeds<WS2811, PIN_BACK_RIGHT, RGB>(pixels, LED_COUNT);

    leds = (float) LED_COUNT;
    InitializePixels();

    if (START_RPM > 0) {
        rpm = START_RPM;
        CalculateDelayTime();
    }
}

void RpmController::InitializePixels() {
    // for(int i = 0; i < LED_COUNT; i++) {
    //     if (i < spoke1start + spokeSize && i >= spoke1start) {
    //         pixels[i] = 0x00FF00;
    //     }
    //     else if (i < spoke2start + spokeSize && i >= spoke2start) {
    //         pixels[i] = 0x0000FF;
    //     }
    //     else if (i < spoke3start + spokeSize && i >= spoke3start) {
    //         pixels[i] = 0xFF0000;
    //     }
    //     else if (i < spoke4start + spokeSize && i >= spoke4start) {
    //         pixels[i] = 0x00FFFF;
    //     }
    //     else {
    //         pixels[i] = 0;
    //     }
    // }
    //fill_rainbow(pixels, LED_COUNT_RIM, 0, 5); 
    // fill_rainbow(pixels, 128, 0, 5);
    // for(int i = 0; i < 8; i++) {
    //     CRGB randColor = CHSV(random8(),255,255);
    //     for(int j = 0; j < 8; j++) {
    //         pixels[i * 8 + j] = randColor;
    //     }
    // }
    FillSection(0, 0xff0000);
    FillSection(8, 0x55ff00);
    FillSection(16, 0x00aaff);
    FillSection(24, 0xff00ff);
    FillSection(32, 0xff0000);
    FillSection(40, 0x55ff00);
    FillSection(48, 0x00aaff);
    FillSection(56, 0xff00ff);


    fadeToBlackBy(pixels, LED_COUNT_RIM, 192);
    FastLED.show();
}

void RpmController::FillSection(int start, CRGB color) {
    for(int i = start; i < start + 8; i++) {
        pixels[i] = color;
    }
}

void RpmController::SetSpokes() {
    for(uint8_t spokeIdx = 0; spokeIdx < NUM_SPOKES; spokeIdx++) {
        uint8_t referencePixel = spokes[spokeIdx][2];
        CRGB referenceColor = pixels[referencePixel];
        for(uint8_t pixelIdx = spokes[spokeIdx][0]; pixelIdx < spokes[spokeIdx][1]; pixelIdx++) {
            pixels[pixelIdx] = referenceColor;
        }
    }
}

void RpmController::UpdateWheels() {
    uint64_t loopStartTime = micros();
    //CheckForTimeout(loopStartTime);
    CheckForSerialRead();
    AdvanceWheels();
    SetSpokes();
    DelayWithOffset(loopStartTime);
    FastLED.show();
}

void RpmController::AdvanceWheels() {
    if (rpm == 0) {
        return;
    }
    CRGB first = pixels[0];
    for(int i = 0; i < LED_COUNT_RIM - 1; i++) {
        pixels[i] = pixels[i + 1];
    }
    pixels[LED_COUNT_RIM - 1] = first;
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
