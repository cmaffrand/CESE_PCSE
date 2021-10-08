/*=============================================================================
 * Authors: Carlos Maffrand 	<carlosmaffrand5@gmail.com>
 * Date: 2021/10/08
 *===========================================================================*/

#include "MFRC522.h"
#include "MFRC522spiPort.h"

void initMFRC522( void )
{
  spiInitPort();
  resetPort(TRUE);
  chipSelectPort(FALSE);
  // Reset
  resetMFRC522();
  // Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
  // f(Timer) = 6.78MHz/TPreScaler
  // TPreScaler = TModeReg[3..0] TPrescalerReg
  writeRegisterMFRC522(TModeReg, 0x8D);       // | Tauto = 1 | TGated = 0  | TAutoRestart = 1 | TPreScalerHi = 5 |
  writeRegisterMFRC522(TPrescalerReg, 0x3E);  // | TPrescalerReg = 0x3E |
  writeRegisterMFRC522(TReloadRegLo, 0x1E);   // | TReloadReg_Lo = 0x1E |
  writeRegisterMFRC522(TReloadRegHi, 0x00);   // | TReloadReg_Hi = 0x00 |
  writeRegisterMFRC522(TxASKReg, 0x40);       // | Force100ASK = 1 |
  writeRegisterMFRC522(ModeReg, 0x3D);        // | MSBFirst = 0 | TxWaitRF = 1 | PolMFin = 1 | CRCPreset = 01 |
  setBitsMFRC522(TxControlReg, 0x03);         // | Tx2RFEn = 1 | Tx1RFEn = 1 |
}

void writeRegisterMFRC522 (uint8_t address, uint8_t data)
{
  uint8_t buffer[2];

  buffer[0] = (address<<1) & 0x7E;
  buffer[1] = data;
  
  chipSelectPort(TRUE);
  spiWritePort(buffer, sizeof(buffer)/sizeof(buffer[0]));
  chipSelectPort(FALSE);
}

uint8_t readRegisterMFRC522 (uint8_t address)
{
  uint8_t buffer[1];
  uint8_t bufferAdd[1];

  buffer[0] = ((address<<1) & 0x7E) | 0x80;
  bufferAdd[0] = 0;
  
  chipSelectPort(TRUE);
  spiWritePort(buffer, sizeof(buffer)/sizeof(buffer[0]));
  spiReadPort(buffer, sizeof(buffer)/sizeof(buffer[0]),bufferAdd);
  chipSelectPort(FALSE);

  return buffer[0];
}

void setBitsMFRC522(uint8_t address, uint8_t mask)
{
  uint8_t reg;
  reg = readRegisterMFRC522(address);
  reg = mask | reg;
  writeRegisterMFRC522(address,reg);
}

void clearBitsMFRC522(uint8_t address, uint8_t mask)
{
  uint8_t reg;
  reg = readRegisterMFRC522(address);
  reg = ~mask & reg;
  writeRegisterMFRC522(address,reg);
}

void resetMFRC522(void)
{
  writeRegisterMFRC522(CommandReg,MFRC522_SOFTRESET);
}

uint8_t firmwareMFRC522 (void)
{
  uint8_t version;
  version = readRegisterMFRC522(VersionReg);
  return version;
}



