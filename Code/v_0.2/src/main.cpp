#include <RBDdimmer.h>
#include <Wire.h>
#include <at24c256.h>
#include <Arduino.h>
#include <IO_Pins.h>
#include <brightness_controller.h>
#include <../include/Storage_Addresses.h>

#include "settings_controller.h"


constexpr int brightnessLevels[3] = {30, 60, 100}; // brightness % levels
int currentBrightnessIndex = 0;

dimmerLamp dimmer(DIMMER_PIN);
AT24C256* eeprom = nullptr;
BrightnessController* brightnessController = nullptr;
SettingsController* settingsController = nullptr;
EdgeDetector* edge_detector = nullptr;

unsigned long mainsLostTime = 0;
bool mainsPresent = true;
unsigned long lastBlinkTime = 0;
bool ledOn = false;

void loadBrightness() {
  currentBrightnessIndex = brightnessController->readWithRetry(BRIGHTNESS_INDEX_ADDR, 0);
}

void setup() {
  pinMode(MAINS_DETECT_PIN, INPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  Wire.begin();
  dimmer.begin(NORMAL_MODE, ON);
  eeprom = new AT24C256(0x50, Wire);
  brightnessController = new BrightnessController(&dimmer, eeprom);
  edge_detector = new EdgeDetector(
        BUTTON_PIN,
        10,
        1000
    );
  settingsController = new SettingsController(&dimmer, eeprom, brightnessController, edge_detector);

  loadBrightness();
  brightnessController->applyBrightness();

#ifdef DEBUG
  digitalWrite(STATUS_LED_PIN, HIGH);
  delay(50);
  digitalWrite(STATUS_LED_PIN, LOW);
#endif
}

void loop() {
  const bool currentMains = (digitalRead(MAINS_DETECT_PIN) == LOW);

  if (!mainsPresent && currentMains) {
    if (const unsigned long interruption = millis() - mainsLostTime; interruption <= 350) {
      brightnessController->cycleBrightness();
      brightnessController->applyBrightness();
      brightnessController->saveBrightness(currentBrightnessIndex);
    }
  }

  if (mainsPresent && !currentMains) {
    mainsLostTime = millis();
  }

  if (edge_detector->detectButtonEvent() == BUTTON_HELD) {
    // Enter settings loop. Only exited by switching off mains
    settingsController->settings_loop();
  }

  mainsPresent = currentMains;
  brightnessController->handleStatusLED();
  delay(20);
}

void onZeroCross() {
  dimmer.setState(ON);
}