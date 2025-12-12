# FreeRTOS wrapper on Azure RTOS ThreadX for ARMv8-M (Cortex-M33)
# Get the $(FREERTOSPATH) for wrapper headers if used
include $(OPENMRNPATH)/etc/freertos.mk

# Get env flags
include $(OPENMRNPATH)/etc/env.mk

# Toolchain
ifeq ($(TOOLPATH),)
include $(OPENMRNPATH)/etc/armgcc.mk
endif

PREFIX = $(TOOLPATH)/bin/arm-none-eabi-

AS = $(PREFIX)gcc
CC = $(shell $(OPENMRNPATH)/bin/find_distcc.sh $(realpath $(PREFIX)gcc))
CXX = $(shell $(OPENMRNPATH)/bin/find_distcc.sh $(realpath $(PREFIX)g++))
AR = $(PREFIX)ar
LD = $(PREFIX)g++
SIZE = $(PREFIX)size
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump
GDB = $(PREFIX)gdb

AROPTS=D
OBJDUMPOPTS=-C

STARTGROUP := -Wl,--start-group
ENDGROUP := -Wl,--end-group

# CMSIS/STM32CubeH5 HAL includes
include $(OPENMRNPATH)/etc/stm32cubeh5.mk

INCLUDES += -I$(OPENMRNPATH)/include/freertos \
            -idirafter $(OPENMRNPATH)/include/freertos_select \
            -I$(OPENMRNPATH)/src/freertos_drivers/common

# Optimization and arch flags for Cortex-M33
ARCHOPTIMIZATION += -Os -fno-strict-aliasing -fomit-frame-pointer -fdata-sections -ffunction-sections
ARCHFLAGS = -g -MD -MP -mcpu=cortex-m33 -mthumb -mfloat-abi=soft

ASFLAGS = -c $(ARCHFLAGS)

CORECFLAGS = $(ARCHOPTIMIZATION) $(ARCHFLAGS) -Wall -Werror -Wno-unknown-pragmas \
             -fdata-sections -ffunction-sections -fno-builtin -fno-stack-protector \
             -D__FreeRTOS__ -DGCC_ARMCM33 -specs=nano.specs

CFLAGS += -c $(CORECFLAGS) -std=c99 -Wstrict-prototypes $(CFLAGSENV) $(CFLAGSEXTRA)

CXXFLAGS += -c $(CORECFLAGS) -std=c++14 -D_ISOC99_SOURCE -D__STDC_FORMAT_MACROS \
            -fno-exceptions -fno-rtti -Wsuggest-override -Wno-psabi \
            $(CXXFLAGSENV) $(CXXFLAGSEXTRA)

LDFLAGS += -g -fdata-sections -ffunction-sections -T target.ld \
           $(ARCHFLAGS) -Os -Wl,-Map="$(@:%.elf=%.map)" -Wl,--gc-sections \
           -Wl,--undefined=ignore_fn $(LDFLAGSEXTRA) $(LDFLAGSENV) --specs=nano.specs

SYSLIB_SUBDIRS +=
SYSLIBRARIES +=

# newlib exception/unwind disabling wrappers
SYSLIBRARIES += $(SYSLIBRARIESEXTRA) \
          -Wl,--wrap=__cxa_pure_virtual \
          -Wl,--defsym=__wrap___cxa_pure_virtual=abort \
          -Wl,--wrap=__cxa_atexit \
          -Wl,--defsym=__wrap___cxa_atexit=ignore_fn \
          -Wl,--wrap=__aeabi_atexit \
          -Wl,--defsym=__wrap___aeabi_atexit=ignore_fn \
          -Wl,--wrap=__gxx_personality_v0 \
          -Wl,--defsym=__wrap___gxx_personality_v0=abort

EXTENTION = .elf
