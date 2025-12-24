#include "util.h"

#include <math.h>

volatile uint32_t tickCount;

const uint8_t small_spirals_lens[] = {5, 4, 4, 4, 4, 5, 4, 4, 5, 4, 4, 4, 4, 5, 4, 4, 5, 4, 4, 4, 4};
const uint8_t small_spirals[][5] = {
	{88, 87, 86, 85, 84,},
	{80, 81, 82, 83, 0xff,},
	{79, 78, 77, 76, 0xff,},
	{72, 73, 74, 75, 0xff,},
	{71, 70, 69, 68, 0xff,},
	{63, 64, 65, 66, 67,},
	{62, 61, 60, 59, 0xff,},
	{55, 56, 57, 58, 0xff,},
	{54, 53, 52, 51, 50,},
	{46, 47, 48, 49, 0xff,},
	{45, 44, 43, 42, 0xff,},
	{38, 39, 40, 41, 0xff,},
	{37, 36, 35, 34, 0xff,},
	{29, 30, 31, 32, 33,},
	{28, 27, 26, 25, 0xff,},
	{21, 22, 23, 24, 0xff,},
	{20, 19, 18, 17, 16,},
	{12, 13, 14, 15, 0xff,},
	{11, 10,  9,  8, 0xff,},
	{4,  5,   6,  7, 0xff,},
	{3,  2,   1,  0, 0xff,},
};

const uint8_t large_spirals_lens[] = {7, 7, 7, 7, 6, 7, 7, 7, 7, 6, 7, 7, 7,};
const uint8_t large_spirals[][7] = {
	{72, 79, 81, 86,  1,  7,  8,},
	{80, 87,  2,  6,  9, 15, 16,},
	{88,  3,  5, 10, 14, 17, 24,},
	{ 4, 11, 13, 18, 23, 25, 33,},
	{12, 19, 22, 26, 32, 34, 0xff,},
	{20, 21, 27, 31, 35, 41, 42,},
	{28, 30, 36, 40, 43, 49, 50,},
	{29, 37, 39, 44, 48, 51, 58,},
	{38, 45, 47, 52, 57, 59, 67,},
	{46, 53, 56, 60, 66, 68, 0xff,},
	{54, 55, 61, 65, 69, 75, 76,},
	{62, 64, 70, 74, 77, 83, 84,},
	{63, 71, 73, 78, 82, 85,  0,},
};

const float led_positions[NUM_LEDS][2] = {
	{-0.295600f, -0.881845f},
	{-0.147642f, -0.769175f},
	{-0.035599f, -0.635367f},
	{0.038057f, -0.488029f},
	{0.179926f, -0.394470f},
	{0.168341f, -0.555471f},
	{0.116204f, -0.717929f},
	{0.023421f, -0.873812f},
	{0.232423f, -0.936628f},
	{0.301129f, -0.760752f},
	{0.328102f, -0.585689f},
	{0.315025f, -0.419325f},
	{0.392868f, -0.255296f},
	{0.466666f, -0.401150f},
	{0.506617f, -0.569514f},
	{0.508307f, -0.753704f},
	{0.724054f, -0.689743f},
	{0.690701f, -0.500792f},
	{0.621962f, -0.334684f},
	{0.523603f, -0.197011f},
	{0.402082f, -0.092510f},
	{0.503473f, -0.004886f},
	{0.643665f, -0.093004f},
	{0.766763f, -0.218189f},
	{0.865428f, -0.377196f},
	{0.886471f, -0.053956f},
	{0.739284f, 0.054065f},
	{0.581749f, 0.122011f},
	{0.421872f, 0.149424f},
	{0.301882f, 0.249448f},
	{0.457132f, 0.284555f},
	{0.624881f, 0.281390f},
	{0.797553f, 0.237514f},
	{0.967186f, 0.151767f},
	{0.807515f, 0.447203f},
	{0.622814f, 0.463278f},
	{0.450418f, 0.439581f},
	{0.297278f, 0.380064f},
	{0.131620f, 0.405760f},
	{0.248079f, 0.516986f},
	{0.395414f, 0.602039f},
	{0.568461f, 0.654807f},
	{0.458040f, 0.841453f},
	{0.289416f, 0.757803f},
	{0.152117f, 0.646686f},
	{0.050236f, 0.515038f},
	{-0.115473f, 0.446860f},
	{-0.072279f, 0.604083f},
	{0.011016f, 0.755164f},
	{0.133363f, 0.892186f},
	{-0.067530f, 0.990708f},
	{-0.169640f, 0.828974f},
	{-0.230295f, 0.660292f},
	{-0.249830f, 0.492731f},
	{-0.230015f, 0.334066f},
	{-0.362822f, 0.338933f},
	{-0.407380f, 0.497945f},
	{-0.414354f, 0.672862f},
	{-0.380634f, 0.856274f},
	{-0.612366f, 0.633545f},
	{-0.576115f, 0.455234f},
	{-0.505354f, 0.299452f},
	{-0.405576f, 0.172048f},
	{-0.384114f, 0.019641f},
	{-0.522417f, 0.097668f},
	{-0.644195f, 0.212445f},
	{-0.742204f, 0.360620f},
	{-0.809812f, 0.537627f},
	{-0.890698f, 0.214167f},
	{-0.765219f, 0.078460f},
	{-0.622068f, -0.019637f},
	{-0.469039f, -0.078271f},
	{-0.365062f, -0.206826f},
	{-0.525650f, -0.211043f},
	{-0.691454f, -0.175127f},
	{-0.854531f, -0.098069f},
	{-0.899516f, -0.308812f},
	{-0.718989f, -0.360257f},
	{-0.543208f, -0.370169f},
	{-0.379756f, -0.341152f},
	{-0.213409f, -0.401333f},
	{-0.350636f, -0.488605f},
	{-0.513417f, -0.544320f},
	{-0.695552f, -0.563401f},
	{-0.615037f, -0.770683f},
	{-0.431352f, -0.719810f},
	{-0.273469f, -0.636007f},
	{-0.146724f, -0.525350f},
	{-0.055075f, -0.394778f},
};

