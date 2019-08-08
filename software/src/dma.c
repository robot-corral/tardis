#include "dma.h"

#include "stm32/stm32l4xx_ll_bus.h"
#include "stm32/stm32l4xx_ll_dac.h"
#include "stm32/stm32l4xx_ll_dma.h"

#include "tardis_sound.h"

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

    LL_DMA_ConfigAddresses(DMA1,
                           LL_DMA_CHANNEL_3,
                           (uint32_t) &g_waveformSine_12bits,
                           LL_DAC_DMA_GetRegAddr(DAC1, LL_DAC_CHANNEL_1, LL_DAC_DMA_REG_DATA_12BITS_RIGHT_ALIGNED),
                           LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_3, WAVEFORM_SAMPLES_SIZE);

    LL_DMA_EnableIT_TE(DMA1, LL_DMA_CHANNEL_3);

    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_3);
}
