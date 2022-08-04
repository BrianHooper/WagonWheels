#include "Logger.h"

void Logger::println(String value) {
    if (LOGGER_ENABLED) {
        Serial.println(value);
    }
}

void Logger::print(String value) {
    if (LOGGER_ENABLED) {
        Serial.print(value);
    }
}
void Logger::println(String value, int delayMs) {
    if (LOGGER_ENABLED) {
        delay(delayMs);
        Serial.println(value);
    }
}

void Logger::print(String value, int delayMs) {
    if (LOGGER_ENABLED) {
        delay(delayMs);
        Serial.print(value);
    }
}