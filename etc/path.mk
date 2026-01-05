# This makefile contains the lookup path for all the external dependencies to
# toolchains and libraries.

ifndef OPENMRN_PATH_MK
OPENMRN_PATH_MK:=1

################# helper functions ##############

# Finds the first occurence of a file under a search path. Returns the first
# entry in the directories list that has file under it, or empty string if none
# found.
# $(1): file or dir to look for, $(2); list of paths
findfirst=$(firstword $(foreach dir,$(2),$(if $(wildcard $(dir)/$(1)),$(wildcard $(dir)))))

# Finds missing dependencies from a list.
#
# Accepts as $(1) a list of variable NAMEs. Returns a string containing the
# name of those variables that have no or empty value. Returns an empty string
# if all dependencies are met.
#
# Usage:
# DEPS += TOOLPATH FREERTOSPATH
# MISSING_DEPS:=$(call find_missing_deps,$(DEPS))
# ifneq (,$(MISSING_DEPS))
# all:
# 	@echo missing dependencies: $(MISSING_DEPS)
# else
# all: build-deps
# endif
find_missing_deps=$(strip $(foreach depvar,$(1),$(if $(value $(depvar)),,$(depvar))))

ifeq ($(OS),Windows_NT)
include $(OPENMRNPATH)/etc/path_windows.mk
else

################ shell ##################
# Various commands in the makefiles are using the bash syntax. We ignore the
# user's login shell preferences and use a specific shell instead.
export SHELL :=/bin/bash

################ flock ##################
ifndef FLOCKPATH
SEARCHPATH := \
  /usr/bin \

TRYPATH:=$(call findfirst,flock,$(SEARCHPATH))
ifneq ($(TRYPATH),)
FLOCKPATH:=$(TRYPATH)
endif
endif #FLOCKPATH


ifndef OPENMRN_EXPLICIT_DEPS_ONLY

################ tivaware ##################
ifndef TIVAWAREPATH
SEARCHPATH := \
  /opt/ti/TivaWare/default \
  /opt/TivaWare/default \
  /opt/TivaWare \
  $(HOME)/TivaWare

TRYPATH:=$(call findfirst,driverlib,$(SEARCHPATH))
ifneq ($(TRYPATH),)
TIVAWAREPATH:=$(TRYPATH)
TRYPATH:=$(call findfirst,inc/hw_onewire.h,$(TRYPATH))
ifneq ($(TRYPATH),)
BUILDTIVAWARE:=$(TRYPATH)
endif
endif
endif #TIVAWAREPATH

################ TI MSPM0 SDK ##################
ifndef TIMSPM0SDKPATH
SEARCHPATH := \
  /opt/ti/mspm0sdk/default

TRYPATH:=$(call findfirst,source/ti/devices/msp/msp.h,$(SEARCHPATH))
ifneq ($(TRYPATH),)
TIMSPM0SDKPATH:=$(TRYPATH)
endif
endif #TIMSPM0SDKPATH

################ STM32Cube_F0 ##################
ifndef STM32CUBEF0PATH
SEARCHPATH := \
  /opt/st/STM32Cube_FW_F0/default

TRYPATH:=$(call findfirst,Drivers,$(SEARCHPATH))
ifneq ($(TRYPATH),)
STM32CUBEF0PATH:=$(TRYPATH)
endif
endif #STM32CUBEF0PATH

################ STM32Cube_F1 ##################
ifndef STM32CUBEF1PATH
SEARCHPATH := \
  /opt/st/STM32Cube_FW_F1/default

TRYPATH:=$(call findfirst,Drivers/CMSIS/Device/ST/STM32F1xx/Include/stm32f103xb.h,$(SEARCHPATH))
ifneq ($(TRYPATH),)
STM32CUBEF1PATH:=$(TRYPATH)
endif
endif #STM32CUBEF1PATH

################ STM32Cube_F3 ##################
ifndef STM32CUBEF3PATH
SEARCHPATH := \
  /opt/st/STM32Cube_FW_F3/default

TRYPATH:=$(call findfirst,Drivers,$(SEARCHPATH))
ifneq ($(TRYPATH),)
STM32CUBEF3PATH:=$(TRYPATH)
endif
endif #STM32CUBEF3PATH

################ STM32Cube_F4 ##################
ifndef STM32CUBEF4PATH
SEARCHPATH := \
  /opt/st/STM32Cube_FW_F4/default

