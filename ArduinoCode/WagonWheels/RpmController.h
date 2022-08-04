#ifndef RpmController_h
#define RpmController_h

#include <stdint.h>
#include "WheelController.h"

#define DEFAULT_DELAY_TIME_MS 10
#define TIMEOUT_SECONDS 0

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
        int rpm = 0;
        int delayTime = 0;
        uint64_t last_read_time = 0;

        void CheckForTimeout(uint64_t loopStartTime);
        void AdvanceWheels();
		void CalculateDelayTime();
        void UpdateRPM();
        void DelayWithOffset(uint64_t loopStartTime);
};

#endif
