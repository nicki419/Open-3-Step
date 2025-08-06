//
// Created by nick on 05/08/25.
//

#ifndef EDGE_DETECTOR_H
#define EDGE_DETECTOR_H

enum ButtonEvent {
    NO_EVENT,
    BUTTON_PRESSED,
    BUTTON_RELEASED,
    BUTTON_HELD,
    BUTTON_RELEASED_AFTER_HOLD
};

class EdgeDetector {
private:
    bool prev_state = false;
    bool current_state = false;
    unsigned long last_change_time = 0;
    bool is_pressed;
    unsigned long press_start_time;
    bool hold_detected;

public:
    EdgeDetector(int pin, unsigned long debounce_delay, unsigned long hold_threshold);
    ButtonEvent detectButtonEvent();
    int pin;
    unsigned long debounce_delay;
    unsigned long hold_threshold;
};

#endif //EDGE_DETECTOR_H
