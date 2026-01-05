/**
 * @file stm32_hal_compat.h
 * @brief STM32 HAL compatibility shim for bare-metal C++ compilation
 * 
 * This header provides forward declarations and type definitions needed
 * for STM32 HAL headers to compile in bare-metal C++ contexts where
 * normal include chains may cause issues.
 */

#ifndef __STM32_HAL_COMPAT_H
#define __STM32_HAL_COMPAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/* We need to ensure HAL_StatusTypeDef is available before stm32f7xx_hal.h
   tries to use it. However, we can't directly include hal_def.h here as that
   would create circular includes.  Instead, we'll just trust that our definition
   matches what hal_def.h will provide, and rely on C++'s ability to handle
   the redeclaration as long as they're identical. */

/* Declaration of HAL_StatusTypeDef to be available to stm32f7xx_hal.h
   even before hal_def.h is included */
#ifdef __cplusplus
/* In C++, put typedef in extern "C" block to match hal_def.h's declaration */
extern "C" {
typedef enum {
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;
}
#else
/* In C, just define it directly  */
typedef enum {
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;
#endif

#ifdef __cplusplus
}
#endif

#endif /* __STM32_HAL_COMPAT_H */
