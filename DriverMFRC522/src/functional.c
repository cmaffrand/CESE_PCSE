/*=============================================================================
 * Authors: Carlos Maffrand 	<carlosmaffrand5@gmail.com>
 * Date: 2021/10/10
 *===========================================================================*/

#include "functional.h" // <= Su propia cabecera

void lockDoor(void)
{
    uint8_t data[MAX_LEN];
    static uint8_t uid[] = {1, 1, 1, 1, 1};
    uint8_t clear_uid[] = {0, 0, 0, 0, 0};
    uint8_t pass_uid[] = {0xB7, 0x4A, 0x95, 0x4A, 0x22};
    static uint8_t init = 1;

    if (init)
    {
        ledInit();
        printf("App Cerradura Puerta cargada\r\n");
        init = 0;
    }


    if (requestTag(MF1_REQIDL, data) == MI_OK)
    {
        printf("RFID detectado | Tipo: %02X, %02X | ", data[0], data[1]);
        if (antiCollision(data) == MI_OK)
        {
            memcpy(uid, data, sizeof(uid) / sizeof(uid[0]));
            printf("User ID: %02X %02X %02X %02X %02X |\r\n", uid[0], uid[1], uid[2], uid[3], uid[4]);
        }
    }

    if (uid[0] == pass_uid[0] && uid[1] == pass_uid[1] && uid[2] == pass_uid[2] && uid[3] == pass_uid[3] && uid[4] == pass_uid[4])
    {
        printf("Puerta Abierta\r\n");
        gpioWrite(LED3, ON);
        gpioWrite(LED2, OFF);
        delay(3000);
        gpioWrite(LED3, OFF);
        gpioWrite(LED2, ON);
        printf("Puerta Cerrada\r\n");
        memcpy(uid, clear_uid, sizeof(uid) / sizeof(uid[0]));
    }
    else if (uid[0] != clear_uid[0] || uid[1] != clear_uid[1] || uid[2] != clear_uid[2] || uid[3] != clear_uid[3] || uid[4] != clear_uid[4])
    {
        printf("Puerta Cerrada\r\n");
        gpioWrite(LED3, OFF);
        gpioWrite(LED2, ON);
        delay(1000);
        memcpy(uid, clear_uid, sizeof(uid) / sizeof(uid[0]));
    }
}

void ledInit(void)
{
    gpioWrite(LED1, ON);
    gpioWrite(LED2, ON);
    gpioWrite(LED3, OFF);
}