#include <RBDdimmer.h>
#include <Wire.h>
#include <at24c256.h>

#define BRIGHTNESS_INDEX_ADDR 0

const int mainsDetectPin = 4;     // Mains detector output (LOW = mains present)
const int zeroCrossPin = 2;       // Zero-cross input for dimmer timing
const int dimmerPin = 3;          // Dimmer PSM output
const int statusLedPin = 12;      // External status LED

const int brightnessLevels[3] = {30, 60, 100}; // brightness % levels
int currentBrightnessIndex = 0;

dimmerLamp dimmer(dimmerPin);
AT24C256 eeprom(0x50, Wire);  // Create EEPROM object

unsigned long mainsLostTime = 0;
bool mainsPresent = true;
unsigned long lastBlinkTime = 0;
bool ledOn = false;

bool writeAndVerify(byte address, byte data) {
  for (int attempt = 0; attempt < 3; attempt++) {
    // Temporarily disable dimmer
    dimmer.setPower(0);
    dimmer.setState(OFF);
    delayMicroseconds(100);
    
    eeprom.write(address, data);
    delayMicroseconds(100);
    
    byte readBack = eeprom.read(address);
    if (readBack == data) {
      dimmer.setState(ON);
      applyBrightness();
      return true;
    }
  }
  
  dimmer.setState(ON);
  applyBrightness();
  return false;
}

byte readWithRetry(byte address, byte defaultValue) {
  for (int attempt = 0; attempt < 3; attempt++) {
    dimmer.setPower(0);
    dimmer.setState(OFF);
    delayMicroseconds(100);
    
    byte value = eeprom.read(address);
    if (value >= 0 && value < 3) {
      dimmer.setState(ON);
      applyBrightness();
      return value;
    }
  }
  
  dimmer.setState(ON);
  applyBrightness();
  return defaultValue;
}

void saveBrightness() {
  if (!writeAndVerify(BRIGHTNESS_INDEX_ADDR, currentBrightnessIndex)) {
    // Write failed - indicate error by rapid LED flashing
    for (int i = 0; i < 5; i++) {
      digitalWrite(statusLedPin, HIGH);
      delay(50);
      digitalWrite(statusLedPin, LOW);
      delay(50);
    }
  }
}

void loadBrightness() {
  currentBrightnessIndex = readWithRetry(BRIGHTNESS_INDEX_ADDR, 0);
}

void setup() {
  pinMode(mainsDetectPin, INPUT);
  pinMode(statusLedPin, OUTPUT);
  
  Wire.begin();
  dimmer.begin(NORMAL_MODE, ON);
  
  // Load brightness first, before any delays
  loadBrightness();
  applyBrightness();
  
  // Quick single blink to show we're alive
  digitalWrite(statusLedPin, HIGH);
  delay(50);
  digitalWrite(statusLedPin, LOW);
}

void loop() {
  bool currentMains = (digitalRead(mainsDetectPin) == LOW);

  if (!mainsPresent && currentMains) {
    unsigned long interruption = millis() - mainsLostTime;
    if (interruption <= 350) {
      cycleBrightness();
      applyBrightness();
      saveBrightness();  // Save new brightness state
    }
  }

  if (mainsPresent && !currentMains) {
    mainsLostTime = millis();
  }

  mainsPresent = currentMains;
  handleStatusLED();
  delay(20);
}

void applyBrightness() {
  dimmer.setPower(brightnessLevels[currentBrightnessIndex]);
}

void cycleBrightness() {
  currentBrightnessIndex = (currentBrightnessIndex + 1) % 3;
}

void handleStatusLED() {
  int brightness = brightnessLevels[currentBrightnessIndex];
  unsigned long now = millis();

  if (brightness <= 30) {
    // Slow blink
    if (now - lastBlinkTime >= 800) {
      ledOn = !ledOn;
      digitalWrite(statusLedPin, ledOn ? HIGH : LOW);
      lastBlinkTime = now;
    }
  }
  else if (brightness <= 70) {
    // Medium blink
    if (now - lastBlinkTime >= 300) {
      ledOn = !ledOn;
      digitalWrite(statusLedPin, ledOn ? HIGH : LOW);
      lastBlinkTime = now;
    }
  }
  else {
    // Solid ON for highest brightness
    digitalWrite(statusLedPin, HIGH);
  }
}

void onZeroCross() {
  dimmer.setState(ON);
}