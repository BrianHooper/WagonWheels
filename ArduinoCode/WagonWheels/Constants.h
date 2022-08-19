#ifndef Constants_h
#define Constants_h

// ---- Debugging ----
#define LOGGER_ENABLED false
#define START_RPM 4

// ---- Serial communication
#define BAUD_RATE 9600
#define SERIAL_BUFFER_SIZE 4
#define READ_BUFFER_CLEAR_TIME_MS 500
#define USE_FLOAT_SERIAL false

// ---- LED/Neopixel parameters ----
#define BRIGHTNESS 50
#define LED_COUNT 128
#define LED_COUNT_RIM 64
#define PIN_FRONT_LEFT 2
#define PIN_FRONT_RIGHT 3
#define PIN_BACK_LEFT 4
#define PIN_BACK_RIGHT 5
#define NUM_SPOKES 8

// ---- RPM Calculations
#define DEFAULT_DELAY_TIME_MS 10
#define TIMEOUT_SECONDS 0
#define MAX_RPM 120

#endif
