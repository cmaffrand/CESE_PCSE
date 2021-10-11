/*=============================================================================
 * Authors: Carlos Maffrand 	<carlosmaffrand5@gmail.com>
 * Date: 2021/10/08
 *===========================================================================*/

#include "MFRC522.h"
#include "MFRC522spiPort.h"

void initMFRC522(void)
{
  // Inicializa todas funcionalidades necesarias para el MFRC522
  sysTickInit();
  interruptInitPort();
  chipSelectInitPort();
  resetInit();
  spiInitPort();
  resetPort(TRUE);
  chipSelectPort(FALSE);
}

void beginMFRC522(void)
{
  // Configuración inical del MFRC522
  // Soft Reset
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
  // Escitura de un registro en MFRC522
  uint8_t buffer[2];
  buffer[0] = (address << 1) & 0x7E;
  buffer[1] = data;
  chipSelectPort(TRUE);
  spiWritePort(buffer, sizeof(buffer) / sizeof(buffer[0]));
  chipSelectPort(FALSE);
}

uint8_t readRegisterMFRC522(uint8_t address)
{
  // Lectura de un registro en MFRC522
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
  // Seteo de BITS en registro para MFRC522
  uint8_t reg;
  reg = readRegisterMFRC522(address);
  reg = mask | reg;
  writeRegisterMFRC522(address, reg);
}

void clearBitsMFRC522(uint8_t address, uint8_t mask)
{
  // Clean de BITS en registro para MFRC522
  uint8_t reg;
  reg = readRegisterMFRC522(address);
  reg = ~mask & reg;
  writeRegisterMFRC522(address, reg);
}

void resetMFRC522(void)
{
  // Soft Reset en MFRC522
  writeRegisterMFRC522(CommandReg, MFRC522_SOFTRESET);
}

void delayMFRC522(uint64_t duration_ms)
{
  // Delay bloqueante para funciones del MFRC522
  uint64_t tickRateMS = 1;
  uint64_t startTime = GetCycleCounter();
  while ((uint64_t)(GetCycleCounter() - startTime) < duration_ms / tickRateMS)
    ;
}

uint8_t firmwareMFRC522(void)
{
  // Obtiene el Firmaware Version
  uint8_t version;
  version = readRegisterMFRC522(VersionReg);
  return version;
}

void clearAllIRQMFRC522(void)
{
  // Limpia todas las banderas de interrupciones
  writeRegisterMFRC522(ComIrqReg, 0x80); //Clear interrupts
}

void enableAllIRQMFRC522(void)
{
  // Habilita todas las interrupciones
  writeRegisterMFRC522(ComlEnReg, 0x7F); //Enable all interrupts
  writeRegisterMFRC522(DivlEnReg, 0x14); //Enable all interrupts
}

