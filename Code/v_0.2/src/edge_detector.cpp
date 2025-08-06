//
// Created by nick on 06/08/25.
//

#include <Arduino.h>
#include <edge_detector.h>

EdgeDetector::EdgeDetector(const int pin, const unsigned long debounce_delay, const unsigned long hold_threshold) {
    this->pin = pin;
    this->debounce_delay = debounce_delay;
    this->hold_threshold = hold_threshold;

    hold_detected = false;
    is_pressed = false;
    prev_state = false;
    current_state = false;
    last_change_time = 0;
    press_start_time = 0;
}

ButtonEvent EdgeDetector::detectButtonEvent() {
    const bool raw_state = digitalRead(pin);
    ButtonEvent event = NO_EVENT;

    // Debounce logic
    if (raw_state != this->current_state) {
        last_change_time = millis();
    }

    // If enough time has passed, accept the change
    if ((millis() - last_change_time) > debounce_delay) {
        if (raw_state != prev_state) {

            if (!raw_state && prev_state) {
                is_pressed = true;
                press_start_time = millis();
                hold_detected = false;
                event = BUTTON_PRESSED;
            }

            if (raw_state && !prev_state) {
                is_pressed = false;
                if (hold_detected) {
                    event = BUTTON_RELEASED_AFTER_HOLD;
                } else {
                    event = BUTTON_RELEASED;
                }
                hold_detected = false;
            }

            prev_state = raw_state;
        }
    }

    if (is_pressed && !hold_detected) {
        if ((millis() - press_start_time) >= hold_threshold) {
            hold_detected = true;
            event = BUTTON_HELD;
        }
    }

    current_state = raw_state;
    return event;
}
