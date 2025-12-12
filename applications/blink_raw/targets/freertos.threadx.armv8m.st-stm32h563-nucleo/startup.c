/** \copyright
 * Copyright (c) 2015, Stuart W Baker
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are  permitted provided that the following conditions are met:
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
 * This file sets up the runtime environment for ST STM32H5 MCUs.
 *
 * @author OpenMRN Contributors
 * @date 2025
 */

#include <stdint.h>

#include "FreeRTOSConfig.h"

/* prototypes */
extern unsigned long *__stack;
extern void reset_handler(void);
static void nmi_handler(void);
static void hard_fault_handler(void);
static void mpu_fault_handler(void);
static void bus_fault_handler(void);
static void usage_fault_handler(void);

extern void SVC_Handler(void);
extern void PendSV_Handler(void);
extern void SysTick_Handler(void);

extern void __libc_init_array(void);

extern int main(int argc, char *argv[]);

extern void ignore_fn(void);

extern const unsigned long cm3_cpu_clock_hz;

/** Exception table for STM32H5 */
__attribute__((section(".interrupt_vector")))
void ( *const __interrupt_vector[])(void) =
{
    (void (*)(void))(&__stack),        /**<  0 initial stack pointer */
    reset_handler,                     /**<  1 reset vector */
    nmi_handler,                       /**<  2 non-maskable interrupt */
    hard_fault_handler,                /**<  3 hard fault */
    mpu_fault_handler,                 /**<  4 memory protection fault */
    bus_fault_handler,                 /**<  5 bus fault */
    usage_fault_handler,               /**<  6 usage fault */
    0,                                 /**<  7 reserved */
    0,                                 /**<  8 reserved */
    0,                                 /**<  9 reserved */
    0,                                 /**< 10 reserved */
    SVC_Handler,                       /**< 11 SV call */
    0,                                 /**< 12 debug monitor */
    0,                                 /**< 13 reserved -- bootloader appentry */
    PendSV_Handler,                    /**< 14 pend SV */
    SysTick_Handler,                   /**< 15 system tick */
    ignore_fn /**< forces the linker to add this fn */
};

/** Get the system clock frequency.
 * @return SystemCoreClock
 */
__attribute__((__weak__)) uint32_t HAL_RCC_GetSysClockFreq(void)
{
    return cm3_cpu_clock_hz;
}

/** Stub function to make the HAL happy.  We don't need it for any of our
 * drivers.
 *
 * @return 0
 */
uint32_t HAL_GetTick(void)
{
    return 0;
}

extern unsigned long __data_section_table;
extern unsigned long __data_section_table_end;
extern unsigned long __bss_section_table;
extern unsigned long __bss_section_table_end;

extern unsigned long _etext;
extern unsigned long _data;
extern unsigned long _edata;
extern unsigned long _bss;
extern unsigned long _ebss;

/** This hardware initialization code will be called before C++ global objects
 * are initialized. */
extern void hw_preinit(void);

extern void hw_set_to_safe(void);

/** Startup the C/C++ runtime environment.
 */
void reset_handler(void)
{
    /* Globally disables interrupts until the FreeRTOS scheduler is up. */
    __asm("cpsid i\n");

    unsigned long *section_table_addr = &__data_section_table;

    /* copy ram load sections from flash to ram */
    while (section_table_addr < &__data_section_table_end)
    {
        unsigned long *src = (unsigned long *)*section_table_addr++;
        unsigned long *dst = (unsigned long *)*section_table_addr++;
        unsigned long  len = (unsigned long)  *section_table_addr++;

        for ( ; len; len -= 4)
        {
            *dst++ = *src++;
        }
    }

    /* zero initialize bss segment(s) */
    while (section_table_addr < &__bss_section_table_end)
    {
        unsigned long *zero = (unsigned long *)*section_table_addr++;
        unsigned long  len  = (unsigned long)  *section_table_addr++;
        
        for ( ; len; len -= 4)
        {
            *zero++ = 0;
        }
    }

    hw_preinit();

    /* call static constructors */
    __libc_init_array();

    /* execute main */
    char *argv[] = {0};
    main(0, argv);

    for ( ; /* forever */ ;)
    {
        /* if we ever return from main, loop forever */
    }
}

extern void resetblink(unsigned long pattern);

