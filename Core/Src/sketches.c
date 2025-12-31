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

typedef struct {
	uint32_t c1, c2, c3;
} Palette;

#define NUM_PALETTES 4
const Palette palettes[NUM_PALETTES] = {
	// {RGB(180, 5, 0), RGB(255, 90, 0), RGB(255, 180, 0)},
	{RGB(220, 20, 0), RGB(255, 120, 20), RGB(255, 210, 60)},
	{RGB(255, 0, 75), RGB(200, 140, 0), RGB(220, 220, 100)},
	{RGB(255, 90, 0), RGB(200, 255, 0), RGB(100, 200, 140)},
	{RGB(205, 0, 90), RGB(255, 140, 0), RGB(220, 120, 180)},
};

uint32_t get_square_palette(float angle, Palette p) {
	// Using cos/sin ensures that as angle wraps, the color wraps perfectly
	float color_sample = cosf(angle) * 0.5f + 0.5f; 
	
	// Lerp between Deep Red, Burnt Orange, and Amber
	// uint32_t red = rgb(180, 5, 0);
	// uint32_t orange = rgb(255, 60, 0);
	// uint32_t gold = rgb(255, 160, 0);
	
	if (color_sample < 0.5f) {
		return lerp_rgb(p.c1, p.c2, color_sample * 2.0f);
	} else {
		return lerp_rgb(p.c2, p.c3, (color_sample - 0.5f) * 2.0f);
	}
}

uint32_t get_palette(float angle, Palette p) {
	// 1. Normalize angle (-PI to PI) to a 0.0 to 1.0 range
	// Adding TAU before fmod handles negative angles safely
	float t = fmodf(angle, TAU);
	if (t < 0) t += TAU;
	t /= TAU; 

	// 2. Divide 1.0 into three equal zones
	float zone = t * 3.0f;

	if (zone < 1.0f) {
		// Zone 1: C1 to C2 (0° to 120°)
		return lerp_rgb(p.c1, p.c2, zone);
	} else if (zone < 2.0f) {
		// Zone 2: C2 to C3 (120° to 240°)
		return lerp_rgb(p.c2, p.c3, zone - 1.0f);
	} else {
		// Zone 3: C3 to C1 (240° to 360°)
		return lerp_rgb(p.c3, p.c1, zone - 2.0f);
	}
}

int next_palette(int current_palette) {
	int roll = rand() % 100;
	int next_palette = current_palette;

	if (roll < 70) {
		while ((next_palette = rand() % 2) == current_palette);
	} else {
		while ((next_palette = (rand() % 2) + 2) == current_palette);
	}
	return next_palette;
}

#define NUM_GLINT_COLORS 9
static uint32_t glint_colors[NUM_GLINT_COLORS] = {
	RGB(40, 200, 90),
	RGB(0, 99, 190),
	RGB(0, 140, 140),
	RGB(140, 140, 0),
	RGB(110, 140, 110),
	RGB(200, 150, 255),
	RGB(150, 230, 255),
	RGB(255, 200, 150),
	RGB(255, 255, 200),
};

