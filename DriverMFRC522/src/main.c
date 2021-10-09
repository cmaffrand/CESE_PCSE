/*============================================================================
 * Autor:
 * Licencia:
 * Fecha:
 *===========================================================================*/

// Inlcusiones

#include "sapi.h" // <= Biblioteca sAPI
#include "main.h" // <= Su propia cabecera

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main(void)
{
   // ---------- CONFIGURACIONES ------------------------------

   //uint8_t buffer[] = {0xAA};
   uint8_t last;
   uint8_t dump[64];
   uint8_t i;
   uint8_t data[MAX_LEN];
   uint8_t uid[5];
   uint8_t clear_uid[] = {0,0,0,0,0};
   uint8_t reg;

   // Inicializar y configurar la plataforma
   boardConfig();

   // Inicializacion del MFRC522
   initMFRC522();
   printf("MFRC522 PowerUp\r\n");

   // Chequeo de version del firmware del MFRC522
   uint8_t version = 0x00;
   printf("Start of MFRC522 SelfTest\r\n");
   version = firmwareMFRC522();
   printf("Firmware: %#02X\r\n", version);

   //Build In Test
   if (digitalSelfTestPass(dump))
      printf("SelfTest: PASS\r\n");
   else
      printf("SelfTest: FAIL\r\n");

   printf("Memory dump: \r\n");
   for (i = 0; i < sizeof(dump) / sizeof(dump[0]); i++)
   {
      printf(" %02X", dump[i]);
      if (!((sizeof(dump) / sizeof(dump[0]) - i + 7) % 8) | (i == sizeof(dump) / sizeof(dump[0]) - 1))
         printf("\r\n");
   }

   // Inicialización del MFRC522
   beginMFRC522();
   printf("MFRC522 PowerUp\r\n");
   enableIRQPort();
   clearAllIRQMFRC522();
   printf("Interrupciones habilitadas\r\n");  

   gpioWrite(LED1, ON);
   gpioWrite(LED2, ON);
   gpioWrite(LED3, OFF);

   // ---------- REPETIR POR SIEMPRE --------------------------
   while (TRUE)
   {
	   __WFI();
	   gpioToggle(LED1);
      delay(1000);

      if (requestTag(MF1_REQIDL, data) == MI_OK)
      {
         printf("Tag detected\r\n");
         printf("Type: %02X, %02X\r\n", data[0],data[1]);         
         if (antiCollision(data) == MI_OK)
         {
            memcpy(uid, data, sizeof(uid) / sizeof(uid[0]));
            printf("User ID: %02X %02X %02X %02X %02X\r\n",uid[0],uid[1],uid[2],uid[3],uid[4]);
         }
      }

      if ((uid[0] == 0xB7) && (uid[1] == 0x4A) && (uid[2] == 0x95) && (uid[3] == 0x4A) && (uid[4] == 0x22))
      {
         printf("Abrierto \r\n");
         gpioWrite(LED3, ON);
         gpioWrite(LED2, OFF);
         delay(3000);
         gpioWrite(LED3, OFF);
         gpioWrite(LED2, ON);
         printf("Cerrado \r\n");
         memcpy(uid, clear_uid, sizeof(uid) / sizeof(uid[0]));
      }
      else if (uid[0] != 0x00)
      {
         printf("Cerrado \r\n");
         gpioWrite(LED3, OFF);
         gpioWrite(LED2, ON);
         delay(1000);
         memcpy(uid, clear_uid, sizeof(uid) / sizeof(uid[0]));
      }
   }

   return 0;
}
