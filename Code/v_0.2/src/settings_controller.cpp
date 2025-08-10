//
// Created by nick on 05/08/25.
//

#include <settings_controller.h>
#include <IO_Pins.h>
#include <edge_detector.h>

#include "Storage_Addresses.h"

void SettingsController::setAndSaveBrightness(const int currentBrightnessIndex) const {
    // Read from ADC
    const int pin = currentBrightnessIndex == 0 ? POT1_PIN : currentBrightnessIndex == 1 ? POT2_PIN : POT3_PIN;
    const int address = currentBrightnessIndex == 0
                            ? BRIGHTNESS_VALUE1_ADDR
                            : currentBrightnessIndex == 1
                                  ? BRIGHTNESS_VALUE2_ADDR
                                  : BRIGHTNESS_VALUE3_ADDR;
    const int pot_reading =  map(analogRead(pin), 0, 1023, 1, 100);

    // Set dimmer and write on EEPROM
    dimmer->setPower(pot_reading);
    if (const bool result = brightness_controller->writeAndVerify(address, pot_reading); !result) {
        for (int i = 0; i < 5; i++) {
            digitalWrite(STATUS_LED_PIN, HIGH);
            delay(50);
            digitalWrite(STATUS_LED_PIN, LOW);
            delay(50);
        }
    }
}

SettingsController::SettingsController(dimmerLamp *dimmer, AT24C256 *eeprom,
                                       BrightnessController *brightness_controller, EdgeDetector *edge_detector):
    dimmer(dimmer),
    eeprom(eeprom),
    brightness_controller(brightness_controller),
    edge_detector(edge_detector) {

}

void SettingsController::settings_loop() const {
    bool running = true;
    int currentBrightnessIndex = 0;

    while (running) {
        switch (edge_detector->detectButtonEvent()) {
            case BUTTON_RELEASED:
                setAndSaveBrightness(currentBrightnessIndex);
                break;

            case BUTTON_HELD:
                currentBrightnessIndex = (currentBrightnessIndex + 1) % 3;
                setAndSaveBrightness(currentBrightnessIndex);
                break;

            case BUTTON_PRESSED:
            case BUTTON_RELEASED_AFTER_HOLD:
            case NO_EVENT:
            default:
                break;
        }
    }
}
