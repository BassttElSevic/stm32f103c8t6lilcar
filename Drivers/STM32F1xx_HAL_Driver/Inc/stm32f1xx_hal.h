/**
  ******************************************************************************
  * @file    stm32f1xx_hal.h
  * @brief   This file contains all the functions prototypes for the HAL 
  *          module driver.
  ******************************************************************************
  */

#ifndef __STM32F1xx_HAL_H
#define __STM32F1xx_HAL_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f1xx_hal_conf.h"

/* Exported types ------------------------------------------------------------*/
typedef enum 
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/
HAL_StatusTypeDef HAL_Init(void);
HAL_StatusTypeDef HAL_DeInit(void);
void HAL_IncTick(void);
uint32_t HAL_GetTick(void);
void HAL_Delay(uint32_t Delay);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_HAL_H */
