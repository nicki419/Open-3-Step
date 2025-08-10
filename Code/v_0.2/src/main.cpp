#include <RBDdimmer.h>
#include <Wire.h>
#include <at24c256.h>
#include <Arduino.h>
#include <IO_Pins.h>
#include <brightness_controller.h>
#include <Storage_Addresses.h>
#include <OneButtonTiny.h>

#include "settings_controller.h"


constexpr int brightnessLevels[3] = {30, 60, 100}; // brightness % levels

dimmerLamp dimmer(DIMMER_PIN);
AT24C256* eeprom = nullptr;
BrightnessController* brightnessController = nullptr;
SettingsController* settingsController = nullptr;
OneButtonTiny* button = nullptr;

unsigned long mainsLostTime = 0;
bool mainsPresent = true;
unsigned long lastBlinkTime = 0;
bool ledOn = false;
bool inSettingsLoop = false;

void loadBrightness() {
  const int currentBrightnessIndex = brightnessController->readWithRetry(BRIGHTNESS_INDEX_ADDR, 2);
  brightnessController->setCurrentBrightnessIndex(currentBrightnessIndex);
}

void handleClick() {

}

void handleHold() {

}

void setup() {
  pinMode(MAINS_DETECT_PIN, INPUT);
  pinMode(STATUS_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);
  pinMode(POT1_PIN, INPUT);
  pinMode(POT2_PIN, INPUT);
  pinMode(POT3_PIN, INPUT);

  Wire.begin();
  dimmer.begin(NORMAL_MODE, ON);
  eeprom = new AT24C256(0x50, Wire);
  brightnessController = new BrightnessController(&dimmer, eeprom);
  button = new OneButtonTiny(
        BUTTON_PIN,
        false,
        false
    );
  settingsController = new SettingsController(&dimmer, eeprom, brightnessController, button);

  loadBrightness();
  brightnessController->applyBrightness();

#ifdef DEBUG
  digitalWrite(STATUS_LED_PIN, HIGH);
  delay(50);
  digitalWrite(STATUS_LED_PIN, LOW);
#endif

  button->attachClick([]() {
    if (!inSettingsLoop) {
      handleClick();
    }
    else {
      settingsController->handle_button_click(brightnessController->getCurrentBrightnessIndex());
    }
  });

  button->attachDoubleClick([]() {
    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(100);
    digitalWrite(STATUS_LED_PIN, LOW);
    delay(100);

    inSettingsLoop = !inSettingsLoop;
    if (inSettingsLoop) {
      brightnessController->setCurrentBrightnessIndex(0);
      settingsController->handle_button_click(brightnessController->getCurrentBrightnessIndex());
    }
  });

  button->attachLongPressStart([]() {
    if (!inSettingsLoop) {
      handleHold();
    }
    else {
      settingsController->handle_button_hold(brightnessController->getCurrentBrightnessIndex());
    }
  });

}

void loop() {
  button->tick();
  const bool currentMains = (digitalRead(MAINS_DETECT_PIN) == LOW);

  if (inSettingsLoop) {
    settingsController->settings_loop(brightnessController->getCurrentBrightnessIndex());
    return;
  }

  if (!mainsPresent && currentMains) {
    if (const unsigned long interruption = millis() - mainsLostTime; interruption <= 350) {
      brightnessController->cycleBrightness();
      const int currentBrightnessIndex = brightnessController->getCurrentBrightnessIndex();
      brightnessController->applyBrightness();
      brightnessController->saveBrightnessIndex(currentBrightnessIndex);
    }
  }

  if (mainsPresent && !currentMains) {
    mainsLostTime = millis();
  }

  mainsPresent = currentMains;
  brightnessController->handleStatusLED();

  delay(10);
}

void onZeroCross() {
  dimmer.setState(ON);
}