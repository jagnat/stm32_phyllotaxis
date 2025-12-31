#include "sketches.h"

#include "util.h"
#include "audio.h"

#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

static float lastSeconds;
static float seconds;
static float secondsDiff;

typedef void (*Sketch)(LEDBuffer);

uint32_t get_warm_palette(float angle) {
	// Using cos/sin ensures that as angle wraps, the color wraps perfectly
	float color_sample = cosf(angle) * 0.5f + 0.5f; 
	
	// Lerp between Deep Red, Burnt Orange, and Amber
	uint32_t red = rgb(180, 5, 0);
	uint32_t orange = rgb(255, 60, 0);
	uint32_t gold = rgb(255, 160, 0);
	
	if (color_sample < 0.5f) {
		return lerp_rgb(red, orange, color_sample * 2.0f);
	} else {
		return lerp_rgb(orange, gold, (color_sample - 0.5f) * 2.0f);
	}
}

#define NUM_GLINT_COLORS 6
static uint32_t glint_colors[NUM_GLINT_COLORS] = {
	0x0000BE00,
	0x000000BE,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
};

void organic_vibe_system(LEDBuffer leds) {
	static float global_phase = 0;
	static float rot_dir = 1.0f;
	static float color_phase = 0;
	static float state_switch_timer = 0;

	static float glint_level = 0.f;
	static float glint_phase = 0;

	// Logic: State Machine "Director"
	state_switch_timer += secondsDiff;
	if (state_switch_timer > 30.0f) { // Every 30 seconds, change vibe
		rot_dir *= -1.0f; // Flip rotation
		state_switch_timer = 0;
	}

	// Audio-reactive speeds
	global_phase += secondsDiff * (0.2f + band_smooth_fast[0] * 2.0f); // Bass drives speed
	glint_phase += secondsDiff * 0.1f;
	color_phase += secondsDiff * 0.1f * rot_dir;
	color_phase = fmod(color_phase, TAU);
	if (color_phase < 0) color_phase += TAU;

	// float hi_transient = fmax(band_magnitudes[7], fmax(band_magnitudes[6], fmax(band_magnitudes[5], band_magnitudes[4])));
	float hi_transient = band_transients[7];

	if (hi_transient > 0.01 && hi_transient > glint_level) {
		glint_level = hi_transient;
		if (glint_level > 0.5) {
			glint_level = 0.5;
		}
	} else {
		glint_level = glint_level * 0.985f;
		// glint_level = glint_level * 0.9999;
	}

	if (glint_level < 0.0001) {
		glint_level = 0;
	}

	for (int i = 0; i < NUM_LEDS; i++) {
		float x = led_positions[i][0];
		float y = led_positions[i][1];
		float r = sqrtf(x*x + y*y);
		float theta = atan2f(y, x);

		// --- LAYER 1: THE BASE (Angular Drift) ---
		uint32_t base_color = get_warm_palette(theta + color_phase);
		
		// --- LAYER 2: THE PULSE (Radial) ---
		// We use r and band_smooth_slow[1] (Low mids) to create "breathing"
		float wave = sinf(r * 12.0f - global_phase * 2.0f);
		float pulse_intensity = (wave + 1.0f) * 0.5f * (0.3f + band_smooth_slow[1]);

		// --- LAYER 3: THE SPIRAL GLINT (High Freq) ---
		// This uses the Golden Angle logic you had in biggest_spirals
		// Triggered only by treble transients (Band 6/7)
		float spiral_theta = theta - r * 2.39996f; 
		float glint_sin = sinf(spiral_theta * 4.0f + glint_phase * 4.0f);
		float glint = (glint_sin > 0.9f) ? glint_level : 0;

		// --- COMPOSITION ---
		// Scale base color by pulse intensity
		uint32_t final_c = lerp_rgb(rgb(20,0,0), base_color, pulse_intensity);
		
		// Additively blend the white/cyan glint for treble
		if (glint > 0.04f) {
			int col_idx = rand() % NUM_GLINT_COLORS;
			uint32_t glint_color = lerp_rgb(0, glint_colors[0], glint);
			// uint32_t glint_color = rgb(0, 0, 255);
			final_c = add_rgb(final_c, glint_color); // You'll need to add add_rgb to util
		}

		leds[i] = final_c;
	}
}

