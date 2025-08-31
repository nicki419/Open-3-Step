//
// Created by nick on 05/08/25.
//

#ifndef AC_SWITCH_CONTROLLER_H
#define AC_SWITCH_CONTROLLER_H

#include <RBDdimmer.h>
#include <at24c256.h>
#include <OneButtonTiny.h>
#include <IO_Pins.h>
#include <brightness_controller.h>

class ACSwitchController {
private:
    dimmerLamp* dimmer;
    AT24C256* eeprom;
    BrightnessController* brightness_controller;
    OneButtonTiny* button;

public:
    ACSwitchController(dimmerLamp *dimmer, AT24C256 *eeprom, BrightnessController *brightness_controller,
                       OneButtonTiny *button);

    void handleACSwitch(int delay_ms) const;
    void handle_button_click() const;
    void handle_button_double_click();
    int currentTestIndex;
};

#endif //AC_SWITCH_CONTROLLER_H
