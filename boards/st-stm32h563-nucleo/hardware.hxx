#ifndef _HARDWARE_HXX_
#define _HARDWARE_HXX_

#include "Stm32Gpio.hxx"
#include "utils/GpioInitializer.hxx"
#include "BlinkerGPIO.hxx"

// STM32H563 NUCLEO board pinout
// LED: PB0 (Green LED LD1)
// User button: PC13
// CAN: Requires external transceiver or use FDCAN1/FDCAN2

GPIO_PIN(LED_GREEN_RAW, LedPin, B, 0);
GPIO_PIN(SW_USER, GpioInputPU, C, 13);

// CAN pins (FDCAN1 as example)
GPIO_PIN(CAN1_RX, GpioInputPU, D, 0);  // FDCAN1_RX
GPIO_PIN(CAN1_TX, GpioOutputOD, D, 1); // FDCAN1_TX

// USART3 pins (PD8=TX, PD9=RX)
GPIO_PIN(USART3_TX, GpioOutputOD, D, 8); // USART3_TX
GPIO_PIN(USART3_RX, GpioInputPU, D, 9);  // USART3_RX

typedef GpioInitializer<
    LED_GREEN_RAW_Pin,
    SW_USER_Pin,
    CAN1_RX_Pin,
    CAN1_TX_Pin,
    USART3_TX_Pin,
    USART3_RX_Pin> GpioInit;

typedef LED_GREEN_RAW_Pin BLINKER_RAW_Pin;
typedef BLINKER_Pin LED_GREEN_Pin;

#endif // _HARDWARE_HXX_