TRYPATH:=$(call findfirst,Drivers,$(SEARCHPATH))
ifneq ($(TRYPATH),)
STM32CUBEF4PATH:=$(TRYPATH)
endif
endif #STM32CUBEF4PATH

################ STM32Cube_L4 ##################
ifndef STM32CUBEL4PATH
SEARCHPATH := \
  /opt/st/STM32Cube_FW_L4/default

TRYPATH:=$(call findfirst,Drivers,$(SEARCHPATH))
ifneq ($(TRYPATH),)
STM32CUBEL4PATH:=$(TRYPATH)
endif
endif #STM32CUBEL4PATH

################ STM32Cube_F7 ##################
ifndef STM32CUBEF7PATH
SEARCHPATH := \
  /opt/st/STM32Cube_FW_F7/default

TRYPATH:=$(call findfirst,Drivers,$(SEARCHPATH))
ifneq ($(TRYPATH),)
STM32CUBEF7PATH:=$(TRYPATH)
endif
endif #STM32CUBEF7PATH

################ STM32Cube_G0 ##################
ifndef STM32CUBEG0PATH
SEARCHPATH := \
  /opt/st/STM32Cube_FW_G0/default \
  $(HOME)/STM32Cube/Repository/STM32Cube_FW_G0_V1.6.1

TRYPATH:=$(call findfirst,Drivers,$(SEARCHPATH))
ifneq ($(TRYPATH),)
STM32CUBEG0PATH:=$(TRYPATH)
endif
endif #STM32CUBEG0PATH

################ lpcopen_18xx_43xx ##################
ifndef LPCOPENPATH_18XX_43XX
SEARCHPATH := \
  /opt/nxp/lpcopen_18xx_43xx/default 

TRYPATH:=$(call findfirst,driverlib,$(SEARCHPATH))
ifneq ($(TRYPATH),)
LPCOPENPATH_18XX_43XX:=$(TRYPATH)
endif
endif #LPCOPENPATH_18XX_43XX

################ lpc_chip_17xx_40xx ##################
ifndef LPCCHIPPATH_17XX_40XX
SEARCHPATH := \
  /opt/nxp/lpc_chip/lpc_chip_17xx_40xx \
  /opt/nxp/lpc_chip/lpc_chip_175x_6x \

TRYPATH:=$(call findfirst,inc/can_17xx_40xx.h,$(SEARCHPATH))
ifneq ($(TRYPATH),)
LPCCHIPPATH_17XX_40XX:=$(TRYPATH)
endif
endif #LPCCHIPPATH_17XX_40XX

################ nxpusblib ##################
ifndef NXPUSBLIBPATH
SEARCHPATH := \
  /opt/nxp/nxpUSBlib/default 

TRYPATH:=$(call findfirst,driverlib,$(SEARCHPATH))
ifneq ($(TRYPATH),)
NXPUSBLIBPATH:=$(TRYPATH)
endif
endif #NXPUSBLIBPATH

################# mbed library ##################

ifndef MBEDPATH
SEARCHPATH := \
  $(HOME)/lpc-workspace/libmbed_2387/mbed \
  $(HOME)/train/libmbed_2387/mbed \
  /opt/mbed/default/libraries \


TRYPATH:=$(call findfirst,USBDevice,$(SEARCHPATH))
ifneq ($(TRYPATH),)
MBEDPATH:=$(TRYPATH)
endif
endif #ifndef MBED_PATH

########### libmaple library source #############
ifndef LIBMAPLEPATH
SEARCHPATH := \
  $(HOME)/libmaple \
  /opt/libmaple/default \

TRYPATH:=$(call findfirst,support/ld/stm32/mem/sram_20k_flash_128k/mem-flash.inc,$(SEARCHPATH))
ifneq ($(TRYPATH),)
LIBMAPLEPATH:=$(TRYPATH)
endif
TRYPATH:=$(call findfirst,Source,$(SEARCHPATH))
ifneq ($(TRYPATH),)
FREERTOSPATH:=$(TRYPATH)
endif
endif #FREERTOSPATH

################### FreeRTOS+TCP ################
ifndef FREERTOSTCPPATH
SEARCHPATH := \
  /opt/FreeRTOSPlus/TCP \
  /opt/FreeRTOSPlus/default/TCP \
  /opt/FreeRTOS/plus-tcp \
  /opt/FreeRTOS-Plus/default/Source/FreeRTOS-Plus-TCP \
  $(HOME)/FreeRTOSPlus/Source/FreeRTOS-Plus-TCP \
  /d/FreeRTOSPlus/default/TCP \

