//
// Created by nick on 05/08/25.
//

#include <ac_switch_controller.h>
#include <IO_Pins.h>
#include <OneButtonTiny.h>

#include "Storage_Addresses.h"

ACSwitchController::ACSwitchController(dimmerLamp *dimmer, AT24C256 *eeprom,
                                       BrightnessController *brightness_controller, OneButtonTiny *button): dimmer(
        dimmer),
    eeprom(eeprom),
    brightness_controller(brightness_controller),
    button(button),
    currentTestIndex(0) {
}

void ACSwitchController::handleACSwitch(int delay_ms) const {
    switch (brightness_controller->getCurrentBrightnessIndex()) {
        // Medium Brightness. Toggle AC Switch Once.
        case 1:
            digitalWrite(AC_SW_PIN, HIGH);
            delay(delay_ms);
            digitalWrite(AC_SW_PIN, LOW);
            break;

            // Lowest Brightness. Toggle AC Switch Twice.
        case 2:
            digitalWrite(AC_SW_PIN, HIGH);
            delay(delay_ms);
            digitalWrite(AC_SW_PIN, LOW);
            delay(delay_ms);
            digitalWrite(AC_SW_PIN, HIGH);
            delay(delay_ms);
            digitalWrite(AC_SW_PIN, LOW);
            break;
        default:
            break;
    }
}

void ACSwitchController::handle_button_click() const {
    // Blink to show how long it will be
    for (int i = 0; i < currentTestIndex; i++) {
        digitalWrite(STATUS_LED_PIN, HIGH);
        delay(100);
        digitalWrite(STATUS_LED_PIN, LOW);
        delay(100);
    }
    delay(400);

    digitalWrite(AC_SW_PIN, HIGH);
    delay(100*currentTestIndex);
    digitalWrite(AC_SW_PIN, LOW);
}

void ACSwitchController::handle_button_double_click() {
    currentTestIndex++;
    handle_button_click();
}