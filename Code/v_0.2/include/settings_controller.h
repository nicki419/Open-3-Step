//
// Created by nick on 05/08/25.
//

#ifndef SETTINGS_CONTROLLER_H
#define SETTINGS_CONTROLLER_H

#include <RBDdimmer.h>
#include <at24c256.h>
#include <edge_detector.h>
#include <IO_Pins.h>
#include <brightness_controller.h>

class SettingsController {
private:
    dimmerLamp* dimmer;
    AT24C256* eeprom;
    BrightnessController* brightness_controller;
    EdgeDetector* edge_detector;

    void setAndSaveBrightness(int currentBrightnessIndex) const;

public:
    SettingsController(dimmerLamp *dimmer, AT24C256 *eeprom, BrightnessController *brightness_controller,
                       EdgeDetector *edge_detector);

    void settings_loop() const;
};

#endif //SETTINGS_CONTROLLER_H
