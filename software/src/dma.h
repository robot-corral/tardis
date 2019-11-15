#pragma once

#include <stdint.h>

extern uint32_t g_data[3];

void configureDma();

void configureSpi3Dma();
void shutdownSpi3Dma();

void copyAudioDataToDmaBuffer();
