APP_PATH ?= $(realpath ../..)
-include $(APP_PATH)/config.mk
-include local_config.mk

OPENMRNPATH ?= $(shell \
sh -c "if [ \"X`printenv OPENMRNPATH`\" != \"X\" ]; then printenv OPENMRNPATH; \
     elif [ -d /opt/openmrn/src ]; then echo /opt/openmrn; \
     elif [ -d ~/openmrn/src ]; then echo ~/openmrn; \
     elif [ -d ../../../src ]; then echo ../../..; \
     else echo OPENMRNPATH not found; fi" \
)

# Find STM32CubeH5 libraries
include $(OPENMRNPATH)/etc/stm32cubeh5.mk

LDFLAGSEXTRA += 
SYSLIBRARIESEXTRA += -lfreertos_drivers_stm32cubeh5
OBJEXTRA += 

CFLAGS += -DSTM32H563xx
CXXFLAGS += -DSTM32H563xx
OPENOCDARGS = -f board/st_nucleo_h563zi.cfg

ifndef TARGET
export TARGET := freertos.threadx.armv8m
endif
include $(OPENMRNPATH)/etc/prog.mk

ifndef DEFAULT_ADDRESS
DEFAULT_ADDRESS=0x16
endif

include $(OPENMRNPATH)/etc/node_id.mk

ifeq ($(call find_missing_deps,OPENOCDPATH OPENOCDSCRIPTSPATH),)
all: $(EXECUTABLE).bin

flash: $(EXECUTABLE)$(EXTENTION) $(EXECUTABLE).lst
	@if ps ax -o comm | grep -q openocd ; then echo openocd already running. quit existing first. ; exit 1 ; fi
	$(GDB) $< -ex "target remote | $(OPENOCDPATH)/openocd -c \"gdb_port pipe\" --search $(OPENOCDSCRIPTSPATH) $(OPENOCDARGS)" -ex "monitor reset halt" -ex "load" -ex "monitor reset init" -ex "monitor reset run"  -ex "detach" -ex "quit"

gdb:
	@if ps ax -o comm | grep -q openocd ; then echo openocd already running. quit existing first. ; exit 1 ; fi
	$(GDB) $(EXECUTABLE)$(EXTENTION) -ex "target remote | $(OPENOCDPATH)/openocd -c \"gdb_port pipe\" --search $(OPENOCDSCRIPTSPATH) $(OPENOCDARGS)" -ex "continue"

else

flash gdb:
	echo OPENOCD not found ; exit 1

endif
