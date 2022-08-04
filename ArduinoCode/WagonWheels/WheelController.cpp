#include <Arduino.h>
#include "WheelController.h"
#include "Logger.h"

WheelController::WheelController(int pin) { 
    this->strip = new Adafruit_NeoPixel(48, pin, NEO_RGB + NEO_KHZ400); 
    strip->begin();
    strip->setBrightness(BRIGHTNESS);
}

void WheelController::Show() {
    strip->show();
}

void WheelController::SetPixel(int index, uint32_t color) {
  uint8_t redChannel = (uint8_t)((color & 0x00FF0000) >> 16);
  uint8_t blueChannel = (uint8_t)((color & 0x0000FF00) >> 8);
  uint8_t greenChannel = (uint8_t)(color & 0x000000FF);
  strip->setPixelColor(index, redChannel, blueChannel, greenChannel);
}

