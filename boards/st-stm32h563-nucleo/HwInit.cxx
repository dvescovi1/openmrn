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
 * \file HwInit.cxx
 *
 * Hardware initialization for STM32H563 with CMSIS RTOS v2.
 *
 * @author OpenMRN Contributors
 * @date December 2025
 */

#include "hardware.hxx"
#include "cmsis_os2.h"
#include "stm32h5xx_hal.h"

extern "C" {

// FDCAN1 handle
FDCAN_HandleTypeDef hfdcan1;

// USART3 handle
UART_HandleTypeDef huart3;

/** System clock initialization for STM32H563.
 * Configure to run at maximum frequency (250 MHz).
 */
void SystemClock_Config(void)
{
    // TODO: Implement proper clock configuration using STM32 HAL
    // For now, this is a placeholder that assumes default clock
}

/** Hardware initialization - called before RTOS starts */
void hw_preinit(void)
{
    // Initialize system clocks
    SystemClock_Config();
    
    // Initialize HAL
    // HAL_Init();
}

/** Initialize FDCAN1 peripheral */
static void FDCAN1_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // Enable clocks
    __HAL_RCC_FDCAN1_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    // Configure FDCAN1 GPIO pins
    // PD0 -> FDCAN1_RX
    // PD1 -> FDCAN1_TX
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF9_FDCAN1;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    // Configure FDCAN1 peripheral
    hfdcan1.Instance = FDCAN1;
    hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
    hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
    hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
    hfdcan1.Init.AutoRetransmission = ENABLE;
    hfdcan1.Init.TransmitPause = DISABLE;
    hfdcan1.Init.ProtocolException = DISABLE;
    
    // Nominal bit timing: 500 kbit/s
    // Assuming FDCAN kernel clock = 40 MHz (typical default)
    hfdcan1.Init.NominalPrescaler = 5;
    hfdcan1.Init.NominalSyncJumpWidth = 1;
    hfdcan1.Init.NominalTimeSeg1 = 13;
    hfdcan1.Init.NominalTimeSeg2 = 2;
    
    // Data bit timing (not used in Classic CAN mode)
    hfdcan1.Init.DataPrescaler = 5;
    hfdcan1.Init.DataSyncJumpWidth = 1;
    hfdcan1.Init.DataTimeSeg1 = 13;
    hfdcan1.Init.DataTimeSeg2 = 2;
    
    // Message RAM configuration
    hfdcan1.Init.StdFiltersNbr = 28;
    hfdcan1.Init.ExtFiltersNbr = 8;
    hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
    
    if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
    {
        // Initialization Error
        while(1);
    }
    
    // Configure global filter to reject all non-matching frames
    if (HAL_FDCAN_ConfigGlobalFilter(&hfdcan1, 
                                     FDCAN_REJECT, 
                                     FDCAN_REJECT, 
                                     FDCAN_FILTER_REMOTE, 
                                     FDCAN_FILTER_REMOTE) != HAL_OK)
    {
        while(1);
    }
    
    // Start FDCAN peripheral
    if (HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
    {
        while(1);
    }
}

/** Initialize USART3 peripheral */
static void USART3_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    
    // Enable clocks
    __HAL_RCC_USART3_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    
    // Configure USART3 GPIO pins
    // PD8 -> USART3_TX
    // PD9 -> USART3_RX
    GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART3;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);
    
    // Configure USART3
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
    huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        // Initialization Error
        while(1);
    }
}

/** Hardware initialization - called after RTOS starts */
void hw_postinit(void)
{
    // Initialize GPIO
    GpioInit::hw_init();
    
    // Initialize FDCAN1
    FDCAN1_Init();
    
    // Initialize USART3
    USART3_Init();
}

} // extern "C"
