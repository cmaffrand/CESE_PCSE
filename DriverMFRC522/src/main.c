/*=============================================================================
 * Authors: Carlos Maffrand 	<carlosmaffrand5@gmail.com>
 * Date: 2021/10/08
 *===========================================================================*/

// Inlcusiones

#include "sapi.h" // <= Biblioteca sAPI
#include "main.h" // <= Su propia cabecera
#include "functional.h"

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main(void)
{
   // ---------- CONFIGURACIONES ------------------------------

   // Inicializar y configurar la plataforma
   boardConfig();

   // Inicializacion del MFRC522
   initMFRC522();
   printf("MFRC522 Energizado\r\n");

   // Chequeo de version del firmware del MFRC522
   uint8_t version = 0x00;
   printf("Inicio del MFRC522 SelfTest\r\n");
   version = firmwareMFRC522();
   printf("Version Firmware: 0x%02X\r\n", version);

   //Build In Test
   uint8_t dump[64];
   uint8_t pass = digitalSelfTestPass(dump);
   printf("Memory dump: \r\n");
   for (uint32_t i = 0; i < sizeof(dump) / sizeof(dump[0]); i++)
   {
      printf(" %02X", dump[i]);
      if (!((sizeof(dump) / sizeof(dump[0]) - i + 7) % 8) | (i == sizeof(dump) / sizeof(dump[0]) - 1))
         printf("\r\n");
   }
   if (pass)
      printf("SelfTest: PASS\r\n");
   else
   {
      printf("SelfTest: FAIL\r\n");
      printf("MFRC522: ERROR\r\n");
      while (1)
         ;
   }

   // Inicialización del MFRC522
   beginMFRC522();
   printf("MFRC522: Listo\r\n");
   clearAllIRQMFRC522();
   enableIRQPort();
   enableAllIRQMFRC522();
   printf("Interrupciones Habilitadas\r\n");

   // ---------- REPETIR POR SIEMPRE --------------------------
   while (TRUE)
   {
      __WFI();
      gpioToggle(LED1);
      delay(100);
      lockDoor();
   }

   return 0;
}
