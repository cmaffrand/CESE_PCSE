/*=============================================================================
 * Authors: Carlos Maffrand 	<carlosmaffrand5@gmail.com>
 * Date: 2021/10/08
 *===========================================================================*/

#include "MFRC522spiPort.h"

void spiInitPort(void)
{
    // Interrupt Pin Config
    Chip_SCU_PinMux(3, 4, SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS, 0);
    Chip_GPIO_SetDir(LPC_GPIO_PORT, 3, (1 << 4), 0);
    Chip_SCU_GPIOIntPinSel(0, 3, 4);                           // (GPIO2 PortNum=3 y PinNum=4) a INT0
    Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0)); // Borra el pending IRQ INT0
    Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(0)); // activo por flanco
    Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(0));  // flanco descendente
    // Chip select Pin Config
    Chip_SCU_PinMuxSet(0x6, 1, (SCU_MODE_PULLUP | SCU_MODE_FUNC0)); // Pin for SPI SS configured as GPIO output with pull-up
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 3, 0);
    // Reset Pin Config
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 3, 3);
    // Configure SSP SSP1 pins
    Chip_SCU_PinMuxSet(0xF, 4, (SCU_MODE_PULLUP | SCU_MODE_FUNC0));                                         // SSP1_SCK
    Chip_SCU_PinMuxSet(0x1, 3, (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC5)); // SSP1_MISO
    Chip_SCU_PinMuxSet(0x1, 4, (SCU_MODE_PULLUP | SCU_MODE_FUNC5));                                         // SSP1_MOSI
    // Initialize SSP Peripheral
    Chip_SSP_Init(LPC_SSP1);
    Chip_SSP_Enable(LPC_SSP1);
}

void spiReadPort(uint8_t *buffer, uint32_t bufferSize, uint8_t *bufferAdd)
{
    Chip_SSP_DATA_SETUP_T xferConfig;

    xferConfig.tx_data = bufferAdd;
    xferConfig.tx_cnt = 0;
    xferConfig.rx_data = buffer;
    xferConfig.rx_cnt = 0;
    xferConfig.length = bufferSize;
    Chip_SSP_RWFrames_Blocking(LPC_SSP1, &xferConfig);
    while (Chip_SSP_GetStatus(LPC_SSP1, SSP_STAT_BSY))
        ;
}

void spiWritePort(uint8_t *buffer, uint32_t bufferSize)
{
    Chip_SSP_DATA_SETUP_T xferConfig;

    xferConfig.tx_data = buffer;
    xferConfig.tx_cnt = 0;
    xferConfig.rx_data = NULL;
    xferConfig.rx_cnt = 0;
    xferConfig.length = bufferSize;
    Chip_SSP_RWFrames_Blocking(LPC_SSP1, &xferConfig);
    while (Chip_SSP_GetStatus(LPC_SSP1, SSP_STAT_BSY))
        ;
}

void chipSelectPort(bool chipSelect)
{

    if (chipSelect)
        Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 3, 0);
    else
        Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, 3, 0);
}

void resetPort(bool resteSelect)
{

    if (resteSelect)
        Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, 3, 3);
    else
        Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 3, 3);
}

void GPIO0_IRQHandler(void)
{
    if (Chip_PININT_GetFallStates(LPC_GPIO_PIN_INT) & PININTCH0) // Interrupcion correcta
    {
        Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH0); //borra flag
    }
}

void enableIRQPort(void)
{
    NVIC_EnableIRQ(PIN_INT0_IRQn);
}
