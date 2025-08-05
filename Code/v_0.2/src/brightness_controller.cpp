#include <../include/brightness_controller.h>
#include <RBDdimmer.h>
#include <IO_Pins.h>
#include <Arduino.h>
#include <at24c256.h>
#include <../include/Storage_Addresses.h>

BrightnessController::BrightnessController(dimmerLamp* dimmer, AT24C256* eeprom) : dimmer(dimmer), eeprom(eeprom) {

}

bool BrightnessController::writeAndVerify(const byte address, const byte data) const {
    for (int attempt = 0; attempt < 3; attempt++) {
        // Temporarily disable dimmer
        dimmer->setPower(0);
        dimmer->setState(OFF);
        delayMicroseconds(100);

        eeprom->write(address, data);
        delayMicroseconds(100);

        if (const byte readBack = eeprom->read(address); readBack == data) {
            dimmer->setState(ON);
            applyBrightness();
            return true;
        }
    }

    dimmer->setState(ON);
    applyBrightness();
    return false;
}

byte BrightnessController::readWithRetry(const byte address, const byte defaultValue) const {
    for (int attempt = 0; attempt < 3; attempt++) {
        dimmer->setPower(0);
        dimmer->setState(OFF);
        delayMicroseconds(100);

        if (const byte value = eeprom->read(address); value < 3) {
            dimmer->setState(ON);
            applyBrightness();
            return value;
        }
    }

    dimmer->setState(ON);
    applyBrightness();
    return defaultValue;
}

void BrightnessController::saveBrightness(const int currentBrightnessIndex) const {
    if (!writeAndVerify(BRIGHTNESS_INDEX_ADDR, currentBrightnessIndex)) {
        // Write failed - indicate error by rapid LED flashing
        for (int i = 0; i < 5; i++) {
            digitalWrite(STATUS_LED_PIN, HIGH);
            delay(50);
            digitalWrite(STATUS_LED_PIN, LOW);
            delay(50);
        }
    }
}

void BrightnessController::applyBrightness() const {
    dimmer->setPower(brightnessLevels[currentBrightnessIndex]);
}

void BrightnessController::cycleBrightness() {
    currentBrightnessIndex = (currentBrightnessIndex + 1) % 3;
}

void BrightnessController::handleStatusLED() {
    const int brightness = brightnessLevels[currentBrightnessIndex];
    const unsigned long now = millis();

    if (brightness <= 30) {
        // Slow blink
        if (now - lastBlinkTime >= 800) {
            ledOn = !ledOn;
            digitalWrite(STATUS_LED_PIN, ledOn ? HIGH : LOW);
            lastBlinkTime = now;
        }
    }
    else if (brightness <= 70) {
        // Medium blink
        if (now - lastBlinkTime >= 300) {
            ledOn = !ledOn;
            digitalWrite(STATUS_LED_PIN, ledOn ? HIGH : LOW);
            lastBlinkTime = now;
        }
    }
    else {
        // Solid ON for highest brightness
        digitalWrite(STATUS_LED_PIN, HIGH);
    }
}

int BrightnessController::getCurrentBrightness() const {
    return brightnessLevels[currentBrightnessIndex];
}

int BrightnessController::getCurrentBrightnessIndex() const {
    return currentBrightnessIndex;
}