/** Fault handling information */
typedef struct fault_information
{
    volatile unsigned long stacked_r0 ;
    volatile unsigned long stacked_r1 ;
    volatile unsigned long stacked_r2 ;
    volatile unsigned long stacked_r3 ;
    volatile unsigned long stacked_r12 ;
    volatile unsigned long stacked_lr ;
    volatile unsigned long stacked_pc ;
    volatile unsigned long stacked_psr ;
    volatile unsigned long _CFSR ;
    volatile unsigned long _HFSR ;
    volatile unsigned long _DFSR ;
    volatile unsigned long _AFSR ;
    volatile unsigned long _BFAR ;
    volatile unsigned long _MMAR ;
} FaultInformation;

/** Global instance so that it can be added to the watch expressions */
volatile FaultInformation faultInfo;

/** Decode the stack state prior to an exception occuring.  This code is
 * inspired by FreeRTOS.
 * @param address address of the stack
 */
__attribute__((optimize("-O0"))) void hard_fault_handler_c( unsigned long *hardfault_args )
{
    /* force a reference in the local variables for debug */
    volatile FaultInformation *fault_info = &faultInfo;

    fault_info->stacked_r0 = ((unsigned long)hardfault_args[0]) ;
    fault_info->stacked_r1 = ((unsigned long)hardfault_args[1]) ;
    fault_info->stacked_r2 = ((unsigned long)hardfault_args[2]) ;
    fault_info->stacked_r3 = ((unsigned long)hardfault_args[3]) ;
    fault_info->stacked_r12 = ((unsigned long)hardfault_args[4]) ;
    fault_info->stacked_lr = ((unsigned long)hardfault_args[5]) ;
    fault_info->stacked_pc = ((unsigned long)hardfault_args[6]) ;
    fault_info->stacked_psr = ((unsigned long)hardfault_args[7]) ;

    // Configurable Fault Status Register
    fault_info->_CFSR = (*((volatile unsigned long *)(0xE000ED28))) ;   
                                                                                    
    // Hard Fault Status Register
    fault_info->_HFSR = (*((volatile unsigned long *)(0xE000ED2C))) ;

    // Debug Fault Status Register
    fault_info->_DFSR = (*((volatile unsigned long *)(0xE000ED30))) ;

    // Auxiliary Fault Status Register
    fault_info->_AFSR = (*((volatile unsigned long *)(0xE000ED3C))) ;

    // MemManage Fault Address Register
    fault_info->_MMAR = (*((volatile unsigned long *)(0xE000ED34))) ;
    // Bus Fault Address Register
    fault_info->_BFAR = (*((volatile unsigned long *)(0xE000ED38))) ;

    hw_set_to_safe();

    __asm("BKPT #0\n") ; // Break into the debugger

    /* When the following line is hit, the variables contain the register values. */
    if (fault_info->stacked_r0  || fault_info->stacked_r1  ||
        fault_info->stacked_r2  || fault_info->stacked_r3  ||
        fault_info->stacked_r12 || fault_info->stacked_lr  ||
        fault_info->stacked_pc  || fault_info->stacked_psr ||
        fault_info->_CFSR       || fault_info->_HFSR       ||
        fault_info->_DFSR       || fault_info->_AFSR       ||
        fault_info->_MMAR       || fault_info->_BFAR)
    {
        resetblink(BLINK_DIE_HARDFAULT);
        for( ;; );
    }
}

/** The fault handler implementation.  This code is inspired by FreeRTOS.
 */
__attribute__((__naked__)) static void hard_fault_handler(void)
{
    __asm volatile
    (
        " tst   lr, #4               \n"
        " ite   eq                   \n"
        " mrseq r0, msp              \n"
        " mrsne r0, psp              \n"
        " b     hard_fault_handler_c \n"
        " bx    lr                   \n"
    );
}

static void nmi_handler(void)
{
    for (; /* forever */;)
    {
    }
}

static void mpu_fault_handler(void)
{
    for ( ; /* forever */ ; )
    {
    }
}

static void bus_fault_handler(void)
{
    for ( ; /* forever */ ; )
    {
    }
}

static void usage_fault_handler(void)
{
    for ( ; /* forever */ ; )
    {
    }
}

/** This is the default handler for exceptions not defined by the application.
 */
void default_interrupt_handler(void) __attribute__((weak));
void default_interrupt_handler(void)
{
    while (1)
        ;
    diewith(BLINK_DIE_UNEXPIRQ);
}

void ignore_fn(void) __attribute__((weak));
void ignore_fn(void) {}
