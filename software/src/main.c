#include "dac.h"
#include "dma.h"
#include "spi.h"
#include "system.h"

#include "stm32/stm32l4xx_ll_bus.h"
#include "stm32/stm32l4xx_ll_dac.h"
#include "stm32/stm32l4xx_ll_rcc.h"
#include "stm32/stm32l4xx_ll_tim.h"
#include "stm32/stm32l4xx_ll_gpio.h"

void main()
{
    configureSystemClock();
    configureDma();
    configureDacTimer();
    configureDac();
    configureSpi();
    activateDac();

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

    LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_ResetOutputPin(GPIOA, LL_GPIO_PIN_1);

    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_1, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_1);

    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_6, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);

    LL_GPIO_SetPinMode(GPIOB, LL_GPIO_PIN_9, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_9);

    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_7, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_ResetOutputPin(GPIOC, LL_GPIO_PIN_7);

    // audio off
    LL_GPIO_SetPinMode(GPIOC, LL_GPIO_PIN_5, LL_GPIO_MODE_OUTPUT);
    LL_GPIO_SetOutputPin(GPIOC, LL_GPIO_PIN_5);

    for (;;);
}
