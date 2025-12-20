#pragma once

#include <stdint.h>

#define NUM_LEDS 89

typedef uint32_t LEDBuffer[NUM_LEDS];

extern volatile uint32_t tickCount;

extern const uint8_t small_spirals_lens[];
extern const uint8_t small_spirals[][5];
extern const uint8_t large_spirals_lens[];
extern const uint8_t large_spirals[][7];
extern const float led_positions[NUM_LEDS][2];

// Adafruit gamma LUT
extern const uint8_t gamma8[];

// Translate from 4 bit nibbles from color channels
// into SPI neopixel bits
// 0 -> 100
// 1 -> 110
// Starting from MSB
extern const uint16_t neopixel_spi_encode_nibble_lut[];

uint32_t make_urgb(uint8_t r, uint8_t g, uint8_t b);