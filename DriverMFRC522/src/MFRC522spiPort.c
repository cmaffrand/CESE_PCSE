/*=============================================================================
 * Authors: Carlos Maffrand 	<carlosmaffrand5@gmail.com>
 * Date: 2021/10/08
 *===========================================================================*/

#include "MFRC522spiPort.h"

void sysTickInitPort(void)
{
    //Contadores de ciclos
    uint32_t *H_DWT_DEMCR = (uint32_t *)0xE000EDFC;
    uint32_t *H_DWT_CTRL = (uint32_t *)0xE0001000;
    // bit24[TRCENA]   = habilita todos los DWT
    *H_DWT_DEMCR |= 1 << 24;
    // bit0[CYCCNTENA] =  enable CYCCNT
    *H_DWT_CTRL |= 1;
}

void spiInitPort(void)
{
    // Configure SSP SSP1 pins
    Chip_SCU_PinMuxSet(0xF, 4, (SCU_MODE_PULLUP | SCU_MODE_FUNC0));                                         // SSP1_SCK
    Chip_SCU_PinMuxSet(0x1, 3, (SCU_MODE_PULLUP | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS | SCU_MODE_FUNC5)); // SSP1_MISO
    Chip_SCU_PinMuxSet(0x1, 4, (SCU_MODE_PULLUP | SCU_MODE_FUNC5));                                         // SSP1_MOSI
    // Initialize SSP Peripheral
    Chip_SSP_Init(LPC_SSP1);
    Chip_SSP_Enable(LPC_SSP1);
}

void interruptInitPort(void)
{
    // Interrupt Pin Config | GPIO2 | Puerto 3 | Pin 4 | Utilizando INT0 |
    Chip_SCU_PinMux(3, 4, SCU_MODE_PULLDOWN | SCU_MODE_INBUFF_EN | SCU_MODE_ZIF_DIS, 0);
    Chip_GPIO_SetDir(LPC_GPIO_PORT, 3, (1 << 4), 0);
    Chip_SCU_GPIOIntPinSel(0, 3, 4);
    Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH(0)); // Borra el pending IRQ INT0
    Chip_PININT_SetPinModeEdge(LPC_GPIO_PIN_INT, PININTCH(0)); // activo por flanco
    Chip_PININT_EnableIntHigh(LPC_GPIO_PIN_INT, PININTCH(0));  // flanco descendente
}

void chipSelectInitPort(void)
{
    // Chip Select Pin Config | GPIO0 | Puerto 3 | Pin 0 |
    Chip_SCU_PinMuxSet(0x6, 1, (SCU_MODE_PULLUP | SCU_MODE_FUNC0)); // Pin for SPI SS configured as GPIO output with pull-up
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 3, 0);
}

void resetInitPort(void)
{
    // Reset Pin Config | GPIO1 | Puerto 3 | Pin 3 |
    Chip_GPIO_SetPinDIROutput(LPC_GPIO_PORT, 3, 3);
}

void spiReadPort(uint8_t *buffer, uint32_t bufferSize, uint8_t *bufferAdd)
{
    // Funcion de lectura de SPI
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
    // Funcion de escritura de SPI
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
    // Funcion para habilitar y desabilitar el CS del SPI
    if (chipSelect)
        Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 3, 0);
    else
        Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, 3, 0);
}

void resetPort(bool resteSelect)
{
    // Funcion para hacer el hard reset del MFRC522
    if (resteSelect)
        Chip_GPIO_SetPinOutHigh(LPC_GPIO_PORT, 3, 3);
    else
        Chip_GPIO_SetPinOutLow(LPC_GPIO_PORT, 3, 3);
}

void GPIO0_IRQHandler(void)
{
    // Manejo de interrupciones
    int8_t buffer[2];
    buffer[0] = (0x04 << 1) & 0x7E; // ComIrqReg
    buffer[1] = 0x80;
    chipSelectPort(TRUE);
    spiWritePort(buffer, sizeof(buffer) / sizeof(buffer[0]));
    chipSelectPort(FALSE);
    buffer[0] = (0x05 << 1) & 0x7E; // DivIrqReg
    chipSelectPort(TRUE);
    spiWritePort(buffer, sizeof(buffer) / sizeof(buffer[0]));
    chipSelectPort(FALSE);
    Chip_PININT_ClearIntStatus(LPC_GPIO_PIN_INT, PININTCH0); //borra flag}
}

void enableIRQPort(void)
{
    // habilita interrupciones
    NVIC_EnableIRQ(PIN_INT0_IRQn);
}
