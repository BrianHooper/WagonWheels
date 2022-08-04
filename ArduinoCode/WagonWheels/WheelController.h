#ifndef WheelController_h
#define WheelController_h

#include <Adafruit_NeoPixel.h>
#include <math.h>

#define BRIGHTNESS 50
#define LED_COUNT 48

class WheelController {
	public:
		WheelController(int pinA);
        void Advance();
	private:
		Adafruit_NeoPixel* strip;

		// #TODO create one array of pixels in RpmController and reference it in this class
		uint32_t pixels[LED_COUNT];

		void SetPixel(int index, uint32_t color);
		void SetInitialPattern();
};
#endif