void audio_spirals(LEDBuffer leds) {
	float bass = band_smooth_fast[0];
	float treble = band_magnitudes[5];

	for (int i = 0; i < NUM_LEDS; i++) {
		float x = led_positions[i][0];
		float y = led_positions[i][1];
		
		float r = sqrtf(x*x + y*y);
		float theta = atan2f(y, x);

		float theta_rotating = theta + seconds * bass * 0.1f;
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

void radial_audio_vibe(LEDBuffer leds) {
	static float phase = 0;
	static float color_drift = 0;
	static float wave_mod = 0;
	
	phase += secondsDiff + band_transients[0] * 0.3f;
	color_drift += secondsDiff * 0.1f + band_smooth_slow[2] * 0.05f;
	wave_mod += secondsDiff * 0.15f + band_transients[5] * 0.2f;
	
	for (int i = 0; i < NUM_LEDS; i++) {
		float x = led_positions[i][0];
		float y = led_positions[i][1];
		float r = sqrtf(x*x + y*y);
		float theta = atan2f(y, x);
		
		// Compress into warm range: 0-60° (red→yellow)
		float base_hue = fmodf((theta / 3.14159f) * 180.0f + 180.0f, 360.0f);
		float hue = fmodf(base_hue * 0.17f + color_drift * 40.0f, 360.0f);
		
		// Modulate wave frequency slightly
		float wave_freq = 8.0f + sinf(wave_mod) * 1.2f;
		float wave = sinf(r * wave_freq - phase * 0.5f);
		
		float brightness = (wave + 1.0f) * 0.5f;
		brightness += band_transients[6] * 0.25f;  // Treble sparkle
		if (brightness > 1.0f) brightness = 1.0f;
		
		HsvColorF color = {hue, 0.9f, brightness};
		leds[i] = hsvFToRgbRainbow(color);
	}
}

void radial_audio(LEDBuffer leds) {
	static float phase = 0;
	phase += secondsDiff + band_transients[0]*0.3;

	for (int i = 0; i < NUM_LEDS; i++) {
		float x = led_positions[i][0];
		float y = led_positions[i][1];
		
		// Distance from center
		float r = sqrtf(x*x + y*y);

		float theta = atan2f(y, x);
		float hue = fmodf((theta / 3.14159f) * 180.0f + 180.0f, 360.0f);
		
		float wave = sinf(r * 8.0f - phase * 0.5f);

		// Map [-1, 1] to [0, 1]
		float brightness = (wave + 1.0f) * 0.5f;

		HsvColorF color = {hue, 1, brightness * 1.0f};
		leds[i] = hsvFToRgbRainbow(color);
	}
}

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
		float energy = band_magnitudes[band];
		if (energy > 1.0f) energy = 1.0f;
		
		// Each band gets a different hue
		float hue = (band / (float)NUM_FFT_BANDS) * 360.0f;
		
		// Brightness based on energy
		HsvColorF color = {hue, 1.0f, energy};
		leds[i] = hsvFToRgbRainbow(color);
	}
}

void rgb_pulse(LEDBuffer leds) {
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

Sketch sketches[] = {
	rgb_pulse,

};

void draw(LEDBuffer leds) {
	for (int i = 0; i < 89; i++) {
		leds[i] = 0;
	}
	seconds = tickCount / 1000.0f;
	secondsDiff = seconds - lastSeconds;

	// biggest_spirals(leds);

	organic_vibe_system(leds);
	// radial_audio_vibe(leds);
	// audio_spirals(leds);
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

	lastSeconds = seconds;
}
