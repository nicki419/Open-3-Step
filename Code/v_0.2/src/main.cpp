#include <RBDdimmer.h>
#include <Wire.h>
#include <at24c256.h>
#include <Arduino.h>
#include <IO_Pins.h>
#include <brightness_controller.h>
#include <Storage_Addresses.h>
#include <OneButtonTiny.h>

#include "ac_switch_controller.h"
#include "settings_controller.h"
#include "delay.h"

#define AC_SW_DELAY 75

constexpr int brightnessLevels[3] = {30, 60, 100}; // brightness % levels

dimmerLamp dimmer(DIMMER_PIN);
AT24C256* eeprom = nullptr;
BrightnessController* brightnessController = nullptr;
SettingsController* settingsController = nullptr;
ACSwitchController* acSwitchController = nullptr;
OneButtonTiny* button = nullptr;

unsigned long mainsLostTime = 0;
bool mainsPresent = true;
unsigned long lastBlinkTime = 0;
bool ledOn = false;
bool inSettingsLoop = false;
bool inACSwitchLoop = false;

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
  pinMode(AC_SW_PIN, OUTPUT);

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
  acSwitchController = new ACSwitchController(&dimmer, eeprom, brightnessController, button);

  loadBrightness();
  brightnessController->applyBrightness();
  acSwitchController->handleACSwitch(AC_SW_DELAY);

#ifdef DEBUG
  digitalWrite(STATUS_LED_PIN, HIGH);
  delay(50);
  digitalWrite(STATUS_LED_PIN, LOW);
#endif

  button->attachClick([]() {
    if (!inSettingsLoop && !inACSwitchLoop) {
      handleClick();
    }
    else if (inSettingsLoop) {
      settingsController->handle_button_click(brightnessController->getCurrentBrightnessIndex());
    }
    else if (inACSwitchLoop) {
      acSwitchController->handle_button_click();
    }
  });

  button->attachDoubleClick([]() {
    digitalWrite(STATUS_LED_PIN, HIGH);
    delay(100);
    digitalWrite(STATUS_LED_PIN, LOW);
    delay(100);

    if (!inACSwitchLoop) {
      inSettingsLoop = !inSettingsLoop;
      if (inSettingsLoop) {
        brightnessController->setCurrentBrightnessIndex(0);
        settingsController->handle_button_click(brightnessController->getCurrentBrightnessIndex());
      }
    }
    else {
      acSwitchController->handle_button_double_click();
    }
  });

  button->attachLongPressStart([]() {
    if (!inSettingsLoop) {
      inACSwitchLoop = !inACSwitchLoop;
      if (inACSwitchLoop) {
        acSwitchController->currentTestIndex = 1;
        digitalWrite(STATUS_LED_PIN, LOW);
        delay(200);
      }
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
  if (inACSwitchLoop) {
    return;
  }

  // Mains Power Cycle Check
  if (!mainsPresent && currentMains) {
    if (const unsigned long interruption = millis() - mainsLostTime; interruption <= DELAY) {
      brightnessController->cycleBrightness();
      const int currentBrightnessIndex = brightnessController->getCurrentBrightnessIndex();
      brightnessController->applyBrightness();
      brightnessController->saveBrightnessIndex(currentBrightnessIndex);
    }
    else {
      acSwitchController->handleACSwitch(AC_SW_DELAY);
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