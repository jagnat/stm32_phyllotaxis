#include "sketches.h"

#include "util.h"

#include <stdbool.h>
#include <math.h>

void draw(LEDBuffer leds) {
    for (int i = 0; i < 89; i++) {
      leds[i] = 0;
    }

    static uint8_t level = 100;
    static bool ascending = true;

    level = ascending? (level + 1) : (level - 1);
    if (level == 100) {
      ascending = true;
    }
    if (level == 255) {
      ascending = false;
    }

    float time = tickCount / 1000.0f;  // Convert to seconds
    
    for (int i = 0; i < NUM_LEDS; i++) {
        float x = led_positions[i][0];
        float y = led_positions[i][1];
        
        // Distance from center
        float r = sqrtf(x*x + y*y);
        
        // Wave that expands outward
        float wave = sinf(r * 8.0f - time * 0.4f);  // frequency 8, speed 3
        
        // Map [-1, 1] to [0, 1]
        float brightness = (wave + 1.0f) * 0.5f;
        
        uint8_t level = (uint8_t)(brightness * 255.0f);
        leds[i] = make_urgb(gamma8[level], gamma8[level/3], 0);
    }

    // for (int i = 0; i < NUM_LEDS; i++) {
    //     float x = led_positions[i][0];
    //     float y = led_positions[i][1];
        
    //     // Polar coordinates
    //     float r = sqrtf(x*x + y*y);
    //     float theta = atan2f(y, x);  // Angle in radians [-π, π]
        
    //     // Spiral pattern: combine angle and radius
    //     float spiral = sinf(theta * 7.0f + r * 5.0f - time * 2.0f);

        
    //     float brightness = (spiral + 1.0f) * 0.5f;
    //     uint8_t level = (uint8_t)(brightness * 255.0f);
        
    //     urgb_buffer[i] = make_urgb(gamma8[level], 0, 0);
    // }

    // for (int i = 0; i < NUM_LEDS; i++) {
    //     float x = led_positions[i][0];
    //     float y = led_positions[i][1];
        
    //     // Multiple moving sine waves
    //     float v1 = sinf(x * 5.0f + time);
    //     float v2 = sinf(y * 5.0f + time * 1.3f);
    //     float v3 = sinf((x + y) * 4.0f + time * 0.7f);
    //     float v4 = sinf(sqrtf(x*x + y*y) * 8.0f - time * 2.0f);
        
    //     float combined = (v1 + v2 + v3 + v4) / 4.0f;
    //     float brightness = (combined + 1.0f) * 0.5f;
        
    //     uint8_t level = (uint8_t)(brightness * 255.0f);
    //     urgb_buffer[i] = make_urgb(0, gamma8[level], gamma8[level/2]);
    // }

    // static int ringLevel = 0;
    // int numSmallSpirals = sizeof(small_spirals_lens) / sizeof(small_spirals_lens[0]);

    // for (int i = 0; i < small_spirals_lens[ringLevel]; i++) {
    //   urgb_buffer[small_spirals[ringLevel][i]] = make_urgb(gamma8[level], 0,gamma8[level / 3]);
    // }

    // ringLevel += 1;
    // if (ringLevel >= numSmallSpirals) {
    //   ringLevel = 0;
    // }

    // static int ringLevel = 0;
    // int numLargeSpirals = sizeof(large_spirals_lens) / sizeof(large_spirals_lens[0]);

    // for (int i = 0; i < large_spirals_lens[ringLevel]; i++) {
    //   urgb_buffer[large_spirals[ringLevel][i]] = make_urgb(gamma8[level], 0,gamma8[level / 3]);
    // }
    // int level2 = (ringLevel + numLargeSpirals / 2) % numLargeSpirals;
    // for (int i = 0; i < large_spirals_lens[ringLevel]; i++) {
    //   urgb_buffer[large_spirals[level2][i]] = make_urgb(gamma8[level], 0,gamma8[level / 3]);
    // }

    // ringLevel += 1;
    // if (ringLevel >= numLargeSpirals) {
    //   ringLevel = 0;
    // }

    // for (int i = 0; i < 89; i++) {
    //   uint32_t col = i == count? make_urgb(1, 0, 0) : make_urgb(0x0, 0x0, 0x0);
    //   if (i == (count + 30) % NUM_LEDS) {
    //     col = make_urgb(255, 0, 0);
    //   }
    //   if (i == (count + 60) % NUM_LEDS) {
    //     col = make_urgb(5, 0, 0);
    //   }
    //   urgb_buffer[i] = col;
    // }
}