TRYPATH:=$(call findfirst,include/FreeRTOS_DNS.h,$(SEARCHPATH))
ifneq ($(TRYPATH),)
FREERTOSTCPPATH:=$(TRYPATH)
endif
endif #FREERTOSTCPPATH

################# lpcxpresso ####################
ifndef LPCXPRESSOPATH
SEARCHPATH := \
  /opt/lpcxpresso/default/lpcxpresso \
  /opt/lpcxpresso/lpcxpresso_*/lpcxpresso \
  /usr/local/lpcxpresso_*/lpcxpresso \

TRYPATH:=$(call findfirst,bin/LPCXpressoWIN.enc,$(SEARCHPATH))
ifneq ($(TRYPATH),)
LPCXPRESSOPATH:=$(TRYPATH)
endif
endif #LPCXPRESSOPATH

################## checksum #####################
ifndef CHECKSUM
SEARCHPATH := \
  $(LPCXPRESSOPATH)/bin \

TRYPATH:=$(call findfirst,checksum,$(SEARCHPATH))
ifneq ($(TRYPATH),)
CHECKSUM:=$(TRYPATH)/checksum
endif
endif #CHECKSUM

ifndef CHECKSUM
CHECKSUM:=@echo No CHECKSUM binary available - skipping writing header checksum. It is possible that the MCU will drop into bootloader when using this binary. \#
endif


TRYPATH:=$(call findfirst,readme.txt,$(SEARCHPATH))
ifneq ($(TRYPATH),)
TICC3200SDKPATH:=$(TRYPATH)
endif
endif #TICC3200SDKPATH

################### TI-CC3220-SDK #####################
ifndef TICC3220SDKPATH
SEARCHPATH := \
  /opt/ti/CC3220SDK/default  \
  /opt/ti/CC3220SDK/simplelink_cc32xx_sdk_1_30_01_03 \


TRYPATH:=$(call findfirst,source/ti/devices/cc32xx/driverlib/gpio.c,$(SEARCHPATH))
ifneq ($(TRYPATH),)
TICC3220SDKPATH:=$(TRYPATH)
endif
endif #TICC3220SDKPATH

endif #GTESTSRCPATH

############### GMOCK ###################
ifndef GMOCKPATH
SEARCHPATH := \
  $(GTESTPATH)/googlemock \
  $(GTESTPATH)/../googlemock \
  /opt/gtest/default/googlemock \
  /opt/gmock/default \
  /usr \

TRYPATH:=$(call findfirst,include/gmock/gmock.h,$(SEARCHPATH))
ifneq ($(TRYPATH),)
GMOCKPATH:=$(TRYPATH)
endif
endif #GMOCKPATH

ifndef GMOCKSRCPATH
SEARCHPATH := \
  $(GMOCKPATH) \
  /usr/src/gmock \
  /opt/gmock/default \

TRYPATH:=$(call findfirst,src/gmock-all.cc,$(SEARCHPATH))
ifneq ($(TRYPATH),)
GMOCKSRCPATH:=$(TRYPATH)
endif
endif #GMOCKSRCPATH

################### MIPS-ELF-GCC #####################
ifndef MIPSGCCPATH
SEARCHPATH := \
  /opt/CodeSourcery/default_mips_elf \
  /opt/CodeSourcery/Sourcery_CodeBench_Lite_for_MIPS_ELF \
  /opt/MentorGraphics/default_mips_elf \

# Master page for releases is https://sourcery.mentor.com/GNUToolchain/subscription3537?lite=MIPS
# To download latest release (2016-05) go here https://sourcery.mentor.com/GNUToolchain/release3215

TRYPATH:=$(call findfirst,bin/mips-sde-elf-g++,$(SEARCHPATH))
ifneq ($(TRYPATH),)
MIPSGCCPATH:=$(TRYPATH)
endif
endif #MIPSGCCPATH

################### MIPS-ELF-NEWLIB #####################
ifndef MIPSNEWLIBPATH
SEARCHPATH := \
  /opt/newlib/mips-sde-elf \
  $(MIPSGCCPATH)/mips-sde-elf

TRYPATH:=$(call findfirst,lib/el/mips16/sof/libc.a,$(SEARCHPATH))
ifneq ($(TRYPATH),)
MIPSNEWLIBPATH:=$(TRYPATH)
endif
endif #MIPSNEWLIBPATH

