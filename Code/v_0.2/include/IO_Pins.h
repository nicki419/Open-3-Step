//
// Created by nick on 05/08/25.
//

#ifndef IO_PINS_H
#define IO_PINS_H

#define MAINS_DETECT_PIN PD4     // Mains detector output (LOW = mains present)
#define ZERO_CROSS_PIN PD3       // Zero-cross input for dimmer timing
#define DIMMER_PIN PD2           // Dimmer PSM output
#define STATUS_LED_PIN PD6      // External status LED

#define POT1_PIN A2             // ADC2 (PC2/A2)
#define POT2_PIN A1             // ADC1 (PC1/A1)
#define POT3_PIN A0             // ADC0 (PC0/A0)
#define BUTTON_PIN A3           // ADC3 (PC3/A3) Used as digital input

#endif //IO_PINS_H
