#ifndef WheelController_h
#define WheelController_h

#include <Adafruit_NeoPixel.h>
#include <math.h>
#include "Constants.h"

class WheelController {
	public:
		WheelController(int pinA);
		void SetPixel(int index, uint32_t color);
		void Show();
	private:
		Adafruit_NeoPixel* strip;
};
#endif
