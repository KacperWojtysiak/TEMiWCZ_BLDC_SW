/**
  ******************************************************************************
  * @file           : USB.c
  * @author         : 
  * @brief          : Code for all USB related functionalities.
  ******************************************************************************
  */

/* ------------------------------------- INCLUDES -------------------------------------*/
#include "USB.h"

/* --------------------------------- PRIVATE VARIABLES ---------------------------------*/


/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/

void USB_Send(){
  CDC_Transmit_FS((uint8_t*)"Hello", 5);
  printf("siema\n");
}

uint8_t rx_buf[64];
extern USBD_HandleTypeDef hUsbDeviceFS;

uint8_t CDC_Receive_FS(uint8_t* Buf, uint32_t *Len)
{
    memcpy(rx_buf, Buf, *Len);
    rx_buf[*Len] = '\0';  // jeśli dane są tekstowe

    // Prosty debug – echo
    CDC_Transmit_FS(rx_buf, *Len);

    USBD_CDC_SetRxBuffer(&hUsbDeviceFS, Buf);
    USBD_CDC_ReceivePacket(&hUsbDeviceFS);
    return (USBD_OK);
}