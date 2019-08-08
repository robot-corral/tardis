#include "dac.h"

#include "stm32/stm32l4xx_ll_bus.h"
#include "stm32/stm32l4xx_ll_dac.h"
#include "stm32/stm32l4xx_ll_rcc.h"
#include "stm32/stm32l4xx_ll_tim.h"
#include "stm32/stm32l4xx_ll_gpio.h"

#include "tardis_sound.h"

#define WAVEFORM_TIMER_FREQUENCY_RANGE_MIN ((uint32_t) 0x1)
#define WAVEFORM_TIMER_PRESCALER_MAX_VALUE ((uint32_t) 0xFFFF - 1)

void configureDacTimer()
{
    uint32_t timer_reload = 0;
    uint32_t timer_prescaler = 0;
    uint32_t timer_clock_frequency = 0;

    if (LL_RCC_GetAPB1Prescaler() == LL_RCC_APB1_DIV_1)
    {
        timer_clock_frequency = __LL_RCC_CALC_PCLK1_FREQ(SystemCoreClock, LL_RCC_GetAPB1Prescaler());
    }
    else
    {
        timer_clock_frequency = (__LL_RCC_CALC_PCLK1_FREQ(SystemCoreClock, LL_RCC_GetAPB1Prescaler()) * 2);
    }

    timer_prescaler = ((timer_clock_frequency / (WAVEFORM_TIMER_PRESCALER_MAX_VALUE * WAVEFORM_TIMER_FREQUENCY_RANGE_MIN)) + 1);
    timer_reload = (timer_clock_frequency / (timer_prescaler * WAVEFORM_FREQUENCY));

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM6);

    LL_TIM_SetPrescaler(TIM6, (timer_prescaler - 1));
    LL_TIM_SetAutoReload(TIM6, (timer_reload - 1));
    LL_TIM_SetCounterMode(TIM6, LL_TIM_COUNTERMODE_UP);
    // LL_TIM_SetRepetitionCounter(TIM6, 0);
    LL_TIM_SetTriggerOutput(TIM6, LL_TIM_TRGO_UPDATE);
    LL_TIM_EnableCounter(TIM6);
}

void configureDac()
{
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);

    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_4, LL_GPIO_MODE_ANALOG);

    NVIC_SetPriority(TIM6_DAC_IRQn, 0);
    NVIC_EnableIRQ(TIM6_DAC_IRQn);

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_DAC1);

    LL_DAC_SetTriggerSource(DAC1, LL_DAC_CHANNEL_1, LL_DAC_TRIG_EXT_TIM6_TRGO);
    LL_DAC_ConfigOutput(DAC1, LL_DAC_CHANNEL_1, LL_DAC_OUTPUT_MODE_NORMAL, LL_DAC_OUTPUT_BUFFER_ENABLE, LL_DAC_OUTPUT_CONNECT_GPIO);
    LL_DAC_EnableDMAReq(DAC1, LL_DAC_CHANNEL_1);
    LL_DAC_EnableIT_DMAUDR1(DAC1);
}

void activateDac()
{
    LL_DAC_Enable(DAC1, LL_DAC_CHANNEL_1);

    __IO uint32_t wait_loop_index = ((LL_DAC_DELAY_STARTUP_VOLTAGE_SETTLING_US * (SystemCoreClock / (100000 * 2))) / 10);

    while (wait_loop_index != 0)
    {
        wait_loop_index--;
    }

    LL_DAC_EnableTrigger(DAC1, LL_DAC_CHANNEL_1);
    LL_DAC_TrigSWConversion(DAC1, LL_DAC_CHANNEL_1);
}
