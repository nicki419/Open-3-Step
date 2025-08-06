#ifndef BRIGHTNESS_CONTROLLER_H
#define BRIGHTNESS_CONTROLLER_H
#include <RBDdimmer.h>
#include <at24c256.h>

class BrightnessController {
private:
    int brightnessLevels[3] = {30, 70, 100};
    int currentBrightnessIndex = 0;
#ifdef DEBUG
    bool ledOn = false;
    unsigned long lastBlinkTime = 0;
#endif

    dimmerLamp* dimmer;
    AT24C256* eeprom;
    
public:
    BrightnessController(dimmerLamp* dimmer, AT24C256* eeprom);

    void applyBrightness() const;
    void cycleBrightness();

#ifdef DEBUG
    void handleStatusLED();
#else
    static void handleStatusLED();
#endif

    [[nodiscard]] bool writeAndVerify(byte address, byte data) const;
    [[nodiscard]] byte readWithRetry(byte address, byte defaultValue) const;
    void saveBrightness(int currentBrightnessIndex) const;
    
    // Getters
    [[nodiscard]] int getCurrentBrightness() const;
    [[nodiscard]] int getCurrentBrightnessIndex() const;
};

#endif