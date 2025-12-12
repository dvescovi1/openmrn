#ifndef _HARDWARE_HXX_
#define _HARDWARE_HXX_

#include "Stm32Gpio.hxx"
#include "utils/GpioInitializer.hxx"
#include "BlinkerGPIO.hxx"

// Adjust pins as per NUCLEO-H563ZI (example): LED on PB0, USER button PC13
GPIO_PIN(LED_GREEN_RAW, LedPin, B, 0);
GPIO_PIN(SW_USER, GpioInputPU, C, 13);

typedef GpioInitializer<LED_GREEN_RAW_Pin,SW_USER_Pin> GpioInit;

typedef LED_GREEN_RAW_Pin BLINKER_RAW_Pin;
typedef BLINKER_Pin LED_GREEN_Pin;

#endif // _HARDWARE_HXX_
