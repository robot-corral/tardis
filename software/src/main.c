#include "dac.h"
#include "dma.h"
#include "pwm.h"
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

    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOC);

    configureDma();
    configureDacTimer();
    configureDac();
    configureSpi();
    configurePwm();
    activateDac();

    for (;;);
}
