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

#define PORT 6
#define PIN 11
#define MODE 0
#define FUNC 0
#define GPIO_OUTPUT 1

/*=====[Public function-like macros]=========================================*/

void spiInitPort(void);
void spiReadPort(uint8_t *buffer, uint32_t bufferSize, uint8_t *bufferAdd);
void spiWritePort(uint8_t *buffer, uint32_t bufferSize);
void chipSelectPort(bool chipSelect);
void resetPort(bool resteSelect);

#endif /* __PORT_H__ */