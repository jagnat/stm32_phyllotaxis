#pragma once

#include <stdint.h>

#define NUM_LEDS 89

#define PI         3.1415926535f
#define TAU        6.2831853072f
#define PI_OVER_2  1.5707963268f
#define PI_OVER_3  1.0471975512f
#define PI_OVER_4  0.7854981634f
#define PI_OVER_6  0.5235987756f
#define PI_OVER_8  0.3926990817f
#define PI_OVER_12 0.2617993878f

#define INV_PI     0.3183098862f
#define INV_TAU    0.1591549431f
#define EULER      2.7182818285f

typedef struct _HsvColorF {
	float h, s, v;
} HsvColorF;

typedef uint32_t LEDBuffer[NUM_LEDS];

extern volatile uint32_t tickCount;

#define NUM_SMALL_SPIRALS 21
extern const uint8_t small_spirals_lens[];
extern const uint8_t small_spirals[][5];
#define NUM_LARGE_SPIRALS 13
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

uint32_t rgb(uint8_t r, uint8_t g, uint8_t b);
void apply_gamma_to_leds(LEDBuffer buffer);

uint32_t hsvFToRgbFullSpectrum(HsvColorF hsv);
uint32_t hsvFToRgbRainbow(HsvColorF hsv);