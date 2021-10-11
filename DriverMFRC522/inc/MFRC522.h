/*=============================================================================
 * Authors: Carlos Maffrand 	<carlosmaffrand5@gmail.com>
 * Date: 2021/10/08
 *===========================================================================*/

/*=====[Avoid multiple inclusion - begin]====================================*/

#ifndef __MFRC522_H__
#define __MFRC522_H__

/*=====[Inclusions of public function dependencies]==========================*/

#include <stdint.h>
#include <stddef.h>

/*=====[Definition macros of public constants]===============================*/

#define MAX_LEN 16 // Maximum length of an array.

// MF522 MFRC522 error codes
#define MI_OK 0       // Everything A-OK.
#define MI_NOTAGERR 1 // No tag error
#define MI_ERR 2      // General error

// MF522 Command word
#define MFRC522_IDLE 0x00        // NO action; Cancel the current command
#define MFRC522_MEM 0x01         // Store 25 byte into the internal buffer.
#define MFRC522_GENRANDOMID 0x02 // Generates a 10 byte random ID number.
#define MFRC522_CALCCRC 0x03     // CRC Calculate or selftest.
#define MFRC522_TRANSMIT 0x04    // Transmit data
#define MFRC522_NOCMDCH 0x07     // No command change.
#define MFRC522_RECEIVE 0x08     // Receive Data
#define MFRC522_TRANSCEIVE 0x0C  // Transmit and receive data,
#define MFRC522_AUTHENT 0x0E     // Authentication Key
#define MFRC522_SOFTRESET 0x0F   // Reset

// Mifare_One tag command word
#define MF1_REQIDL 0x26    // find the antenna area does not enter hibernation
#define MF1_REQALL 0x52    // find all the tags antenna area
#define MF1_ANTICOLL 0x93  // anti-collision
#define MF1_SELECTTAG 0x93 // election tag
#define MF1_AUTHENT1A 0x60 // authentication key A
#define MF1_AUTHENT1B 0x61 // authentication key B
#define MF1_READ 0x30      // Read Block
#define MF1_WRITE 0xA0     // write block
#define MF1_DECREMENT 0xC0 // debit
#define MF1_INCREMENT 0xC1 // recharge
#define MF1_RESTORE 0xC2   // transfer block data to the buffer
#define MF1_TRANSFER 0xB0  // save the data in the buffer
#define MF1_HALT 0x50      // Sleep

// MFRC522 registers
//Page 0:Command and Status
#define Reserved00 0x00
#define CommandReg 0x01
#define ComlEnReg 0x02
#define DivlEnReg 0x03
#define ComIrqReg 0x04
#define DivIrqReg 0x05
#define ErrorReg 0x06
#define Status1Reg 0x07
#define Status2Reg 0x08
#define FIFODataReg 0x09
#define FIFOLevelReg 0x0A
#define WaterLevelReg 0x0B
#define ControlReg 0x0C
#define BitFramingReg 0x0D
#define CollReg 0x0E
#define Reserved0F 0x0F
//Page 1:Command
#define Reserved10 0x10
#define ModeReg 0x11
#define TxModeReg 0x12
#define RxModeReg 0x13
#define TxControlReg 0x14
#define TxASKReg 0x15
#define TxSelReg 0x16
#define RxSelReg 0x17
#define RxThresholdReg 0x18
#define DemodReg 0x19
#define Reserved1A 0x1A
#define Reserved1B 0x1B
#define MfTxReg 0x1C
#define MfRxReg 0x1D
#define Reserved1E 0x1E
#define SerialSpeedReg 0x1F
//Page 2:CFG
#define Reserved20 0x20
#define CRCResultRegHi 0x21
#define CRCResultRegLo 0x22
#define Reserved23 0x23
#define ModWidthReg 0x24
#define Reserved25 0x25
#define RFCfgReg 0x26
#define GsNReg 0x27
#define CWGsPReg 0x28
#define ModGsPReg 0x29
#define TModeReg 0x2A
#define TPrescalerReg 0x2B
#define TReloadRegHi 0x2C
#define TReloadRegLo 0x2D
#define TCounterValueRegHi 0x2E
#define TCounterValueRegLo 0x2F
//Page 3:TestRegister
#define Reserved30 0x30
#define TestSel1Reg 0x31
#define TestSel2Reg 0x32
#define TestPinEnReg 0x33
#define TestPinValueReg 0x34
#define TestBusReg 0x35
#define AutoTestReg 0x36
#define VersionReg 0x37
#define AnalogTestReg 0x38
#define TestDAC1Reg 0x39
#define TestDAC2Reg 0x3A
#define TestADCReg 0x3B
#define Reserved3C 0x3C
#define Reserved3D 0x3D
#define Reserved3E 0x3E
#define Reserved3F 0x3F

/*=====[Public function-like macros]=========================================*/

void initMFRC522(void);
void beginMFRC522(void);
void writeRegisterMFRC522(uint8_t address, uint8_t data);
uint8_t readRegisterMFRC522(uint8_t address);
void setBitsMFRC522(uint8_t address, uint8_t mask);
void clearBitsMFRC522(uint8_t address, uint8_t mask);
void resetMFRC522(void);
void delayMFRC522(uint64_t duration_ms);
uint8_t firmwareMFRC522(void);
uint8_t digitalSelfTestPass(uint8_t *dump);
uint8_t sendCommandTag(uint8_t cmd, uint8_t *data, uint32_t dlen, uint8_t *result, uint32_t *rlen);
uint8_t requestTag(uint8_t mode, uint8_t *data);
uint8_t antiCollision(uint8_t *serial);
void clearAllIRQMFRC522(void);
void enableAllIRQMFRC522(void);

#endif /* __MFRC522_H__ */