void organic_vibe_system(LEDBuffer leds) {
	static float pulse_phase = 0;
	static float rot_dir = 1.0f;
	static float color_phase = 0;
	static float state_switch_timer = 0;

	static float glint_level = 0.f;
	static float glint_phase = 0;

	static int glint_color_idx = 0;
	static int last_glint_color_idx = 0;
	static float glint_seed;

	static float ambient_phase = 0;
	static Palette current_palette = palettes[0];
	static int current_palette_idx = 0;
	static int target_palette_idx;
	static float palette_transition = 1;
	static float palette_timer = 0;

	static float pulse_dir_timer = 0;
	static float pulse_dir = 1;

	static float show_spiral = 0.f;
	static float show_spiral_accum = 0.0003f;

	// Logic: State Machine "Director"
	state_switch_timer += secondsDiff;
	if (state_switch_timer > 30.0f) { // Every 30 seconds, change vibe
		if (rand() % 10 > 5) {
			rot_dir *= -1.0f; // Flip rotation
		}
		state_switch_timer = 0;
	}

	pulse_dir_timer += secondsDiff;
	if (pulse_dir_timer > 37) {
		pulse_dir_timer = 0;
		if (rand() % 10 > 9) {
			pulse_dir = -1;
		} else {
			pulse_dir = 1;
		}
	}

	palette_timer += secondsDiff;
	palette_transition += secondsDiff * 0.05f;
	if (palette_timer > 191 || button.pressed) {
		if (button.pressed) {
			pulse_dir = -1;
		}
		palette_timer = 0;
		target_palette_idx = next_palette(current_palette_idx);
		palette_transition = 0.f;
	}

	// Perform the palette "melt"
	if (palette_transition < 1.0f) {
		// Smoothstep makes the start and end of the transition feel natural
		float s = palette_transition * palette_transition * (3.0f - 2.0f * palette_transition);
		
		current_palette.c1 = lerp_rgb(palettes[current_palette_idx].c1, palettes[target_palette_idx].c1, s);
		current_palette.c2 = lerp_rgb(palettes[current_palette_idx].c2, palettes[target_palette_idx].c2, s);
		current_palette.c3 = lerp_rgb(palettes[current_palette_idx].c3, palettes[target_palette_idx].c3, s);
	} else {
		current_palette_idx = target_palette_idx;
		current_palette = palettes[current_palette_idx];
	}

	// Override band 2 for the shell slowly
	show_spiral += show_spiral_accum;
	if (show_spiral > 0.6f) {
		show_spiral_accum = -0.001f;
	}
	if (show_spiral < 0.f) {
		show_spiral_accum = 0.0003f;
	}

	// Audio-reactive speeds
	pulse_phase += pulse_dir * secondsDiff * (0.2f + band_smooth_fast[0] * 2.0f); // Bass drives speed
	glint_phase += secondsDiff * 0.1f;
	color_phase += secondsDiff * 0.1f * rot_dir;
	color_phase = fmod(color_phase, TAU);
	if (color_phase < 0) color_phase += TAU;
	ambient_phase += secondsDiff * 0.04f; // Very slow drift

	// float hi_transient = fmax(band_magnitudes[7], fmax(band_magnitudes[6], fmax(band_magnitudes[5], band_magnitudes[4])));
	float hi_transient = fmax(band_transients[7], band_transients[6]);
	if (hi_transient > 0.01 && hi_transient > glint_level) {
		glint_level = hi_transient;
		if (glint_level > 0.6) {
			glint_level = 0.6;
		}
		last_glint_color_idx = glint_color_idx;
		glint_color_idx = rand() % NUM_GLINT_COLORS;
		glint_seed = (float)(rand() % 1000) * 0.1337f;
	} else {
		glint_level = glint_level * 0.99f;
	}

	if (glint_level < 0.0001) {
		glint_level = 0;
	}

	for (int i = 0; i < NUM_LEDS; i++) {
		float x = led_positions[i][0];
		float y = led_positions[i][1];
		float r = sqrtf(x*x + y*y);
		float theta = atan2f(y, x);

		// --- LAYER 0: THE SHELL SKELETON (Ambient) ---
		float theta_rotating = theta + ambient_phase;
		float twist_factor = 4.0f * 2.39996f;
		float warped_theta = theta_rotating - r * twist_factor;
		// We use a high power (powf) to make the 'arms' thinner/sharper
		float shell_wave = (sinf(7.0f * warped_theta) + 1.0f) * 0.5f;
		shell_wave = powf(shell_wave, 3.0f); // Sharpen the shell lines

		// --- LAYER 1: THE BASE (Angular Drift) ---
		uint32_t base_color = get_palette(theta + color_phase, current_palette);
		// uint32_t base_color = get_palette(theta + color_phase, palettes[1]);
		
		// --- LAYER 2: THE PULSE (Radial) ---
		// We use r and band_smooth_slow[1] (Low mids) to create "breathing"
		float wave = sinf(r * 12.0f - pulse_phase * 2.0f);
		float pulse_intensity = (wave + 1.0f) * 0.5f * (0.2f + band_smooth_slow[1]);
		// Scale base color by pulse intensity
		uint32_t final_c = lerp_rgb(rgb(20,0,0), base_color, pulse_intensity);

		// incorporate shell
		//Double lerp so less pulse intense areas are red
		uint32_t shell_color = lerp_rgb(rgb(100, 0, 80), rgb(0, 80, 20), pulse_intensity);
		shell_color = lerp_rgb(rgb(0, 0, 0), shell_color, fmax(show_spiral, band_smooth_slow[2]));
		shell_color = lerp_rgb(rgb(0,0,0), shell_color, shell_wave);
		final_c = add_rgb(shell_color, final_c);

		// --- LAYER 3: THE SPIRAL GLINT (High Freq) ---
		// This uses the Golden Angle logic you had in biggest_spirals
		// Triggered only by treble transients (Band 6/7)
		float spiral_theta = theta - r * 2.39996f; 
		float glint_sin = sinf(spiral_theta * 4.0f + glint_phase * 4.0f);
		float glint = (glint_sin > 0.9f) ? glint_level : 0;

		if (glint_level > 0.04f) {
			// Create a "stipple" pattern that changes based on the current glint event
			// Using (i + last_glint_color_idx) ensures the 'random' spots change every hit
			float noise = sinf((float)i * 1.618f + glint_seed); 
			
			if (noise > 0.96f) { // Only affect ~10% of pixels
				// Masking: Only show in the dark parts of the pulse
				// We use (1.0 - pulse_intensity) to find the "valleys"
				float masking = powf(1.0f - pulse_intensity, 3.0f);
				
				float brightness = glint_level * masking;
				uint32_t col = lerp_rgb(0, glint_colors[glint_color_idx], brightness);
				final_c = add_rgb(final_c, col);
			}
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
		float twist_factor = 4 * 2.39996f;
		// twist_factor = -twist_factor / 2;
		float warped_theta = theta_rotating - r * twist_factor;

		float val = ((sinf(7 * warped_theta) + 1.f) / 2.f);

		float col = lerp_rgb(rgb(0, 0, 0), rgb(0, 40, 0), val);
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
