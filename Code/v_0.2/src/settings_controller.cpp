//
// Created by nick on 05/08/25.
//

#include <settings_controller.h>
#include <IO_Pins.h>
#include <OneButtonTiny.h>

#include "Storage_Addresses.h"

int readPotAverage(const int pin, const int samples = 5) {
    int total = 0;
    for (int i = 0; i < samples; i++) {
        analogRead(pin);
        delay(5);
        total += analogRead(pin);
        delay(5);
    }
    return total / samples;
}

void SettingsController::setAndSaveBrightness(const int currentBrightnessIndex) const {
    // Read from ADC
    brightness_controller->setCurrentBrightnessIndex(currentBrightnessIndex);
    const int pin = currentBrightnessIndex == 0 ? POT1_PIN : currentBrightnessIndex == 1 ? POT2_PIN : POT3_PIN;
    const int address = currentBrightnessIndex == 0
                            ? BRIGHTNESS_VALUE1_ADDR
                            : currentBrightnessIndex == 1
                                  ? BRIGHTNESS_VALUE2_ADDR
                                  : BRIGHTNESS_VALUE3_ADDR;
    analogRead(pin);
    delay(10);
    const int pot_reading =  static_cast<int>(map(readPotAverage(pin), 0, 1023, 1, 100));

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
                                       BrightnessController *brightness_controller, OneButtonTiny *button):
    dimmer(dimmer),
    eeprom(eeprom),
    brightness_controller(brightness_controller),
    button(button) {
    last_LED_Blink = millis();

}

void SettingsController::settings_loop(const int currentBrightnessIndex) {
    const unsigned long currentTime = millis();
    
    // Check if we should start a new blink sequence
    if (currentTime - last_LED_Blink >= 1000) {
        blink_state = BLINK_START;
        blink_count = 0;
        blink_timer = currentTime;
        last_LED_Blink = currentTime;
    }
    
    // Handle the blinking state machine
    if (blink_state != BLINK_IDLE) {
        switch (blink_state) {
            case BLINK_START:
                if (blink_count <= currentBrightnessIndex) {
                    digitalWrite(STATUS_LED_PIN, HIGH);
                    blink_state = BLINK_ON;
                    blink_timer = currentTime;
                } else {
                    blink_state = BLINK_IDLE;
                }
                break;
                
            case BLINK_ON:
                if (currentTime - blink_timer >= 100) {
                    digitalWrite(STATUS_LED_PIN, LOW);
                    blink_state = BLINK_OFF;
                    blink_timer = currentTime;
                }
                break;
                
            case BLINK_OFF:
                if (currentTime - blink_timer >= 100) {
                    blink_count++;
                    blink_state = BLINK_START;
                    blink_timer = currentTime;
                }
                break;
            default:
                break;
        }
    }
}

void SettingsController::handle_button_click(const int currentBrightnessIndex) const {
    setAndSaveBrightness(currentBrightnessIndex);
}

void SettingsController::handle_button_hold(int currentBrightnessIndex) const {
    currentBrightnessIndex = (currentBrightnessIndex + 1) % 3;
    setAndSaveBrightness(currentBrightnessIndex);
}