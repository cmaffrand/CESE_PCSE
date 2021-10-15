/*=============================================================================
 * Authors: Carlos Maffrand 	<carlosmaffrand5@gmail.com>
 * Date: 2021/10/08
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __PORT_H__
#define __PORT_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include <stdint.h>
#include <stddef.h>
#include "chip.h"

/*=====[Definition macros of public constants]===============================*/

#ifndef DWT_CYCCNT
    #define DWT_CYCCNT (*((volatile uint32_t *)0xE0001004))
#endif
#define DWT_CONTROL (*((volatile uint32_t *)0xE0001000))
#define DWT_CYCCNTENA_BIT (1UL << 0)

#define EnableCycleCounter() DWT_CONTROL |= DWT_CYCCNTENA_BIT
#define GetCycleCounter() DWT_CYCCNT
#define ResetCycleCounter() DWT_CYCCNT = 0
#define DisableCycleCounter() DWT_CONTROL &= ~DWT_CYCCNTENA_BIT

/*=====[Public function-like macros]=========================================*/

void sysTickInitPort(void);
void interruptInitPort(void);
void chipSelectInitPort(void);
void resetInitPort(void);
void spiInitPort(void);
void spiReadPort(uint8_t *buffer, uint32_t bufferSize, uint8_t *bufferAdd);
void spiWritePort(uint8_t *buffer, uint32_t bufferSize);
void chipSelectPort(bool chipSelect);
void resetPort(bool resteSelect);
void enableIRQPort(void);

#endif /* __PORT_H__ */
