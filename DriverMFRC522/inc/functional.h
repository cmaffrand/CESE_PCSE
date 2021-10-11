/*=============================================================================
 * Author: Carlos Maffrand <carlosmaffrand5@gmail.com>
 * Date: 2021/10/08
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __FUNCTIONAL_H__
#define __FUNCTIONAL_H__

#include "sapi.h" // <= Biblioteca sAPI
#include "MFRC522.h"
#include "MFRC522spiPort.h"
#include <stdio.h>
#include <string.h>

/*=====[Public function-like macros]=========================================*/

void lockDoor(void);
void ledInit(void);

#endif /* __FUNCTIONAL_H__ */
