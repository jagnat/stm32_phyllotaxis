#include "sketches.h"

#include "util.h"
#include "audio.h"

#include <stdbool.h>
#include <math.h>

static float seconds;

typedef struct {

} SpiralDefinition;

typedef struct {

} SketchParams;

typedef void (*Sketch)(LEDBuffer, SketchParams);

void biggest_spirals3(LEDBuffer leds) {

	for (int i = 0; i < NUM_LEDS; i++) {
		float x = led_positions[i][0];
		float y = led_positions[i][1];
		
		float r = sqrtf(x*x + y*y);
		float theta = atan2f(y, x);

		float unwarped_theta = theta - r * 1.6180339887f * 2.0f * 3.14159f;

		float theta_rotating = theta;// + seconds * 0.3f;
		float twist_factor = 21.0f * 2.39996f;
		float warped_theta = theta_rotating - r * twist_factor;

		float val = ((sinf(4 * unwarped_theta) + 1.f) / 2.f);
		// val = val * val * val * val * val * val * val * val; //^8
		val = val * val * val * val;

		float col = lerp_rgb(rgb(0, 0, 0), rgb(255, 0, 0), val);
		leds[i] = col;
	}
}

void biggest_spirals(LEDBuffer leds) {

	for (int i = 0; i < NUM_LEDS; i++) {
		float x = led_positions[i][0];
		float y = led_positions[i][1];
		
		float r = sqrtf(x*x + y*y);
		float theta = atan2f(y, x);

		float theta_rotating = theta + seconds * 0.2f;
		float twist_factor = 2.39996f;
		// twist_factor = -twist_factor / 2;
		float warped_theta = theta_rotating - r * twist_factor;

		float val = ((sinf(7 * warped_theta) + 1.f) / 2.f);
		// val = val * val * val * val * val * val * val * val; //^8
		// val = val * val * val * val;

		float col = lerp_rgb(rgb(40, 0, 40), rgb(255, 0, 0), val);
		leds[i] = col;
	}
}

void audio_sketch(LEDBuffer leds) {
	// Show each frequency band as a different color across the LEDs
	// Divide 89 LEDs into 8 bands (roughly 11 LEDs per band)
	
	int leds_per_band = NUM_LEDS / NUM_FFT_BANDS;  // ~11 LEDs per band
	
	for (int i = 0; i < NUM_LEDS; i++) {
		int band = i / leds_per_band;
		if (band >= NUM_FFT_BANDS) band = NUM_FFT_BANDS - 1;
		
		// Get energy for this band and amplify
		float energy = band_magnitudes[band] * 50.0f;  // 10x amplification
		if (energy > 1.0f) energy = 1.0f;
		
		// Each band gets a different hue
		float hue = (band / (float)NUM_FFT_BANDS) * 360.0f;
		
		// Brightness based on energy
		HsvColorF color = {hue, 1.0f, energy};
		leds[i] = hsvFToRgbRainbow(color);
	}
}

void rgb_pulse(LEDBuffer leds, SketchParams params) {
	for (int i = 0; i < NUM_LEDS; i++) {
		float x = led_positions[i][0];
		float y = led_positions[i][1];
		
		// Distance from center
		float r = sqrtf(x*x + y*y);

		float theta = atan2f(y, x);
		float hue = fmodf((theta / 3.14159f) * 180.0f + 180.0f + seconds * 16.0f, 360.0f);
		
		float wave = sinf(r * 8.0f + seconds * 0.5f);

		// Map [-1, 1] to [0, 1]
		float brightness = (wave + 1.0f) * 0.5f;

		HsvColorF color = {hue, 1, brightness * 1.0f};
		leds[i] = hsvFToRgbRainbow(color);
	}
}

void color_pulse(LEDBuffer leds, SketchParams params) {
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

void radial_hsv(LEDBuffer leds, SketchParams params) {
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

void radial_spirals(LEDBuffer leds, SketchParams params) {
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

void multisine(LEDBuffer leds, SketchParams params) {
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

Sketch sketches[] = {
	rgb_pulse,

};

void draw(LEDBuffer leds) {
	for (int i = 0; i < 89; i++) {
		leds[i] = 0;
	}
	seconds = tickCount / 1000.0f;

	biggest_spirals(leds);

	// audio_sketch(leds);
	// audio_radial_waves(leds);
	// full_white(leds);
	// rgb_pulse(leds);
	// color_pulse(leds);
	// radial_hsv(leds);
	// radial_spirals(leds);
	// multisine(leds);
	// nonzero_mic_test(leds);
	// test_rms(leds);
	// test_peak(leds);
}
