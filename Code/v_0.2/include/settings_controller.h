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

    void setAndSaveBrightness(int currentBrightnessIndex) const;

public:
    SettingsController(dimmerLamp *dimmer, AT24C256 *eeprom, BrightnessController *brightness_controller,
                       OneButtonTiny *button);

    void handle_button_click(int currentBrightnessIndex) const;
    void handle_button_hold(int currentBrightnessIndex) const;
};

#endif //SETTINGS_CONTROLLER_H
