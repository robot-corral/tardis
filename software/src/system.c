#include "system.h"

#include "stm32/stm32l4xx_ll_rcc.h"
#include "stm32/stm32l4xx_ll_utils.h"
#include "stm32/stm32l4xx_ll_system.h"

void configureSystemClock(void)
{
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_4);
    LL_RCC_MSI_Enable();

    while (LL_RCC_MSI_IsReady() != 1);

    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_MSI, LL_RCC_PLLM_DIV_1, 40, LL_RCC_PLLR_DIV_2);
    LL_RCC_PLL_Enable();
    LL_RCC_PLL_EnableDomain_SYS();

    while (LL_RCC_PLL_IsReady() != 1);

    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL);

    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    LL_Init1msTick(80000000);
    LL_SetSystemCoreClock(80000000);
}
