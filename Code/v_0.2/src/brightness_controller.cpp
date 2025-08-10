#include <brightness_controller.h>
#include <RBDdimmer.h>
#include <Arduino.h>
#include <at24c256.h>
#include <Storage_Addresses.h>

#ifdef DEBUG
#include <IO_Pins.h>
#endif

BrightnessController::BrightnessController(dimmerLamp* dimmer, AT24C256* eeprom) : dimmer(dimmer), eeprom(eeprom) {

}

bool BrightnessController::writeAndVerify(const byte address, const byte data) const {
    dimmer->setPower(0);
    dimmer->setState(OFF);
    delay(50);
    for (int attempt = 0; attempt < 3; attempt++) {
        eeprom->write(address, data);
        delay(20);

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
            return value;
        }
    }

    dimmer->setState(ON);
    return defaultValue;
}

void BrightnessController::saveBrightnessIndex(const int currentBrightnessIndex) const {
    if (!writeAndVerify(BRIGHTNESS_INDEX_ADDR, currentBrightnessIndex)) {
        // Write failed - indicate error by rapid LED flashing
#ifdef DEBUG
        for (int i = 0; i < 5; i++) {
            digitalWrite(STATUS_LED_PIN, HIGH);
            delay(50);
            digitalWrite(STATUS_LED_PIN, LOW);
            delay(50);
        }
#endif
    }
}

void BrightnessController::applyBrightness() const {
    const int address = currentBrightnessIndex == 0
                            ? BRIGHTNESS_VALUE1_ADDR
                            : currentBrightnessIndex == 1
                                  ? BRIGHTNESS_VALUE2_ADDR
                                  : BRIGHTNESS_VALUE3_ADDR;
    const int brightness = readWithRetry(address, 0);
    dimmer->setPower(brightness);
}

void BrightnessController::cycleBrightness() {
    currentBrightnessIndex = (currentBrightnessIndex + 1) % 3;
}

void BrightnessController::handleStatusLED() {
#ifdef DEBUG
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
#endif
}

int BrightnessController::getCurrentBrightness() const {
    return brightnessLevels[currentBrightnessIndex];
}

int BrightnessController::getCurrentBrightnessIndex() const {
    return currentBrightnessIndex;
}

void BrightnessController::setCurrentBrightnessIndex(const int currentBrightnessIndex) {
    this->currentBrightnessIndex = currentBrightnessIndex;
}
