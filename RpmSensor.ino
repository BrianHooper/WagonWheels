int hall_pin = 2;
float hall_thresh = 10.0;
float multiplier = 3.5;

void setup() {
    Serial.begin(115200);
    pinMode(hall_pin, INPUT);
}

void loop() {
    float hall_count = 0.0;
    float start = micros();
    bool on_state = false;

    while(true) {
        if (digitalRead(hall_pin) == 0) {
            if (on_state == false){
                on_state = true;
                hall_count += 1.0;
            }
        } else{
            on_state = false;
        }

        if (hall_count >= hall_thresh) {
            break;
        }
    }

    if (hall_count > 0) {
        float end_time = micros();
        float time_passed = ((end_time - start) / 1000000.0);
        float rpm_val = ((hall_count/time_passed) * 60.0) / multiplier;
        Serial.print(rpm_val);
        delay(1);
    }
}