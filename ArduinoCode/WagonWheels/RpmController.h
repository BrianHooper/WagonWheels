#ifndef RpmController_h
#define RpmController_h

#include <stdint.h>
#include "Constants.h"
#include <FastLED.h>

class RpmController {
	public:
		RpmController();
        void UpdateWheels();
	private:
        float leds;
        float rpm = 0;
        int delayTime = 0;
        uint64_t last_read_time = 0;
        uint64_t advance_counter = 0;
        CRGB pixels[LED_COUNT];

        uint8_t serialReadIndex;
        union {
            float rpm;
            uint8_t bytes[4];
        } serialReadBuffer;

        int spokeSize = 8;
        int spoke1start = 0;
        int spoke2start = 16;
        int spoke3start = 32;
        int spoke4start = 48;

        // {spoke_start_index, spoke_end_index, rim_index_reference}
        uint8_t spokes[8][3] {
            {64, 72, 0},
            {72, 80, 8},
            {80, 88, 16},
            {88, 96, 24},
            {96, 104, 32},
            {104, 112, 40},
            {112, 120, 48},
            {120, 128, 56}
        };

        void InitializePixels();
        void CheckForTimeout(uint64_t loopStartTime);
        void AdvanceWheels();
        void CalculateDelayTime();
        void CheckForSerialRead();
        void UpdateRPM();
        void DelayWithOffset(uint64_t loopStartTime);
        void SetSpokes();
        void FillSection(int start, CRGB color);
};

#endif
