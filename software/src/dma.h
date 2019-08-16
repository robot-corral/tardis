#pragma once

#include <stdint.h>

extern uint32_t g_data[3];

void configureDma();

void copyAudioDataToDmaBuffer();
