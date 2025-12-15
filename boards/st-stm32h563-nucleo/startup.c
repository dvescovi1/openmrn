/** \copyright
 * Copyright (c) 2025, OpenMRN Contributors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  - Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 *  - Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \file startup.c
 *
 * Startup code for STM32H563 with CMSIS RTOS v2.
 *
 * @author OpenMRN Contributors
 * @date December 2025
 */

#include <stdint.h>

extern "C" {

/* External declarations */
extern uint32_t _estack;
extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

/* Main application entry point */
extern int main(void);

/* System initialization functions */
extern void SystemInit(void);
extern void hw_preinit(void);

/* Reset handler */
void Reset_Handler(void);

/* Default interrupt handler */
void Default_Handler(void) {
    while (1) {
        // Hang in loop
    }
}

/* Cortex-M33 core exception handlers */
void NMI_Handler(void) __attribute__((weak, alias("Default_Handler")));
void HardFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void MemManage_Handler(void) __attribute__((weak, alias("Default_Handler")));
void BusFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void UsageFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SecureFault_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SVC_Handler(void) __attribute__((weak, alias("Default_Handler")));
void DebugMon_Handler(void) __attribute__((weak, alias("Default_Handler")));
void PendSV_Handler(void) __attribute__((weak, alias("Default_Handler")));
void SysTick_Handler(void) __attribute__((weak, alias("Default_Handler")));

/* STM32H563 peripheral interrupt handlers */
void WWDG_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
void PVD_PVM_IRQHandler(void) __attribute__((weak, alias("Default_Handler")));
// Add more handlers as needed...

/* Vector table */
__attribute__((section(".isr_vector")))
void (*const g_pfnVectors[])(void) = {
    (void (*)(void))((uint32_t)&_estack), // Initial stack pointer
    Reset_Handler,                        // Reset handler
    NMI_Handler,                          // NMI handler
    HardFault_Handler,                    // Hard fault handler
    MemManage_Handler,                    // MPU fault handler
    BusFault_Handler,                     // Bus fault handler
    UsageFault_Handler,                   // Usage fault handler
    SecureFault_Handler,                  // Secure fault handler
    0,                                    // Reserved
    0,                                    // Reserved
    0,                                    // Reserved
    SVC_Handler,                          // SVCall handler
    DebugMon_Handler,                     // Debug monitor handler
    0,                                    // Reserved
    PendSV_Handler,                       // PendSV handler
    SysTick_Handler,                      // SysTick handler
    
    /* External interrupts */
    WWDG_IRQHandler,                      // Window Watchdog
    PVD_PVM_IRQHandler,                   // PVD/PVM through EXTI
    // Add more peripheral interrupt handlers here...
};

/* Reset handler implementation */
void Reset_Handler(void) {
    uint32_t *pSrc, *pDest;

    /* Copy data segment initializers from flash to SRAM */
    pSrc = &_sidata;
    pDest = &_sdata;
    while (pDest < &_edata) {
        *pDest++ = *pSrc++;
    }

    /* Zero fill the bss segment */
    pDest = &_sbss;
    while (pDest < &_ebss) {
        *pDest++ = 0;
    }

    /* Call system initialization */
    SystemInit();
    
    /* Call hardware pre-initialization */
    hw_preinit();

    /* Call main application */
    main();

    /* Infinite loop if main returns */
    while (1) {
        // Should never reach here
    }
}

/* Weak definition of SystemInit */
__attribute__((weak))
void SystemInit(void) {
    // Override this with actual STM32H5 HAL SystemInit
}

} // extern "C"
