//
// Created by nick on 05/08/25.
//

#ifndef SETTINGS_CONTROLLER_H
#define SETTINGS_CONTROLLER_H

#include <RBDdimmer.h>
#include <at24c256.h>
#include <OneButtonTiny.h>
#include <IO_Pins.h>
#include <brightness_controller.h>

class SettingsController {
private:
    dimmerLamp* dimmer;
    AT24C256* eeprom;
    BrightnessController* brightness_controller;
    OneButtonTiny* button;
    unsigned long last_LED_Blink;

    enum BlinkState {
        BLINK_IDLE,
        BLINK_START,
        BLINK_ON,
        BLINK_OFF
    };

    BlinkState blink_state = BLINK_IDLE;
    int blink_count = 0;
    unsigned long blink_timer = 0;


    void setAndSaveBrightness(int currentBrightnessIndex) const;

public:
    SettingsController(dimmerLamp *dimmer, AT24C256 *eeprom, BrightnessController *brightness_controller,
                       OneButtonTiny *button);

    void settings_loop(int currentBrightnessIndex);
    void handle_button_click(int currentBrightnessIndex) const;
    void handle_button_hold(int currentBrightnessIndex) const;
};

#endif //SETTINGS_CONTROLLER_H
