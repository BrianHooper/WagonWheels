#ifndef Logger_h
#define Logger_h

#include <Arduino.h>
#include "Constants.h"

class Logger {
	public:
        static void println(String value);
        static void print(String value);
        static void println(String value, int delayMs);
        static void print(String value, int delayMs);
};

#endif