################### PIC32MXLIB #####################
ifndef PIC32MXLIBPATH
SEARCHPATH := \
  /opt/microchip/xc32/default \
  $(HOME)/train/git/pic32/includes \

TRYPATH:=$(call findfirst,pic32mx/include/p32xxxx.h,$(SEARCHPATH))
ifneq ($(TRYPATH),)
PIC32MXLIBPATH:=$(TRYPATH)
endif
endif #PIC32MXLIBPATH

################### PIC32MXLEGACYPLIB #####################
ifndef PIC32MXLEGACYPLIBPATH
SEARCHPATH := \
  $(PIC32MXLIBPATH) \
  /opt/microchip/pic32-plib/default \
  /opt/microchip/xc32/v1.32 \
  $(HOME)/train/git/pic32/includes \

TRYPATH:=$(call findfirst,pic32mx/include/peripheral/CAN.h,$(SEARCHPATH))
ifneq ($(TRYPATH),)
PIC32MXLEGACYPLIBPATH:=$(TRYPATH)
endif
endif #PIC32MXLEGACYPLIBPATH

################### PIC32HARMONY #####################
ifndef PIC32HARMONYPATH
SEARCHPATH := \
  /opt/microchip/harmony/default \

TRYPATH:=$(call findfirst,framework/usb/usb_cdc.h,$(SEARCHPATH))
ifneq ($(TRYPATH),)
PIC32HARMONYPATH:=$(TRYPATH)
endif
endif #PIC32HARMONYPATH

##################### OPENOCD ######################
ifndef OPENOCDPATH
SEARCHPATH := \
  /opt/openocd/default/openocd/src \
  /opt/openocd/default/bin \
  /usr/local/bin \
  /usr/bin \

TRYPATH:=$(call findfirst,openocd,$(SEARCHPATH))
ifneq ($(TRYPATH),)
OPENOCDPATH:=$(TRYPATH)
endif
endif #OPENOCDPATH

##################### OPENOCDSCRIPTS ######################
ifndef OPENOCDSCRIPTSPATH
SEARCHPATH := \
  /opt/openocd/default/openocd/tcl \
  /opt/openocd/default/tcl \
  /opt/openocd/default/scripts \
  /opt/openocd/default/share/openocd/scripts \
  /usr/local/share/openocd/scripts \
  /usr/share/openocd/scripts \


TRYPATH:=$(call findfirst,target/stm32f0x.cfg,$(SEARCHPATH))
ifneq ($(TRYPATH),)
OPENOCDSCRIPTSPATH:=$(TRYPATH)
endif
endif #OPENOCDSCRIPTSPATH

