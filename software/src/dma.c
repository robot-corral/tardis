#include "dma.h"

#include "stm32/stm32l4xx_ll_bus.h"
#include "stm32/stm32l4xx_ll_dac.h"
#include "stm32/stm32l4xx_ll_dma.h"

#include <string.h>

#include "dac.h"
#include "errors.h"
#include "tardis_sound.h"

#define DMA_SOUND_BUFFER_SIZE               40000
#define DMA_SOUND_BUFFER_HALF_SIZE          20000
#define DMA_SOUND_BUFFER_1ST_HALF_START_IDX 0
#define DMA_SOUND_BUFFER_2ND_HALF_START_IDX DMA_SOUND_BUFFER_HALF_SIZE

static uint32_t g_nextWaveformStartIndex = 0;

static uint16_t g_dmaSoundBuffer[DMA_SOUND_BUFFER_SIZE];

void configureDma()
{
    NVIC_SetPriority(DMA1_Channel3_IRQn, 1);
    NVIC_EnableIRQ(DMA1_Channel3_IRQn);

    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

    LL_DMA_ConfigTransfer(DMA1,
                          LL_DMA_CHANNEL_3,
                          LL_DMA_DIRECTION_MEMORY_TO_PERIPH |
                          LL_DMA_MODE_CIRCULAR |
                          LL_DMA_PERIPH_NOINCREMENT |
                          LL_DMA_MEMORY_INCREMENT |
                          LL_DMA_PDATAALIGN_HALFWORD |
                          LL_DMA_MDATAALIGN_HALFWORD |
                          LL_DMA_PRIORITY_HIGH);

    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_3, LL_DMA_REQUEST_6);

    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_3);
    LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_3);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_3);
}

void copyAudioDataToDmaBuffer()
{
    memcpy(&g_dmaSoundBuffer[DMA_SOUND_BUFFER_1ST_HALF_START_IDX], g_waveformSine_12bits, sizeof(uint16_t) * DMA_SOUND_BUFFER_SIZE);

    LL_DMA_ConfigAddresses(DMA1,
                           LL_DMA_CHANNEL_3,
                           (uint32_t) &g_dmaSoundBuffer[DMA_SOUND_BUFFER_1ST_HALF_START_IDX],
                           LL_DAC_DMA_GetRegAddr(DAC1, LL_DAC_CHANNEL_1, LL_DAC_DMA_REG_DATA_12BITS_RIGHT_ALIGNED),
                           LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, DMA_SOUND_BUFFER_SIZE);

    g_nextWaveformStartIndex = DMA_SOUND_BUFFER_SIZE;

    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
}

void transferComplete(uint32_t startAddress)
{
    if (g_nextWaveformStartIndex >= WAVEFORM_SAMPLES_SIZE)
    {
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_3);
        stopAudioPlayback();
    }
    else
    {
        const uint32_t samplesToCopy = g_nextWaveformStartIndex + DMA_SOUND_BUFFER_HALF_SIZE > WAVEFORM_SAMPLES_SIZE ?
                                       WAVEFORM_SAMPLES_SIZE - g_nextWaveformStartIndex :
                                       DMA_SOUND_BUFFER_HALF_SIZE;

        memcpy(&g_dmaSoundBuffer[startAddress], &g_waveformSine_12bits[g_nextWaveformStartIndex], sizeof(uint16_t) * samplesToCopy);

        if (samplesToCopy != DMA_SOUND_BUFFER_HALF_SIZE)
        {
            memset(&g_dmaSoundBuffer[startAddress + samplesToCopy], 0, sizeof(uint16_t) * (DMA_SOUND_BUFFER_HALF_SIZE - samplesToCopy));
        }

        g_nextWaveformStartIndex += samplesToCopy;
    }
}

void DMA1_Channel3_IRQHandler()
{
    if (LL_DMA_IsActiveFlag_TC3(DMA1) == 1)
    {
        LL_DMA_ClearFlag_TC3(DMA1);
        transferComplete(DMA_SOUND_BUFFER_2ND_HALF_START_IDX);
    }
    if (LL_DMA_IsActiveFlag_HT3(DMA1) == 1)
    {
        LL_DMA_ClearFlag_HT3(DMA1);
        transferComplete(DMA_SOUND_BUFFER_1ST_HALF_START_IDX);
    }
    if (LL_DMA_IsActiveFlag_TE1(DMA1) == 1)
    {
        catastrophicError();
    }
}
