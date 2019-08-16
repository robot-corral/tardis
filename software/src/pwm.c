#include "pwm.h"

#include "stm32/stm32l4xx_ll_bus.h"
#include "stm32/stm32l4xx_ll_rcc.h"
#include "stm32/stm32l4xx_ll_tim.h"
#include "stm32/stm32l4xx_ll_gpio.h"

#include "dma.h"

void configurePwm()
{
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);

    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_ALTERNATE);
    LL_GPIO_SetPinPull(GPIOB, LL_GPIO_PIN_6, LL_GPIO_PULL_DOWN);
    LL_GPIO_SetPinSpeed(GPIOB, LL_GPIO_PIN_6, LL_GPIO_SPEED_FREQ_HIGH);
    LL_GPIO_SetAFPin_0_7(GPIOB, LL_GPIO_PIN_6, LL_GPIO_AF_2);

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
    LL_TIM_SetCounterMode(TIM4, LL_TIM_COUNTERMODE_UP);

    LL_TIM_SetAutoReload(TIM4, __LL_TIM_CALC_ARR(SystemCoreClock, LL_TIM_COUNTERMODE_UP, 1250));

    LL_TIM_OC_SetMode(TIM4, LL_TIM_CHANNEL_CH1, LL_TIM_OCMODE_PWM1);
    LL_TIM_OC_ConfigOutput(TIM4, LL_TIM_CHANNEL_CH1, LL_TIM_OCPOLARITY_HIGH | LL_TIM_OCIDLESTATE_HIGH);

    LL_TIM_OC_SetCompareCH1(TIM4, 1);

    LL_TIM_OC_EnablePreload(TIM4, LL_TIM_CHANNEL_CH1);

    LL_TIM_EnableDMAReq_UPDATE(TIM4);

    LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH1);
    LL_TIM_EnableAllOutputs(TIM4);
    LL_TIM_EnableCounter(TIM4);
}
