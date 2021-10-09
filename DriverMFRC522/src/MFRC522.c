/*=============================================================================
 * Authors: Carlos Maffrand 	<carlosmaffrand5@gmail.com>
 * Date: 2021/10/08
 *===========================================================================*/

#include "MFRC522.h"
#include "MFRC522spiPort.h"
#include "sapi.h"

void initMFRC522(void)
{
  spiInitPort();
  resetPort(TRUE);
  chipSelectPort(FALSE);
}

void beginMFRC522(void)
{
  // Reset
  resetMFRC522();
  // Timer: TPrescaler*TreloadVal/6.78MHz = 24ms
  // f(Timer) = 6.78MHz/TPreScaler
  // TPreScaler = TModeReg[3..0] TPrescalerReg
  writeRegisterMFRC522(TModeReg, 0x8D);      // | Tauto = 1 | TGated = 0  | TAutoRestart = 1 | TPreScalerHi = 5 |
  writeRegisterMFRC522(TPrescalerReg, 0x3E); // | TPrescalerReg = 0x3E |
  writeRegisterMFRC522(TReloadRegLo, 0x1E);  // | TReloadReg_Lo = 0x1E |
  writeRegisterMFRC522(TReloadRegHi, 0x00);  // | TReloadReg_Hi = 0x00 |
  writeRegisterMFRC522(TxASKReg, 0x40);      // | Force100ASK = 1 |
  writeRegisterMFRC522(ModeReg, 0x3D);       // | MSBFirst = 0 | TxWaitRF = 1 | PolMFin = 1 | CRCPreset = 01 |
  setBitsMFRC522(TxControlReg, 0x03);        // | Tx2RFEn = 1 | Tx1RFEn = 1 |
}

void writeRegisterMFRC522(uint8_t address, uint8_t data)
{
  uint8_t buffer[2];

  buffer[0] = (address << 1) & 0x7E;
  buffer[1] = data;

  chipSelectPort(TRUE);
  spiWritePort(buffer, sizeof(buffer) / sizeof(buffer[0]));
  chipSelectPort(FALSE);
}

uint8_t readRegisterMFRC522(uint8_t address)
{
  uint8_t buffer[1];
  uint8_t bufferAdd[1];

  buffer[0] = ((address << 1) & 0x7E) | 0x80;
  bufferAdd[0] = 0;

  chipSelectPort(TRUE);
  spiWritePort(buffer, sizeof(buffer) / sizeof(buffer[0]));
  spiReadPort(buffer, sizeof(buffer) / sizeof(buffer[0]), bufferAdd);
  chipSelectPort(FALSE);

  return buffer[0];
}

void setBitsMFRC522(uint8_t address, uint8_t mask)
{
  uint8_t reg;
  reg = readRegisterMFRC522(address);
  reg = mask | reg;
  writeRegisterMFRC522(address, reg);
}

void clearBitsMFRC522(uint8_t address, uint8_t mask)
{
  uint8_t reg;
  reg = readRegisterMFRC522(address);
  reg = ~mask & reg;
  writeRegisterMFRC522(address, reg);
}

void resetMFRC522(void)
{
  writeRegisterMFRC522(CommandReg, MFRC522_SOFTRESET);
}

uint8_t firmwareMFRC522(void)
{
  uint8_t version;
  version = readRegisterMFRC522(VersionReg);
  return version;
}

void clearAllIRQMFRC522(void)
{
  writeRegisterMFRC522(ComIrqReg, 0x80); //Clear interrupts
  writeRegisterMFRC522(ComlEnReg, 0x7F); //Enable all interrupts
  writeRegisterMFRC522(DivlEnReg, 0x14); //Enable all interrupts
}

uint8_t digitalSelfTestPass(uint8_t *dump)
{
  uint8_t i, n;
  uint8_t selfTestResultV1[] = {0x00, 0xC6, 0x37, 0xD5, 0x32, 0xB7, 0x57, 0x5C,
                                0xC2, 0xD8, 0x7C, 0x4D, 0xD9, 0x70, 0xC7, 0x73,
                                0x10, 0xE6, 0xD2, 0xAA, 0x5E, 0xA1, 0x3E, 0x5A,
                                0x14, 0xAF, 0x30, 0x61, 0xC9, 0x70, 0xDB, 0x2E,
                                0x64, 0x22, 0x72, 0xB5, 0xBD, 0x65, 0xF4, 0xEC,
                                0x22, 0xBC, 0xD3, 0x72, 0x35, 0xCD, 0xAA, 0x41,
                                0x1F, 0xA7, 0xF3, 0x53, 0x14, 0xDE, 0x7E, 0x02,
                                0xD9, 0x0F, 0xB5, 0x5E, 0x25, 0x1D, 0x29, 0x79};
  uint8_t selfTestResultV2[] = {0x00, 0xEB, 0x66, 0xBA, 0x57, 0xBF, 0x23, 0x95,
                                0xD0, 0xE3, 0x0D, 0x3D, 0x27, 0x89, 0x5C, 0xDE,
                                0x9D, 0x3B, 0xA7, 0x00, 0x21, 0x5B, 0x89, 0x82,
                                0x51, 0x3A, 0xEB, 0x02, 0x0C, 0xA5, 0x00, 0x49,
                                0x7C, 0x84, 0x4D, 0xB3, 0xCC, 0xD2, 0x1B, 0x81,
                                0x5D, 0x48, 0x76, 0xD5, 0x71, 0x61, 0x21, 0xA9,
                                0x86, 0x96, 0x83, 0x38, 0xCF, 0x9D, 0x5B, 0x6D,
                                0xDC, 0x15, 0xBA, 0x3E, 0x7D, 0x95, 0x3B, 0x2F};
  uint8_t *selfTestResult;

  // Selecciono el vector de comparacion segun la version de Firmware
  switch (firmwareMFRC522())
  {
  case 0x91:
    selfTestResult = selfTestResultV1;
    break;
  case 0x92:
    selfTestResult = selfTestResultV2;
    break;
  default:
    return false;
  }

  resetMFRC522();
  writeRegisterMFRC522(FIFODataReg, 0x00);           // | FIFOData = 0 |
  writeRegisterMFRC522(CommandReg, MFRC522_MEM);     // stores 25 bytes into the internal buffer
  writeRegisterMFRC522(AutoTestReg, 0x09);           // the self test is enabled by value 1001b
  writeRegisterMFRC522(FIFODataReg, 0x00);           // | FIFOData = 0 |
  writeRegisterMFRC522(CommandReg, MFRC522_CALCCRC); //  performs a self test

  // Wait for the self test to complete.
  i = 0xFF;
  do
  {
    n = readRegisterMFRC522(DivIrqReg);
    i--;
  } while ((i != 0) && !(n & 0x04));

  for (i = 0; i < 64; i++)
  {
    dump[i] = readRegisterMFRC522(FIFODataReg);
    if (dump[i] != selfTestResult[i])
    {
      return false;
    }
  }
  return true;
}

