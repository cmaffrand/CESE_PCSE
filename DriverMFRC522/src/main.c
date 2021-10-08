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
   uint8_t adddress = 0x00;
   uint8_t data = 0x00;   

   // Inicializar y configurar la plataforma
   boardConfig();
   
   // Inicializacion del MFRC522
   initMFRC522();
   printf("MFRC522 Inicializado\r\n");

   // Chequeo de version del firmware del MFRC522
   uint8_t version = 0x00;
   version = firmwareMFRC522();
   printf("Firmware: %#02x\r\n", version);

   // ---------- REPETIR POR SIEMPRE --------------------------
   while (TRUE)
   {
        
	   delay(1000);
      //writeRegisterMFRC522(adddress,data);
      //data = readRegisterMFRC522(adddress);
      //adddress++;

      //__WFI__
   }

   return 0;
}
