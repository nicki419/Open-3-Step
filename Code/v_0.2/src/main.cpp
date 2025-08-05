#include <RBDdimmer.h>
#include <Wire.h>
#include <at24c256.h>
#include <Arduino.h>
#include <IO_Pins.h>
#include <brightness_controller.h>
#include <../include/Storage_Addresses.h>


constexpr int brightnessLevels[3] = {30, 60, 100}; // brightness % levels
int currentBrightnessIndex = 0;

dimmerLamp dimmer(DIMMER_PIN);
AT24C256* eeprom = nullptr;
BrightnessController* brightnessController = nullptr;

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

  Wire.begin();
  dimmer.begin(NORMAL_MODE, ON);
  eeprom = new AT24C256(0x50, Wire);
  brightnessController = new BrightnessController(&dimmer, eeprom);


  // Load brightness first, before any delays
  loadBrightness();
  brightnessController->applyBrightness();

  // Quick single blink to show we're alive
  digitalWrite(STATUS_LED_PIN, HIGH);
  delay(50);
  digitalWrite(STATUS_LED_PIN, LOW);
}

void loop() {
  bool currentMains = (digitalRead(MAINS_DETECT_PIN) == LOW);

  if (!mainsPresent && currentMains) {
    unsigned long interruption = millis() - mainsLostTime;
    if (interruption <= 350) {
      brightnessController->cycleBrightness();
      brightnessController->applyBrightness();
      brightnessController->saveBrightness(currentBrightnessIndex);
    }
  }

  if (mainsPresent && !currentMains) {
    mainsLostTime = millis();
  }

  mainsPresent = currentMains;
  brightnessController->handleStatusLED();
  delay(20);
}

void onZeroCross() {
  dimmer.setState(ON);
}