uint8_t sendCommandTag(uint8_t cmd, uint8_t *data, uint32_t dlen, uint8_t *result, uint32_t *rlen)
{
  uint8_t status = MI_ERR;
  uint8_t irqEn = 0x00;
  uint8_t waitIRq = 0x00;
  uint8_t lastBits, n;
  uint32_t i;

  switch (cmd)
  {
  case MFRC522_AUTHENT:
    irqEn = 0x12;
    waitIRq = 0x10;
    break;
  case MFRC522_TRANSCEIVE:
    irqEn = 0x77;
    waitIRq = 0x30;
    break;
  default:
    break;
  }

  writeRegisterMFRC522(ComlEnReg, irqEn | 0x80); // interrupt request
  clearBitsMFRC522(ComIrqReg, 0x80);             // Clear all interrupt requests bits.
  setBitsMFRC522(FIFOLevelReg, 0x80);            // FlushBuffer=1, FIFO initialization.

  writeRegisterMFRC522(CommandReg, MFRC522_IDLE); // No action, cancel the current command.

  // Write to FIFO
  for (i = 0; i < dlen; i++)
  {
    writeRegisterMFRC522(FIFODataReg, data[i]);
  }

  // Execute the command.
  writeRegisterMFRC522(CommandReg, cmd);
  if (cmd == MFRC522_TRANSCEIVE)
  {
    setBitsMFRC522(BitFramingReg, 0x80); // StartSend=1, transmission of data starts
  }

  // Waiting for the command to complete so we can receive data.
  i = 25; // Max wait time is 25ms.
  do
  {
    delay(1);
    // CommIRqReg[7..0]
    // Set1 TxIRq RxIRq IdleIRq HiAlerIRq LoAlertIRq ErrIRq TimerIRq
    n = readRegisterMFRC522(ComIrqReg);
    i--;
  } while ((i != 0) && !(n & 0x01) && !(n & waitIRq));

  clearBitsMFRC522(BitFramingReg, 0x80); // StartSend=0

  if (i != 0)
  { // Request did not time out.
    if (!(readRegisterMFRC522(ErrorReg) & 0x1B))
    { // BufferOvfl Collerr CRCErr ProtocolErr
      status = MI_OK;
      if (n & irqEn & 0x01)
      {
        status = MI_NOTAGERR;
      }

      if (cmd == MFRC522_TRANSCEIVE)
      {
        n = readRegisterMFRC522(FIFOLevelReg);
        lastBits = readRegisterMFRC522(ControlReg) & 0x07;
        if (lastBits)
        {
          *rlen = (n - 1) * 8 + lastBits;
        }
        else
        {
          *rlen = n * 8;
        }

        if (n == 0)
        {
          n = 1;
        }

        if (n > MAX_LEN)
        {
          n = MAX_LEN;
        }

        // Reading the recieved data from FIFO.
        for (i = 0; i < n; i++)
        {
          result[i] = readRegisterMFRC522(FIFODataReg);
        }
      }
    }
    else
    {
      status = MI_ERR;
    }
  }
  return status;
}

uint8_t requestTag(uint8_t mode, uint8_t *data)
{
  uint8_t status;
  uint32_t len;
  writeRegisterMFRC522(BitFramingReg, 0x07); // TxLastBists = BitFramingReg[2..0]

  data[0] = mode;
  status = sendCommandTag(MFRC522_TRANSCEIVE, data, 1, data, &len);

  if ((status != MI_OK) || (len != 0x10))
  {
    status = MI_ERR;
  }

  return status;
}

uint8_t antiCollision(uint8_t *serial)
{
  uint8_t status, i;
  uint32_t len;
  uint8_t check = 0x00;

  writeRegisterMFRC522(BitFramingReg, 0x00); // TxLastBits = BitFramingReg[2..0]

  serial[0] = MF1_ANTICOLL;
  serial[1] = 0x20;
  status = sendCommandTag(MFRC522_TRANSCEIVE, serial, 2, serial, &len);
  len = len / 8; // len is in bits, and we want each byte.
  if (status == MI_OK)
  {
    // The checksum of the tag is the ^ of all the values.
    for (i = 0; i < len - 1; i++)
    {
      check ^= serial[i];
    }
    // The checksum should be the same as the one provided from the
    // tag (serial[4]).
    if (check != serial[i])
    {
      status = MI_ERR;
    }
  }

  return status;
}
