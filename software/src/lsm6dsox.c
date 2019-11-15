#include "lsm6dsox.h"

#include "dma.h"
#include "spi.h"

#define INT1_DRDY_XL 1

#define INT2_NONE 0

#define WHO_AM_I_NONE 0

#define CTRL1_XL_LPF2_XL_DISABLE 0
#define CTRL1_XL_FS_XL_2G        0
#define CTRL1_XL_ODR_XL_26_HZ    0b00100000

#define CTRL2_G_NONE 0

#define CTRL3_C_BOOT_NORMAL           0
#define CTRL3_C_BDU_CONTINUOUS        0
#define CTRL3_C_INTERRUPT_ACTIVE_HIGH 0
#define CTRL3_C_INTERRUPT_PUSH_PULL   0
#define CTRL3_C_SPI_4_WIRE            0
#define CTRL3_C_IF_INCREMENT          0b00000100
#define CTRL3_C_SOFTWARE_RESET_NORMAL 0;

void configureImu()
{
    configureSpi3Dma();
    configureSpi3();
    configureLsm6dsox();
    shutdownSpi3();
    shutdownSpi3Dma();
}

void configureLsm6dsox()
{
    g_imuTxBuffer[0]  = 0b00001101;  // INT1_CTRL (0Dh): address
    g_imuTxBuffer[1]  = INT1_DRDY_XL;
    g_imuTxBuffer[2]  = INT2_NONE;
    g_imuTxBuffer[3]  = WHO_AM_I_NONE;
    g_imuTxBuffer[4]  = CTRL1_XL_LPF2_XL_DISABLE | CTRL1_XL_FS_XL_2G | CTRL1_XL_ODR_XL_26_HZ;
    g_imuTxBuffer[5]  = CTRL2_G_NONE;
    g_imuTxBuffer[6]  = CTRL3_C_BOOT_NORMAL |
                        CTRL3_C_BDU_CONTINUOUS |
                        CTRL3_C_INTERRUPT_ACTIVE_HIGH |
                        CTRL3_C_INTERRUPT_PUSH_PULL |
                        CTRL3_C_SPI_4_WIRE |
                        CTRL3_C_IF_INCREMENT |
                        CTRL3_C_SOFTWARE_RESET_NORMAL;
    g_imuTxBuffer[7]  = 
    g_imuTxBuffer[8]  = 
    g_imuTxBuffer[9]  = 
    g_imuTxBuffer[10] = 
}
