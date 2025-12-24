#include "sketches.h"

#include "util.h"

#include <stdbool.h>
#include <math.h>

static float seconds;

void rgb_pulse(LEDBuffer leds) {
	for (int i = 0; i < NUM_LEDS; i++) {
		float x = led_positions[i][0];
		float y = led_positions[i][1];
		
		// Distance from center
		float r = sqrtf(x*x + y*y);

		// Get angle from center (-π to π)
		float theta = atan2f(y, x);
		float hue = fmodf((theta / 3.14159f) * 180.0f + 180.0f + seconds * 16.0f, 360.0f);
		
		// Wave that expands outward
		float wave = sinf(r * 8.0f + seconds * 3.f);  // frequency 8, speed 3

		// Map [-1, 1] to [0, 1]
		float brightness = (wave + 1.0f) * 0.5f;
		
		// uint8_t level = (uint8_t)(brightness * 255.0f);

		HsvColorF color = {hue, 1, brightness * .6f};
		leds[i] = hsvFToRgbRainbow(color);
	}
}

void full_white(LEDBuffer leds) {
	for (int i = 0; i < NUM_LEDS; i++) {
		leds[i] = rgb(255, 255, 255);
	}
}

void color_pulse(LEDBuffer leds) {
	for (int i = 0; i < NUM_LEDS; i++) {
		float x = led_positions[i][0];
		float y = led_positions[i][1];
		
		// Distance from center
		float r = sqrtf(x*x + y*y);
		
		// Wave that expands outward
		float wave = sinf(r * 8.0f - seconds * 1.2f);  // frequency 8, speed 3
		
		// Map [-1, 1] to [0, 1]
		float brightness = (wave + 1.0f) * 0.5f;
		
		uint8_t level = (uint8_t)(brightness * 255.0f);
		leds[i] = rgb(level, level/3, 0);
	}
}

void radial_hsv(LEDBuffer leds) {
	// HSV
	for (int i = 0; i < NUM_LEDS; i++) {
		float x = led_positions[i][0];
		float y = led_positions[i][1];
		
		float theta = atan2f(y, x);

		float hue = fmodf((theta / 3.14159f) * 180.0f + 180.0f + seconds * 16.0f, 360.0f);
		HsvColorF color = {hue, 1, 1};
		leds[i] = hsvFToRgbRainbow(color);
	}
}

void radial_spirals(LEDBuffer leds) {
	for (int i = 0; i < NUM_LEDS; i++) {
		float x = led_positions[i][0];
		float y = led_positions[i][1];
		
		// Polar coordinates
		float r = sqrtf(x*x + y*y);
		float theta = atan2f(y, x);  // Angle in radians [-π, π]
		
		// Spiral pattern: combine angle and radius
		float spiral = sinf(theta * 2.0f + r * 5.0f - seconds * 2.0f);

		
		float brightness = (spiral + 1.0f) * 0.5f;
		uint8_t level = (uint8_t)(brightness * 255.0f);
		
		leds[i] = rgb(level, 0, 0);
	}
}

void multisine(LEDBuffer leds) {
	for (int i = 0; i < NUM_LEDS; i++) {
		float x = led_positions[i][0];
		float y = led_positions[i][1];
		
		// Multiple moving sine waves
		float v1 = sinf(x * 5.0f + seconds);
		float v2 = sinf(y * 5.0f + seconds * 1.3f);
		float v3 = sinf((x + y) * 4.0f + seconds * 0.7f);
		float v4 = sinf(sqrtf(x*x + y*y) * 8.0f - seconds * 2.0f);
		
		float combined = (v1 + v2 + v3 + v4) / 4.0f;
		float brightness = (combined + 1.0f) * 0.5f;
		
		uint8_t level = (uint8_t)(brightness * 255.0f);
		leds[i] = rgb(level, 0, level/3);
	}
}

void shader_spiral(LEDBuffer leds) {
	for (int i = 0; i < NUM_LEDS; i++) {
		float x = led_positions[i][0];
		float y = led_positions[i][1];


	}
}

void draw(LEDBuffer leds) {
	for (int i = 0; i < 89; i++) {
		leds[i] = 0;
	}
	seconds = tickCount / 1000.0f;

	// full_white(leds);
	// rgb_pulse(leds);
	// color_pulse(leds);
	// radial_hsv(leds);
	// radial_spirals(leds);
	multisine(leds);
}