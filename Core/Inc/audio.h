#pragma once

#include <stdint.h>

// Number of 24-bit samples in 32-bit frames in both channels
// So the number of samples in the left channel is half this
// The array is 16 bit samples, so double this
#define AUDIO_BUFFER_SIZE 2048

#define AUDIO_SAMPLES_HALF (AUDIO_BUFFER_SIZE / 4)
#define FFT_SIZE AUDIO_SAMPLES_HALF

#define NUM_FFT_BANDS 8

// Populated by DMA: [2 byte lo L] [2 byte hi L] [4 byte empty R channel]
extern volatile uint16_t rx_buffer[AUDIO_BUFFER_SIZE * 2];
// extern volatile uint8_t audio_ready;

// Half buffers that are copied by DMA callbacks
extern int32_t audio_buffer[2][AUDIO_SAMPLES_HALF];

// 0xff written by consumer to reset
// 0 or 1 written by producer
extern volatile uint8_t audio_buffer_ready;

extern float fft_output[FFT_SIZE / 2];
extern float band_magnitudes[NUM_FFT_BANDS];
extern float band_smooth_fast[NUM_FFT_BANDS];
extern float band_smooth_slow[NUM_FFT_BANDS];
extern float band_transients[NUM_FFT_BANDS];

void write_half_audio_buffer(volatile uint16_t *rx_buffer_start, int buffer_idx);

void init_audio();
void process_audio();
