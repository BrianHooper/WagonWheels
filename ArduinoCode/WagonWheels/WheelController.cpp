#include <Arduino.h>
#include "WheelController.h"

WheelController::WheelController(int pin) { 
    this->strip = new Adafruit_NeoPixel(48, pin, NEO_RGB + NEO_KHZ400); 
    strip->begin();
    strip->setBrightness(BRIGHTNESS);

    Serial.print("Wheel Constructor: ");
    Serial.print(pin);
    Serial.print("\r\n");

    WheelController::SetInitialPattern();
    strip->show();
}

void WheelController::SetInitialPattern() {
    for(int i = 0; i < LED_COUNT; i++) {
        if (i < 3) {
            pixels[i] = 0xFF0000;
        }
        else {
            pixels[i] = 0;
        }
        SetPixel(i, pixels[i]);
    }
    strip->show();
}

void WheelController::SetPixel(int index, uint32_t color) {
  uint8_t redChannel = (uint8_t)((color & 0x00FF0000) >> 16);
  uint8_t blueChannel = (uint8_t)((color & 0x0000FF00) >> 8);
  uint8_t greenChannel = (uint8_t)(color & 0x000000FF);
  strip->setPixelColor(index, redChannel, blueChannel, greenChannel);
}

void WheelController::Advance()
{
    uint32_t first = pixels[0];
    for(int i = 0; i < LED_COUNT - 1; i++) {
        pixels[i] = pixels[i + 1];
        WheelController::SetPixel(i, pixels[i]);
    }
    pixels[LED_COUNT - 1] = first;
    WheelController::SetPixel(LED_COUNT - 1, first);
    strip->show();
}