#ifndef MyClass_h
#define MyClass_h

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>

#define BRIGHTNESS 50
#define LED_COUNT 48

class Wheel {
	public:
		Wheel(int pinA, int pinB, int pinC);
		void advance();
	private:
		void setPixel(Adafruit_NeoPixel strip, int index, uint32_t color);
		Adafruit_NeoPixel strip;
		uint32_t pixels[LED_COUNT];
		uint32_t prevBuffer[LED_COUNT];
		uint32_t nextBuffer[LED_COUNT];
		int _pin;
};
#endif