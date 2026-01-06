/**
  ******************************************************************************
  * @file    stm32f7xx_hal_conf.h
  * @brief   HAL configuration file for STM32F767 on OpenMRN
  *
  * This file is a cut-down configuration that enables only the HAL modules
  * needed for FreeRTOS drivers on STM32F767.
  ******************************************************************************
  */

#ifndef __STM32F7xx_HAL_CONF_H
#define __STM32F7xx_HAL_CONF_H

#ifdef __cplusplus
extern "C" {
#endif

/* Standard type includes needed for macro definitions */
#include <stdint.h>
#include <stddef.h>

/* Our wrapper stm32f7xx_hal_def.h in freertos_drivers/st/ will handle
   defining the HAL types and breaking the circular dependency. */
#include "stm32_hal_compat.h"

/* ########################## Module Selection ############################## */
#define HAL_MODULE_ENABLED
#define HAL_CORTEX_MODULE_ENABLED
#define HAL_CRC_MODULE_ENABLED
#define HAL_DMA_MODULE_ENABLED
#define HAL_FLASH_MODULE_ENABLED
#define HAL_GPIO_MODULE_ENABLED
#define HAL_I2C_MODULE_ENABLED
#define HAL_PWR_MODULE_ENABLED
#define HAL_RCC_MODULE_ENABLED
#define HAL_SPI_MODULE_ENABLED
#define HAL_TIM_MODULE_ENABLED
#define HAL_UART_MODULE_ENABLED
#define HAL_CAN_MODULE_ENABLED

/* ########################## HSE/HSI Values ################################## */
#if !defined(HSE_VALUE)
  #define HSE_VALUE    ((uint32_t)8000000U)
#endif

#if !defined(HSE_STARTUP_TIMEOUT)
  #define HSE_STARTUP_TIMEOUT    ((uint32_t)100U)
#endif

#if !defined(HSI_VALUE)
  #define HSI_VALUE    ((uint32_t)16000000U)
#endif

#if !defined(LSE_VALUE)
  #define LSE_VALUE    ((uint32_t)32768U)
#endif

#if !defined(LSE_STARTUP_TIMEOUT)
  #define LSE_STARTUP_TIMEOUT    ((uint32_t)5000U)
#endif

#if !defined(EXTERNAL_CLOCK_VALUE)
  #define EXTERNAL_CLOCK_VALUE   ((uint32_t)12288000U)
#endif

/* ########################### System Configuration ######################### */
#define VDD_VALUE                    3300U
#define TICK_INT_PRIORITY            0x0FU
#define USE_RTOS                     0U

/* ########################## Assert Selection ############################### */
#define USE_FULL_ASSERT    0U

/* Define assert_param macro */
#ifdef  USE_FULL_ASSERT
  #define assert_param(expr) ((expr) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
  void assert_failed(uint8_t* file, uint32_t line);
#else
  #define assert_param(expr) ((void)0U)
#endif

/* Include HAL core definitions - which will provide the device structures
   and the HAL function declarations that depend on HAL_StatusTypeDef.
   The stm32_hal_compat.h header (included by stm32f_hal_conf.hxx for library code)
   provides early forward declarations to break the circular dependency. */
#include "stm32f7xx_hal_def.h"

/* Pull in module headers after configuration is set */
#ifdef HAL_CORTEX_MODULE_ENABLED
 #include "stm32f7xx_hal_cortex.h"
#endif

#ifdef HAL_CRC_MODULE_ENABLED
 #include "stm32f7xx_hal_crc.h"
#endif

#ifdef HAL_DMA_MODULE_ENABLED
 #include "stm32f7xx_hal_dma.h"
#endif

#ifdef HAL_FLASH_MODULE_ENABLED
 #include "stm32f7xx_hal_flash.h"
#endif

#ifdef HAL_GPIO_MODULE_ENABLED
 #include "stm32f7xx_hal_gpio.h"
#endif

#ifdef HAL_I2C_MODULE_ENABLED
 #include "stm32f7xx_hal_i2c.h"
#endif

#ifdef HAL_PWR_MODULE_ENABLED
 #include "stm32f7xx_hal_pwr.h"
#endif

#ifdef HAL_RCC_MODULE_ENABLED
 #include "stm32f7xx_hal_rcc.h"
#endif

#ifdef HAL_SPI_MODULE_ENABLED
 #include "stm32f7xx_hal_spi.h"
#endif

#ifdef HAL_TIM_MODULE_ENABLED
 #include "stm32f7xx_hal_tim.h"
#endif

#ifdef HAL_UART_MODULE_ENABLED
 #include "stm32f7xx_hal_uart.h"
#endif

#ifdef HAL_CAN_MODULE_ENABLED
 #include "stm32f7xx_hal_can.h"
#endif

/* Helper function for setting interrupt priorities */
static inline void SetInterruptPriority(uint32_t irq, uint8_t priority)
{
    NVIC_SetPriority((IRQn_Type)irq, priority >> (8U - __NVIC_PRIO_BITS));
}

#ifdef __cplusplus
}
#endif

#endif /* __STM32F7xx_HAL_CONF_H */
