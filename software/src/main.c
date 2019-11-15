#include "dac.h"
#include "dma.h"
#include "pwm.h"
#include "spi.h"
#include "system.h"
#include "lsm6dsox.h"

void main()
{
    configureSystemClock();

    configureImu();

    configureDma();
    configureDacTimer();
    configureDac();
    configureSpi();
    configurePwm();

    // activateDac();

    for (;;);
}
