#ifndef RpmController_h
#define RpmController_h

#include <stdint.h>
#include "WheelController.h"
#include "Constants.h"

class RpmController {
	public:
		RpmController();
        void UpdateWheels();
	private:
		WheelController* FrontLeft;
        WheelController* FrontRight;
        WheelController* BackLeft;
        WheelController* BackRight;

        float leds;
        float rpm = 0;
        int delayTime = 0;
        uint64_t last_read_time = 0;
        uint64_t advance_counter = 0;
        uint32_t pixels[LED_COUNT];

        uint8_t serialReadIndex;
        union {
            float rpm;
            uint8_t bytes[4];
        } serialReadBuffer;

        int spokeSize = 3;
        int spoke1start = 0;
        int spoke2start = 12;
        int spoke3start = 24;
        int spoke4start = 36;

        void InitializePixels();
        void CheckForTimeout(uint64_t loopStartTime);
        void AdvanceWheels();
		void CalculateDelayTime();
        void CheckForSerialRead();
        void UpdateRPM();
        void DelayWithOffset(uint64_t loopStartTime);
        void Show();
        void SetPixel(int index, uint32_t color);
};

#endif
