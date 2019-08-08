#pragma once

#include <stdint.h>

#define WAVEFORM_FREQUENCY ((uint32_t) 22050)
#define WAVEFORM_SAMPLES_SIZE ((uint32_t) 247125)

extern const uint16_t g_waveformSine_12bits[WAVEFORM_SAMPLES_SIZE];
