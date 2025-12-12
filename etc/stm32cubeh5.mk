include $(OPENMRNPATH)/etc/path.mk

ifdef STM32CUBEH5PATH
INCLUDES += -I$(OPENMRNPATH)/src/freertos_drivers/st \
            -I$(STM32CUBEH5PATH)/Drivers/STM32H5xx_HAL_Driver/Inc \
            -I$(STM32CUBEH5PATH)/Drivers/CMSIS/Device/ST/STM32H5xx/Include \
            -I$(STM32CUBEH5PATH)/Drivers/CMSIS/Include
endif

CFLAGS +=
CXXFLAGS +=

DEPS += STM32CUBEH5PATH
