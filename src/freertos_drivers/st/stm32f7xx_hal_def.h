/**
  ******************************************************************************
  * @file    stm32f7xx_hal_def.h
  * @brief   Wrapper to fix circular include in STM32F7 HAL
  * 
  * The STM32 hal_def.h has a circular dependency where HAL_StatusTypeDef
  * is used before it's defined. This wrapper defines the types upfront
  * so they're available during the circular include chain, then includes
  * stm32f7xx.h to get access to device structures.
  ******************************************************************************
  */

#ifndef __STM32F7xx_HAL_DEF
#define __STM32F7xx_HAL_DEF

#ifdef __cplusplus
 extern "C" {
#endif

#include <stddef.h>

/* Define HAL types BEFORE the circular include chain that tries to use them.
   stm32f7xx.h -> stm32f7xx_hal.h uses these types. */
#if !defined(__HAL_STATUS_TYPEDEF_DEFINED)
#define __HAL_STATUS_TYPEDEF_DEFINED

typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;
#endif

#if !defined(__HAL_LOCK_TYPEDEF_DEFINED)
#define __HAL_LOCK_TYPEDEF_DEFINED

typedef enum
{
  HAL_UNLOCKED = 0x00U,
  HAL_LOCKED   = 0x01U
} HAL_LockTypeDef;
#endif

/* Now include stm32f7xx.h which will:
   1. Include stm32f7xx_hal.h
   2. stm32f7xx_hal.h tries to use HAL_StatusTypeDef (now available)
   3. stm32f7xx_hal.h includes stm32f7xx_hal_def.h
   4. But __STM32F7xx_HAL_DEF is already defined (this file), so STM32's version is skipped
   This breaks the circular dependency. */
#include "stm32f7xx.h"

/* Include legacy HAL definitions */
#include "Legacy/stm32_hal_legacy.h"

/* Macro definitions from STM32's hal_def.h */
#if !defined(UNUSED)
#define UNUSED(X) (void)X
#endif

#define HAL_MAX_DELAY      0xFFFFFFFFU
#define HAL_IS_BIT_SET(REG, BIT)  (((REG) & (BIT)) == (BIT))
#define HAL_IS_BIT_CLR(REG, BIT)  (((REG) & (BIT)) == 0U)

#define __HAL_LINKDMA(__HANDLE__, __PPP_DMA_FIELD__, __DMA_HANDLE__)  \
                        do{                                              \
                            (__HANDLE__)->__PPP_DMA_FIELD__ = &(__DMA_HANDLE__); \
                            (__DMA_HANDLE__).Parent = (__HANDLE__);     \
                          } while(0U)

#define __HAL_RESET_HANDLE_STATE(__HANDLE__)  ((__HANDLE__)->State = 0U)

#if (USE_RTOS == 1U)
  #define __HAL_LOCK(__HANDLE__)  \
                        do{                                                     \
                            if((__HANDLE__)->Lock == HAL_LOCKED)                \
                            {                                                   \
                               return HAL_BUSY;                                 \
                            }                                                   \
                            else                                                \
                            {                                                   \
                               (__HANDLE__)->Lock = HAL_LOCKED;                 \
                            }                                                   \
                          }while (0U)
  #define __HAL_UNLOCK(__HANDLE__)  \
                        do{                                                     \
                            (__HANDLE__)->Lock = HAL_UNLOCKED;                  \
                          }while (0U)
#else
  #define __HAL_LOCK(__HANDLE__)  \
                        do{                                                     \
                            if((__HANDLE__)->Lock == HAL_LOCKED)                \
                            {                                                   \
                               return HAL_BUSY;                                 \
                            }                                                   \
                            else                                                \
                            {                                                   \
                               (__HANDLE__)->Lock = HAL_LOCKED;                 \
                            }                                                   \
                          }while (0U)
  #define __HAL_UNLOCK(__HANDLE__)  \
                        do{                                                     \
                            (__HANDLE__)->Lock = HAL_UNLOCKED;                  \
                          }while (0U)
#endif

#if defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
  #define __weak  __attribute__((weak))
  #define __packed __attribute__((packed))
#elif defined ( __GNUC__ ) && !defined (__CC_ARM)
  #define __weak   __attribute__((weak))
  #define __packed __attribute__((__packed__))
#elif defined(__ICCARM__)
  #define __weak __weak
  #define __packed __packed
#elif defined ( __TASKING__ )
  #define __weak  __attribute__((weak))
  #define __packed __unaligned
#endif

#ifdef __cplusplus
}
#endif

#endif /* __STM32F7xx_HAL_DEF */

