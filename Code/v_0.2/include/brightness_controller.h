#ifndef BRIGHTNESS_CONTROLLER_H
#define BRIGHTNESS_CONTROLLER_H
#include <RBDdimmer.h>
#include <at24c256.h>

class BrightnessController {
private:
    int brightnessLevels[3] = {30, 70, 100};
    int currentBrightnessIndex = 0;
    bool ledOn = false;
    unsigned long lastBlinkTime = 0;

    dimmerLamp* dimmer;
    AT24C256* eeprom;
    
public:
    BrightnessController(dimmerLamp* dimmer, AT24C256* eeprom);
    
    // Public methods
    void applyBrightness() const;
    void cycleBrightness();
    void handleStatusLED();

    bool writeAndVerify(byte address, byte data) const;
    byte readWithRetry(const byte address, const byte defaultValue) const;
    void saveBrightness(const int currentBrightnessIndex) const;
    
    // Getters
    int getCurrentBrightness() const;
    int getCurrentBrightnessIndex() const;
};

#endif