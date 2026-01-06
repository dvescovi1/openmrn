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

/* This header is included early by stm32f7xx_hal_conf.h to ensure proper include order,
   but we don't define anything here to avoid conflicts. The typedefs are defined
   directly in stm32f7xx_hal_conf.h before including hal_def.h. */

#ifdef __cplusplus
}
#endif

#endif /* __STM32_HAL_COMPAT_H */
