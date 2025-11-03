/**
  ******************************************************************************
  * @file    stm32f1xx.h
  * @brief   CMSIS STM32F1xx Device Peripheral Access Layer Header File.
  ******************************************************************************
  */

#ifndef __STM32F1xx_H
#define __STM32F1xx_H

#ifdef __cplusplus
 extern "C" {
#endif /* __cplusplus */

/* STM32F103xB specific */
#if defined(STM32F103xB)
  #include "stm32f103xb.h"
#endif

#include "system_stm32f1xx.h"
#include <stdint.h>

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __STM32F1xx_H */