uint8_t digitalSelfTestPass(uint8_t *dump)
{
  uint8_t result = true;
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
  writeRegisterMFRC522(CommandReg, MFRC522_MEM);     // almacena 25 bytes dentro del buffer interno
  writeRegisterMFRC522(AutoTestReg, 0x09);           // se habilita el selftest con 1001b
  writeRegisterMFRC522(FIFODataReg, 0x00);           // | FIFOData = 0 |
  writeRegisterMFRC522(CommandReg, MFRC522_CALCCRC); //  realiza el self test

  // Espera hasta que el SelfTest se complete
  for (uint8_t i = 255; (i > 0) || (readRegisterMFRC522(DivIrqReg) & 0x04); i--) // | CRCIRq = 1 |
    ;

  for (uint8_t i = 0; i < 64; i++)
  {
    dump[i] = readRegisterMFRC522(FIFODataReg);
    if (dump[i] != selfTestResult[i])
      result = false;
  }
  return result;
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

  writeRegisterMFRC522(ComlEnReg, irqEn | 0x80);  // Pedido de interrupciones
  clearBitsMFRC522(ComIrqReg, 0x80);              // Limpia todos los pedidos de interrupciones
  setBitsMFRC522(FIFOLevelReg, 0x80);             // FIFO init | FlushBuffer = 1 |
  writeRegisterMFRC522(CommandReg, MFRC522_IDLE); // Detiene todo con un Idle

  // Escritura a la FIFO
  for (i = 0; i < dlen; i++)
    writeRegisterMFRC522(FIFODataReg, data[i]);

  // Ejecuta el comando
  writeRegisterMFRC522(CommandReg, cmd);
  if (cmd == MFRC522_TRANSCEIVE)
    setBitsMFRC522(BitFramingReg, 0x80); // Comienza transmisión de datos | StartSend = 1 |

  // Espera la finalización del comando con TimeOut = 25ms
  n = readRegisterMFRC522(ComIrqReg);
  for (i = 25; (i > 0) && (n & (0x01 | waitIRq)); i--) // | TimerIRq = 1 | IdleIRq = 1 | RxIRq = 1 (Solo MFRC522_TRANSCEIVE) |
  {
    delayMFRC522(1);
    n = readRegisterMFRC522(ComIrqReg); // CommIRqReg -> | Set1 | TxIRq | RxIRq | IdleIRq | HiAlerIRq | LoAlertIRq | ErrIRq | TimerIRq |
  }

  clearBitsMFRC522(BitFramingReg, 0x80); // | StartSend = 0 |

  if (i != 0) // No Timeout
  {
    // ErrorReg -> | BufferOvfl = 0 | CollErr = 0 | ParityErr = 0 | ProtocolErr = 0 |
    if (!(readRegisterMFRC522(ErrorReg) & 0x1B))
    {
      status = MI_OK;
      if (n & irqEn & 0x01)
        status = MI_NOTAGERR;

      if (cmd == MFRC522_TRANSCEIVE)
      {
        n = readRegisterMFRC522(FIFOLevelReg);
        lastBits = readRegisterMFRC522(ControlReg) & 0x07;
        // Calculo del largo del resultado
        if (lastBits)
          *rlen = (n - 1) * 8 + lastBits;
        else
          *rlen = n * 8;
        // Calculo del largo de datos en FIFO
        if (n > MAX_LEN)
          n = MAX_LEN;
        else if (n == 0)
          n = 1;
        // Lee datos recibidos en FIFO y los almacena en resultado
        for (i = 0; i < n; i++)
          result[i] = readRegisterMFRC522(FIFODataReg);
      }
    }
    else // Timeout
      status = MI_ERR;
  }
  return status;
}

uint8_t requestTag(uint8_t mode, uint8_t *data)
{
  // Pide el User ID del target
  uint8_t status;
  uint32_t len;
  writeRegisterMFRC522(BitFramingReg, 0x07); // | TxLastBists = 7 |
  data[0] = mode;
  status = sendCommandTag(MFRC522_TRANSCEIVE, data, 1, data, &len);
  if ((status != MI_OK) || (len != 0x10))
    status = MI_ERR;
  return status;
}

uint8_t antiCollision(uint8_t *data)
{
  // Configuracion de anti colision con target detectado
  // Esto se logra calculando el CRC de los datos recibidos
  uint8_t status;
  uint32_t i, len;
  uint8_t checkCRC = 0x00;
  writeRegisterMFRC522(BitFramingReg, 0x00); // | TxLastBits = 0 |
  data[0] = MF1_ANTICOLL;
  data[1] = 0x20;
  status = sendCommandTag(MFRC522_TRANSCEIVE, data, 2, data, &len);
  len = len / 8; // El largo viene en Bits, se pasa a bytes.
  if (status == MI_OK)
  {
    // CRC
    for (i = 0; i < len - 1; i++)
      checkCRC ^= data[i]; // XOR bit a bit.
    // El CRC tiene que ser igual que el ultimo byte recibido
    if (checkCRC != data[i])
      status = MI_ERR;
  }
  return status;
}