##################### EMSDK ######################
ifndef EMSDKPATH
SEARCHPATH := \
  /opt/emscripten/default/emscripten/master \
  /opt/emscripten/emsdk_portable/emscripten/master \
  $(wildcard /opt/emscripten/default/emsdk/emscripten/*) \
  /usr/bin


TRYPATH:=$(call findfirst,emcc,$(SEARCHPATH))
ifneq ($(TRYPATH),)
EMSDKPATH:=$(TRYPATH)
endif
endif #EMSDKPATH

##################### EMLLVM ######################
ifndef EMLLVMPATH
SEARCHPATH := \
  /opt/emscripten/default/clang/fastcomp/build_master_64/bin \
  /opt/emscripten/default/clang/fastcomp/build_master_32/bin \
  $(wildcard /opt/emscripten/default/emsdk/clang/*) \
  /usr/bin


TRYPATH:=$(call findfirst,llvm-ar,$(SEARCHPATH))
ifneq ($(TRYPATH),)
EMLLVMPATH:=$(TRYPATH)
endif
endif #EMLLVMPATH

##################### CLANGPPP ######################
ifndef CLANGPPPATH
SEARCHPATH := \
  /usr/bin \
  /usr/lib/llvm-10/bin \
  /usr/lib/llvm-9/bin \
  /usr/lib/llvm-8/bin \


TRYPATH:=$(call findfirst,clang++,$(SEARCHPATH))
ifneq ($(TRYPATH),)
CLANGPPPATH:=$(TRYPATH)
endif
endif #CLANGPPPATH

##################### HOSTCLANGPP ######################
ifndef HOSTCLANGPPPATH
SEARCHPATH := \
  /usr/bin \


TRYPATH:=$(call findfirst,clang++,$(SEARCHPATH))
ifneq ($(TRYPATH),)
HOSTCLANGPPPATH:=$(TRYPATH)
endif
endif #HOSTCLANGPPPATH

##################### NODEJS ######################
ifndef NODEJSPATH
SEARCHPATH := \
  /usr/bin


TRYPATH:=$(call findfirst,nodejs,$(SEARCHPATH))
ifneq ($(TRYPATH),)
NODEJSPATH:=$(TRYPATH)
endif
endif #NODEJSPATH

##################### ESPOPENSDK ######################
ifndef ESPOPENSDKPATH
SEARCHPATH := \
  /opt/esp/patched/esp-open-sdk \
  /opt/esp/esp-open-sdk \


TRYPATH:=$(call findfirst,xtensa-lx106-elf/bin/xtensa-lx106-elf-gcc,$(SEARCHPATH))
ifneq ($(TRYPATH),)
ESPOPENSDKPATH:=$(TRYPATH)
endif
endif #ESPOPENSDKPATH

##################### ESPARDUINO ######################
ifndef ESPARDUINOPATH
SEARCHPATH := \
  $(HOME)/.arduino15/packages/esp8266 \


TRYPATH:=$(call findfirst,hardware/esp8266/2.2.0/cores/esp8266/core_esp8266_main.cpp,$(SEARCHPATH))
ifneq ($(TRYPATH),)
ESPARDUINOPATH:=$(TRYPATH)
endif
endif #ESPARDUINOPATH

##################### XTENSAGCC ######################
ifndef XTENSAGCCPATH
SEARCHPATH := \
  $(ESPOPENSDKPATH)/xtensa-lx106-elf \
  $(HOME)/.arduino15/packages/esp8266/tools/xtensa-lx106-elf-gcc/1.20.0-26-gb404fb9-2


TRYPATH:=$(call findfirst,bin/xtensa-lx106-elf-gcc,$(SEARCHPATH))
ifneq ($(TRYPATH),)
XTENSAGCCPATH:=$(TRYPATH)
endif
endif #XTENSAGCCPATH

##################### ESPTOOL ######################
ifndef ESPTOOLPATH
SEARCHPATH := \
  $(ESPOPENSDKPATH)/esptool \
  $(XTENSAGCCPATH)/bin \
  $(HOME)/prg/esp/esptool


TRYPATH:=$(call findfirst,esptool.py,$(SEARCHPATH))
ifneq ($(TRYPATH),)
ESPTOOLPATH:=$(TRYPATH)
endif
endif #ESPTOOLPATH

##################### ESPNONOSSDK ######################
ifndef ESPNONOSSDKPATH
SEARCHPATH := \
  /opt/esp/ESP8266_NONOS_SDK \
  $(HOME)/.arduino15/packages/esp8266/hardware/esp8266/2.2.0/tools/sdk \


TRYPATH:=$(call findfirst,include/ets_sys.h,$(SEARCHPATH))
ifneq ($(TRYPATH),)
ESPNONOSSDKPATH:=$(TRYPATH)
endif
endif #ESPNONOSSDKPATH


##################### ESPRTOSSDK ######################
ifndef ESPRTOSSDKPATH
SEARCHPATH := \
  /opt/esp/ESP8266_RTOS_SDK


TRYPATH:=$(call findfirst,third_party/spiffs/esp_spiffs.c,$(SEARCHPATH))
ifneq ($(TRYPATH),)
ESPRTOSSDKPATH:=$(TRYPATH)
endif
endif #ESPRTOSSDKPATH

##################### SXMLC ######################
ifndef SXMLCPATH
SEARCHPATH := \
  /opt/sxmlc/default


TRYPATH:=$(call findfirst,src/sxmlc.h,$(SEARCHPATH))
ifneq ($(TRYPATH),)
SXMLCPATH:=$(TRYPATH)
endif
endif #SXMLCPATH

################ TinyUSB ##################
ifndef TINYUSBPATH
SEARCHPATH := \
  /opt/tinyusb/default \
  /opt/tinyusb/tinyusb \

TRYPATH:=$(call findfirst,src/tusb.c,$(SEARCHPATH))
ifneq ($(TRYPATH),)
TINYUSBPATH:=$(TRYPATH)
endif
endif #TINYUSBPATH


endif # ifndef OPENMRN_EXPLICIT_DEPS_ONLY
endif # if  $(OS)  != Windows_NT
endif # ifndef OPENMRN_PATH_MK
