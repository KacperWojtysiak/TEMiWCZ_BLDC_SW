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
PCD_HandleTypeDef hpcd_USB_FS;

/* --------------------------------- PRIVATE FUNCTIONS ---------------------------------*/

/* --------------------------------- PUBLIC FUNCTIONS ---------------------------------*/

void USB_PCD_Init(void)
{
  hpcd_USB_FS.Instance = USB;
  hpcd_USB_FS.Init.dev_endpoints = 8;
  hpcd_USB_FS.Init.speed = PCD_SPEED_FULL;
  hpcd_USB_FS.Init.phy_itface = PCD_PHY_EMBEDDED;
  hpcd_USB_FS.Init.Sof_enable = DISABLE;
  hpcd_USB_FS.Init.low_power_enable = DISABLE;
  hpcd_USB_FS.Init.lpm_enable = DISABLE;
  hpcd_USB_FS.Init.battery_charging_enable = DISABLE;
  if (HAL_PCD_Init(&hpcd_USB_FS) != HAL_OK)
  {
    Error_Handler();
  }
}