const uint8_t gamma8[] = {
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
 25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
 37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
 51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
 69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
 90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
 115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
 144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
 177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
 215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

const uint8_t gamma82[] = {
0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,
0,   0,   0,   0,   0,   0,   0,   0,   0,   1,   1,   1,   1,   1,   1,
1,   1,   1,   1,   1,   1,   2,   2,   2,   2,   2,   2,   2,   2,   3,
3,   3,   3,   3,   3,   4,   4,   4,   4,   5,   5,   5,   5,   5,   6,
6,   6,   6,   7,   7,   7,   8,   8,   8,   9,   9,   9,   10,  10,  10,
11,  11,  11,  12,  12,  13,  13,  13,  14,  14,  15,  15,  16,  16,  17,
17,  18,  18,  19,  19,  20,  20,  21,  21,  22,  22,  23,  24,  24,  25,
25,  26,  27,  27,  28,  29,  29,  30,  31,  31,  32,  33,  34,  34,  35,
36,  37,  38,  38,  39,  40,  41,  42,  42,  43,  44,  45,  46,  47,  48,
49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,
64,  65,  66,  68,  69,  70,  71,  72,  73,  75,  76,  77,  78,  80,  81,
82,  84,  85,  86,  88,  89,  90,  92,  93,  94,  96,  97,  99,  100, 102,
103, 105, 106, 108, 109, 111, 112, 114, 115, 117, 119, 120, 122, 124, 125,
127, 129, 130, 132, 134, 136, 137, 139, 141, 143, 145, 146, 148, 150, 152,
154, 156, 158, 160, 162, 164, 166, 168, 170, 172, 174, 176, 178, 180, 182,
184, 186, 188, 191, 193, 195, 197, 199, 202, 204, 206, 209, 211, 213, 215,
218, 220, 223, 225, 227, 230, 232, 235, 237, 240, 242, 245, 247, 250, 252,
255};

// Translate from 4 bit nibbles from color channels
// into SPI neopixel bits
// 0 -> 100
// 1 -> 110
// Starting from MSB
const uint16_t neopixel_spi_encode_nibble_lut[] = {
	04444, // 0
	04446, // 1
	04464, // 2
	04466, // 3
	04644, // 4
	04646, // 5
	04664, // 6
	04666, // 7
	06444, // 8
	06446, // 9
	06464, // A
	06466, // B
	06644, // C
	06646, // D
	06664, // E
	06666, // F
};

uint32_t rgb(uint8_t r, uint8_t g, uint8_t b) {
	return ((uint32_t) (r) << 16) | ((uint32_t) (g) << 8) | (uint32_t) (b);
}

static uint32_t apply_gamma_to_led(uint32_t urgb) {
	uint8_t r = (urgb >> 16) & 0xff;
	uint8_t g = (urgb >> 8) & 0xff;
	uint8_t b = urgb & 0xff;
	return rgb(gamma8[r],gamma8[g], gamma8[b]);
}

void apply_gamma_to_leds(LEDBuffer buffer) {
	for (int i = 0; i < NUM_LEDS; i++) {
		buffer[i] = apply_gamma_to_led(buffer[i]);
	}
}

uint32_t hsvFToRgbFullSpectrum(HsvColorF hsv) {
	float h = hsv.h;
	float s = hsv.s;
	float v = hsv.v;

	float c = v * s;  // Chroma
	float h_prime = h / 60.0f;
	float x = c * (1.0f - fabsf(fmodf(h_prime, 2.0f) - 1.0f));
	float m = v - c;

	float r1, g1, b1;

	if (h_prime < 1.0f)      { r1 = c; g1 = x; b1 = 0; }
	else if (h_prime < 2.0f) { r1 = x; g1 = c; b1 = 0; }
	else if (h_prime < 3.0f) { r1 = 0; g1 = c; b1 = x; }
	else if (h_prime < 4.0f) { r1 = 0; g1 = x; b1 = c; }
	else if (h_prime < 5.0f) { r1 = x; g1 = 0; b1 = c; }
	else                     { r1 = c; g1 = 0; b1 = x; }

	uint8_t r = (uint8_t)((r1 + m) * 255.0f);
	uint8_t g = (uint8_t)((g1 + m) * 255.0f);
	uint8_t b = (uint8_t)((b1 + m) * 255.0f);

	return rgb(r, g, b);
}

uint32_t hsvFToRgbRainbow(HsvColorF hsv) {
	float h = hsv.h; // 0 to 360
	float s = hsv.s; // 0 to 1
	float v = hsv.v; // 0 to 1

	// Params from fastLED:
	
	// Yellow has a higher inherent brightness than any other color.
	// Level Y1 is a moderate boost, the default.
	// Level Y2 is a strong boost.
	const uint8_t Y1 = 0;
	const uint8_t Y2 = 1;
	
	// G2: Whether to divide all greens by two.
	// Depends GREATLY on your particular LEDs
	const uint8_t G2 = 0;
	
	// Gscale: what to scale green down by (0-255).
	// Depends GREATLY on your particular LEDs
	const uint8_t Gscale = 0;
	
	float hue_byte = (h / 360.0f) * 256.0f;
	if (hue_byte >= 256.0f) hue_byte = 0.0f;
	
	int hue_int = (int)hue_byte;
	
	// Get the position within the current 32-unit segment (0-31)
	float offset = (float)(hue_int & 0x1F);
	
	// Scale offset to 0-1 range within segment
	float offset_norm = offset / 32.0f;
	
	float third = offset_norm / 3.0f;
	float twothirds = offset_norm * 2.0f / 3.0f;
	
	float r, g, b;
	
	const float K255 = 1.0f;
	const float K171 = 171.0f / 255.0f;  // 2/3
	const float K170 = 170.0f / 255.0f;  // ~2/3
	const float K85 = 85.0f / 255.0f;     // 1/3
	
	// Determine which of 8 color bands we're in (using top 3 bits)
	int band = (hue_int >> 5) & 0x07;
	
	switch(band) {
		case 0: // Red - Orange (000)
			r = K255 - third;
			g = third;
			b = 0.0f;
			break;
			
		case 1: // Orange - Yellow (001)
			if (Y1) {
				// Y1: moderate boost
				r = K171;
				g = K85 + third;
				b = 0.0f;
			} else if (Y2) {
				// Y2: strong boost
				r = K170 + third;
				g = K85 + twothirds;
				b = 0.0f;
			} else {
				// No boost - linear continuation from case 0
				r = K255 - third;
				g = third;
				b = 0.0f;
			}
			break;
			
		case 2: // Yellow - Green (010)
			if (Y1) {
				// Y1: moderate boost
				r = K171 - twothirds;
				g = K170 + third;
				b = 0.0f;
			} else if (Y2) {
				// Y2: strong boost
				r = K255 - offset_norm;
				g = K255;
				b = 0.0f;
			} else {
				// No boost - linear continuation
				r = K255 - third;
				g = third;
				b = 0.0f;
			}
			break;
		case 3: // Green - Aqua (011)
			r = 0.0f;
			g = K255 - third;
			b = third;
			break;
		case 4: // Aqua - Blue (100)
			r = 0.0f;
			g = K171 - twothirds;
			b = K85 + twothirds;
			break;
		case 5: // Blue - Purple (101)
			r = third;
			g = 0.0f;
			b = K255 - third;
			break;
		case 6: // Purple - Pink (110)
			r = K85 + third;
			g = 0.0f;
			b = K171 - third;
			break;	
		case 7: // Pink - Red (111)
			r = K170 + third;
			g = 0.0f;
			b = K85 - third;
			break;
	}
	
	// Green adjustments
	if (G2) g = g * 0.5f;
	if (Gscale) g = g * (Gscale / 255.0f);
	
	// Apply saturation
	if (s < 1.0f) {
		if (s == 0.0f) {
			r = g = b = 1.0f;
		} else {
			float desat = 1.0f - s;
			desat = desat * desat; // FastLED's scale8
			
			float satscale = 1.0f - desat;
			
			r = r * satscale + desat;
			g = g * satscale + desat;
			b = b * satscale + desat;
		}
	}
	
	// Apply value
	if (v < 1.0f) {
		float val_scaled = v * v;
		r *= val_scaled;
		g *= val_scaled;
		b *= val_scaled;
		// r *= v;
		// g *= v;
		// b *= v;
	}
	
	uint8_t r8 = (uint8_t)(fminf(r * 255.0f + 0.5f, 255.0f));
	uint8_t g8 = (uint8_t)(fminf(g * 255.0f + 0.5f, 255.0f));
	uint8_t b8 = (uint8_t)(fminf(b * 255.0f + 0.5f, 255.0f));
	return rgb(r8, g8, b8);
}