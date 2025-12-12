APP_PATH ?= $(realpath ../..)
-include $(APP_PATH)/config.mk

OPENMRNPATH ?= $(shell \
sh -c "if [ \"X`printenv OPENMRNPATH`\" != \"X\" ]; then printenv OPENMRNPATH; \
     elif [ -d /opt/openmrn/src ]; then echo /opt/openmrn; \
     elif [ -d ~/openmrn/src ]; then echo ~/openmrn; \
     elif [ -d ../../../src ]; then echo ../../..; \
     else echo OPENMRNPATH not found; fi" \
)

# HAL includes for H5
include $(OPENMRNPATH)/etc/stm32cubeh5.mk

LDFLAGSEXTRA +=
SYSLIBRARIESEXTRA +=
OBJEXTRA +=

CFLAGS += -DSTM32H563xx
CXXFLAGS += -DSTM32H563xx

TARGET := freertos.threadx.armv8m
include $(OPENMRNPATH)/etc/prog.mk
