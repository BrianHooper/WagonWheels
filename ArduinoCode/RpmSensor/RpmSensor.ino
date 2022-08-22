#define BAUD_RATE 9600
#define MIN_RPM 0
#define MAX_RPM 200
#define HALL_PIN 2
#define HALL_THRESHOLD 80.0
#define SENSORS_PER_WHEEL 3.0
#define MULTIPLIER 0.20
#define TIMEOUT_SECONDS 7.0

float hall_counts_per_measurement = HALL_THRESHOLD / SENSORS_PER_WHEEL;

void setup() {
    Serial.begin(BAUD_RATE);
    pinMode(HALL_PIN, INPUT);
}

bool Timeout(float start_time) {
    float end_time = micros();
    float seconds_elapsed = ((end_time - start_time) / 1000000.0);
    return seconds_elapsed >= TIMEOUT_SECONDS;
}

uint8_t GetRpmShort(float start_time) {
    float end_time = micros();
    if (end_time < start_time) {
        return 0;
    }

    float time_passed_seconds = (end_time - start_time) / 1000000.0;
    float revolutions_per_second = hall_counts_per_measurement / time_passed_seconds;
    float revolutions_per_minute = revolutions_per_second * 60.0;
    float rpm = revolutions_per_minute * MULTIPLIER;
    if (rpm < MIN_RPM || rpm > MAX_RPM) {
        return 0;
    }

    return (uint8_t) rpm;

}

void loop() {
    float hall_count = 0;
    float start = micros();
    bool on_state = false;

    while(hall_count < HALL_THRESHOLD && !Timeout(start)) {
        if (digitalRead(HALL_PIN) == 0) {
            if (on_state == false) {
                on_state = true;
                hall_count += 1.0;
            }
        } else{
            on_state = false;
        }
    }

    if (hall_count >= HALL_THRESHOLD) {
        uint8_t rpm = GetRpmShort(start);
        Serial.write(rpm);
    }
    else {
        Serial.write((uint8_t)0);
    }
    delay(2);
}
