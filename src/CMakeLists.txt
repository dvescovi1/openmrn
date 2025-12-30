project(openmrn)

if(NOT DEFINED OPENMRNPATH)
set(OPENMRNPATH $ENV{OPENMRNPATH})
endif()

# Default option values
option(FREERTOS "freertos" OFF)
option(USE_DEVICE_FILE_SYSTEM "use device file system" OFF)
option(TARGET_BARE_ARMV7M     "bare armv7m"            OFF)
option(TARGET_ARMV7M          "bare armv7m"            OFF)
option(TARGET_BARE_ARMV6M     "bare armv6m"            OFF)
option(TARGET_ARMV6M          "bare armv6m"            OFF)
option(TARGET_SPIFFS          "spiffs"                 OFF)
option(TARGET_TM4C129         "tm4c129"                OFF)

# List of sources that get compiled.
set(SOURCES
    ${OPENMRNPATH}/src/ble/Advertisement.cpp
    ${OPENMRNPATH}/src/ble/Defs.cpp

    ${OPENMRNPATH}/src/console/Console.cpp

    ${OPENMRNPATH}/src/dcc/dcc_constants.cpp
    ${OPENMRNPATH}/src/dcc/DccDebug.cpp
    ${OPENMRNPATH}/src/dcc/Defs.cpp
    ${OPENMRNPATH}/src/dcc/LocalTrackIf.cpp
    ${OPENMRNPATH}/src/dcc/Loco.cpp
    ${OPENMRNPATH}/src/dcc/Packet.cpp
    ${OPENMRNPATH}/src/dcc/RailcomBroadcastDecoder.cpp
    ${OPENMRNPATH}/src/dcc/RailCom.cpp
    ${OPENMRNPATH}/src/dcc/RailcomDebug.cpp
    ${OPENMRNPATH}/src/dcc/SimpleUpdateLoop.cpp
    ${OPENMRNPATH}/src/dcc/UpdateLoop.cpp
    
    ${OPENMRNPATH}/src/executor/AsyncNotifiableBlock.cpp
    ${OPENMRNPATH}/src/executor/Executor.cpp
    ${OPENMRNPATH}/src/executor/Notifiable.cpp
    ${OPENMRNPATH}/src/executor/Service.cpp
    ${OPENMRNPATH}/src/executor/StateFlow.cpp
    ${OPENMRNPATH}/src/executor/Timer.cpp

    ${OPENMRNPATH}/src/openlcb/AliasAllocator.cpp
    ${OPENMRNPATH}/src/openlcb/AliasCache.cpp
    ${OPENMRNPATH}/src/openlcb/BLEAdvertisement.cpp
    ${OPENMRNPATH}/src/openlcb/BLEService.cpp
    ${OPENMRNPATH}/src/openlcb/BroadcastTime.cpp
    ${OPENMRNPATH}/src/openlcb/BroadcastTimeClient.cpp
    ${OPENMRNPATH}/src/openlcb/BroadcastTimeDefs.cpp
    ${OPENMRNPATH}/src/openlcb/BroadcastTimeServer.cpp
    ${OPENMRNPATH}/src/openlcb/BulkAliasAllocator.cpp
    ${OPENMRNPATH}/src/openlcb/CanDefs.cpp
    ${OPENMRNPATH}/src/openlcb/ConfigEntry.cpp
    ${OPENMRNPATH}/src/openlcb/ConfigUpdateFlow.cpp
    ${OPENMRNPATH}/src/openlcb/Datagram.cpp
    ${OPENMRNPATH}/src/openlcb/DatagramCan.cpp
    ${OPENMRNPATH}/src/openlcb/DatagramTcp.cpp
    ${OPENMRNPATH}/src/openlcb/DccAccyProducer.cpp
    ${OPENMRNPATH}/src/openlcb/DefaultNode.cpp
    ${OPENMRNPATH}/src/openlcb/DefaultCdi.cpp
    ${OPENMRNPATH}/src/openlcb/EventHandler.cpp
    ${OPENMRNPATH}/src/openlcb/EventHandlerContainer.cpp
    ${OPENMRNPATH}/src/openlcb/EventHandlerTemplates.cpp
    ${OPENMRNPATH}/src/openlcb/EventService.cpp
    ${OPENMRNPATH}/src/openlcb/If.cpp
    ${OPENMRNPATH}/src/openlcb/IfCan.cpp
    ${OPENMRNPATH}/src/openlcb/IfImpl.cpp
    ${OPENMRNPATH}/src/openlcb/IfTcp.cpp
    ${OPENMRNPATH}/src/openlcb/MemoryConfig.cpp
    ${OPENMRNPATH}/src/openlcb/nmranet_constants.cpp
    ${OPENMRNPATH}/src/openlcb/Node.cpp
    ${OPENMRNPATH}/src/openlcb/NodeBrowser.cpp
    ${OPENMRNPATH}/src/openlcb/NodeInitializeFlow.cpp
    ${OPENMRNPATH}/src/openlcb/NonAuthoritativeEventProducer.cpp
    ${OPENMRNPATH}/src/openlcb/PIPClient.cpp
    ${OPENMRNPATH}/src/openlcb/RoutingLogic.cpp
    ${OPENMRNPATH}/src/openlcb/SimpleNodeInfo.cpp
    ${OPENMRNPATH}/src/openlcb/SimpleNodeInfoMockUserFile.cpp
    ${OPENMRNPATH}/src/openlcb/SimpleNodeInfoResponse.cpp
    ${OPENMRNPATH}/src/openlcb/SimpleStack.cpp
    ${OPENMRNPATH}/src/openlcb/StreamReceiver.cpp
    ${OPENMRNPATH}/src/openlcb/StreamTransport.cpp
    ${OPENMRNPATH}/src/openlcb/TcpDefs.cpp
    ${OPENMRNPATH}/src/openlcb/TractionCvSpace.cpp
    ${OPENMRNPATH}/src/openlcb/TractionDefs.cpp
    ${OPENMRNPATH}/src/openlcb/TractionProxy.cpp
    ${OPENMRNPATH}/src/openlcb/TractionTestTrain.cpp
    ${OPENMRNPATH}/src/openlcb/TractionThrottle.cpp
    ${OPENMRNPATH}/src/openlcb/TractionTrain.cpp
    ${OPENMRNPATH}/src/openlcb/Velocity.cpp
    ${OPENMRNPATH}/src/openlcb/WriteHelper.cpp

    
    ${OPENMRNPATH}/src/os/FakeClock.cpp
    ${OPENMRNPATH}/src/os/logging_malloc.cpp
    ${OPENMRNPATH}/src/os/MDNS.cpp
    ${OPENMRNPATH}/src/os/os.c
    ${OPENMRNPATH}/src/os/OSImpl.cpp
    ${OPENMRNPATH}/src/os/OSSelectWakeup.cpp
    ${OPENMRNPATH}/src/os/stack_malloc.c
    ${OPENMRNPATH}/src/os/TempFile.cpp
    ${OPENMRNPATH}/src/os/watchdog.c

    ${OPENMRNPATH}/src/traction_modem/Output.cpp

    ${OPENMRNPATH}/src/utils/Base64.cpp
    ${OPENMRNPATH}/src/utils/Blinker.cpp
    ${OPENMRNPATH}/src/utils/Buffer.cpp
    ${OPENMRNPATH}/src/utils/CanIf.cpp
    ${OPENMRNPATH}/src/utils/ClientConnection.cpp
    ${OPENMRNPATH}/src/utils/ConfigUpdateListener.cpp
    ${OPENMRNPATH}/src/utils/constants.cpp
    ${OPENMRNPATH}/src/utils/Crc.cpp
    ${OPENMRNPATH}/src/utils/DirectHub.cpp
    ${OPENMRNPATH}/src/utils/DirectHubGc.cpp
    ${OPENMRNPATH}/src/utils/DirectHubLegacy.cpp
    ${OPENMRNPATH}/src/utils/errno_exit.c
    ${OPENMRNPATH}/src/utils/FdUtils.cpp
    ${OPENMRNPATH}/src/utils/FileUtils.cpp
    ${OPENMRNPATH}/src/utils/format_utils.cpp
    ${OPENMRNPATH}/src/utils/ForwardAllocator.cpp
    ${OPENMRNPATH}/src/utils/GcStreamParser.cpp
    ${OPENMRNPATH}/src/utils/GcTcpHub.cpp
    ${OPENMRNPATH}/src/utils/gc_format.cpp
    ${OPENMRNPATH}/src/utils/GridConnect.cpp
    ${OPENMRNPATH}/src/utils/GridConnectHub.cpp
    ${OPENMRNPATH}/src/utils/HubDevice.cpp
    ${OPENMRNPATH}/src/utils/HubDeviceSelect.cpp
    ${OPENMRNPATH}/src/utils/ieeehalfprecision.c
    ${OPENMRNPATH}/src/utils/JSHubPort.cpp
    ${OPENMRNPATH}/src/utils/logging.cpp
    ${OPENMRNPATH}/src/utils/Queue.cpp
    ${OPENMRNPATH}/src/utils/ReflashBootloader.cpp
    ${OPENMRNPATH}/src/utils/ServiceLocator.cpp
    ${OPENMRNPATH}/src/utils/SocketCan.cpp
    ${OPENMRNPATH}/src/utils/SocketClient.cpp
    ${OPENMRNPATH}/src/utils/socket_listener.cpp
    ${OPENMRNPATH}/src/utils/Stats.cpp
    ${OPENMRNPATH}/src/utils/StringPrintf.cpp
)

if(${FREERTOS})         
set(SOURCES ${SOURCES}
    ${FREERTOSPATH}/Source/croutine.c
    ${FREERTOSPATH}/Source/event_groups.c
    ${FREERTOSPATH}/Source/list.c
    ${FREERTOSPATH}/Source/queue.c
    ${FREERTOSPATH}/Source/tasks.c
    ${FREERTOSPATH}/Source/timers.c
    ${FREERTOSPATH}/Source/portable/GCC/ARM_CM3/port.c
    ${FREERTOSPATH}/Source/portable/MemMang/heap_3.c
)
endif() # FREERTOS

if(${USE_DEVICE_FILE_SYSTEM})
set(SOURCES ${SOURCES}
    ${OPENMRNPATH}/src/freertos_drivers/common/Can.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/CpuLoad.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/Device.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/DeviceBuffer.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/DeviceFile.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/EEPROM.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/EEPROMEmulation.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/EEPROMEmulation_weak.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/Fileio.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/FileioWeak.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/FileSystem.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/I2C.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/MCP2515.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/Node.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/Null.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/PCA9685PWM.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/Pipe.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/Select.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/Serial.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/SPI.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/SPIFlash.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/SN74HC595GPO.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/Socket.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/WifiDefs.cpp
    ${OPENMRNPATH}/src/freertos_drivers/common/TCAN4550Can.cpp
)
endif() # USE_DEVICE_FILE_SYSTEM


if (${GTEST})
# List of tests
set(TESTS ${TESTS}
    ${OPENMRNPATH}/src/ble/Advertisement.cpptest
    ${OPENMRNPATH}/src/ble/Defs.cpptest

    ${OPENMRNPATH}/src/console/Console.cpptest

    ${OPENMRNPATH}/src/dcc/DccDebug.cpptest
    ${OPENMRNPATH}/src/dcc/LogonFeedback.cpptest
    ${OPENMRNPATH}/src/dcc/Packet.cpptest

    ${OPENMRNPATH}/src/executor/AsyncNotifiableBlock.cpptest
    ${OPENMRNPATH}/src/executor/Dispatcher.cpptest
    ${OPENMRNPATH}/src/executor/Notifiable.cpptest
    ${OPENMRNPATH}/src/executor/StateFlow.cpptest
    ${OPENMRNPATH}/src/executor/Timer.cpptest

    ${OPENMRNPATH}/src/openlcb/AliasAllocator.cpptest
    ${OPENMRNPATH}/src/openlcb/AliasCache.cpptest
    ${OPENMRNPATH}/src/openlcb/BLEAdvertisement.cpptest
    ${OPENMRNPATH}/src/openlcb/Bootloader.cpptest
    ${OPENMRNPATH}/src/openlcb/BootloaderDg.cpptest
    ${OPENMRNPATH}/src/openlcb/BroadcastTimeAlarm.cpptest
    ${OPENMRNPATH}/src/openlcb/BroadcastTimeClient.cpptest
    ${OPENMRNPATH}/src/openlcb/BroadcastTimeDefs.cpptest
    ${OPENMRNPATH}/src/openlcb/BroadcastTimeServer.cpptest
    ${OPENMRNPATH}/src/openlcb/CallbackEventHandler.cpptest
    ${OPENMRNPATH}/src/openlcb/CanFilter.cpptest
    ${OPENMRNPATH}/src/openlcb/CanRoutingHub.cpptest
    ${OPENMRNPATH}/src/openlcb/ConfigRenderer.cpptest
    ${OPENMRNPATH}/src/openlcb/ConfigUpdateFlow.cpptest
    ${OPENMRNPATH}/src/openlcb/DatagramCan.cpptest
    ${OPENMRNPATH}/src/openlcb/DatagramTcp.cpptest
    ${OPENMRNPATH}/src/openlcb/DccAccyConsumer.cpptest
    ${OPENMRNPATH}/src/openlcb/DccAccyProducer.cpptest
    ${OPENMRNPATH}/src/openlcb/EventHandlerContainer.cpptest
    ${OPENMRNPATH}/src/openlcb/EventHandlerTemplates.cpptest
    ${OPENMRNPATH}/src/openlcb/EventHandlerTemplatesConsumer.cpptest
    ${OPENMRNPATH}/src/openlcb/EventHandlerTemplatesPC.cpptest
    ${OPENMRNPATH}/src/openlcb/EventHandlerTemplatesProducer.cpptest
    ${OPENMRNPATH}/src/openlcb/EventHandlerTemplatesRange.cpptest
    ${OPENMRNPATH}/src/openlcb/EventIdentifyGlobal.cpptest
    ${OPENMRNPATH}/src/openlcb/EventService.cpptest
    ${OPENMRNPATH}/src/openlcb/HubLatency.cpptest
    ${OPENMRNPATH}/src/openlcb/IfCan.cpptest
    ${OPENMRNPATH}/src/openlcb/IfCanStress.cpptest
    ${OPENMRNPATH}/src/openlcb/IfImpl.cpptest
    ${OPENMRNPATH}/src/openlcb/IfTcp.cpptest
    ${OPENMRNPATH}/src/openlcb/MemoryConfig.cpptest
    ${OPENMRNPATH}/src/openlcb/MemoryConfigClient.cpptest
    ${OPENMRNPATH}/src/openlcb/MemoryConfigStream.cpptest
    ${OPENMRNPATH}/src/openlcb/NodeBrowser.cpptest
    ${OPENMRNPATH}/src/openlcb/NodeInitializeFlow.cpptest
    ${OPENMRNPATH}/src/openlcb/NonAuthoritativeEventProducer.cpptest
    ${OPENMRNPATH}/src/openlcb/PIPClient.cpptest
    ${OPENMRNPATH}/src/openlcb/PolledProducer.cpptest
    ${OPENMRNPATH}/src/openlcb/ProtocolIdentification.cpptest
    ${OPENMRNPATH}/src/openlcb/RefreshLoop.cpptest
    ${OPENMRNPATH}/src/openlcb/RoutingLogic.cpptest
    ${OPENMRNPATH}/src/openlcb/SimpleInfoProtocol.cpptest
    ${OPENMRNPATH}/src/openlcb/SimpleNodeInfo.cpptest
    ${OPENMRNPATH}/src/openlcb/SimpleStack.cpptest
    ${OPENMRNPATH}/src/openlcb/SNIPClient.cpptest
    ${OPENMRNPATH}/src/openlcb/StreamReceiver.cpptest
    ${OPENMRNPATH}/src/openlcb/StreamSender.cpptest
    ${OPENMRNPATH}/src/openlcb/StreamTransport.cpptest
    ${OPENMRNPATH}/src/openlcb/TractionConsist.cpptest
    ${OPENMRNPATH}/src/openlcb/TractionCvSpace.cpptest
    ${OPENMRNPATH}/src/openlcb/TractionDefs.cpptest
    ${OPENMRNPATH}/src/openlcb/TractionTestTrain.cpptest
    ${OPENMRNPATH}/src/openlcb/TractionThrottle.cpptest
    ${OPENMRNPATH}/src/openlcb/TractionTrain.cpptest
    ${OPENMRNPATH}/src/openlcb/Velocity.cpptest
    ${OPENMRNPATH}/src/openlcb/VirtualMemorySpace.cpptest

    ${OPENMRNPATH}/src/os/FakeClock.cpptest
    ${OPENMRNPATH}/src/os/OS.cpptest

    ${OPENMRNPATH}/src/traction_modem/Defs.cpptest
    ${OPENMRNPATH}/src/traction_modem/Link.cpptest
    ${OPENMRNPATH}/src/traction_modem/MemorySpace.cpptest
    ${OPENMRNPATH}/src/traction_modem/ModemTrain.cpptest
    ${OPENMRNPATH}/src/traction_modem/Output.cpptest
    ${OPENMRNPATH}/src/traction_modem/RxTxFlow.cpptest

    ${OPENMRNPATH}/src/utils/async_if_test_helper.cpptest
    ${OPENMRNPATH}/src/utils/BandwidthMerger.cpptest
    ${OPENMRNPATH}/src/utils/Base64.cpptest
    ${OPENMRNPATH}/src/utils/Blinker.cpptest
    ${OPENMRNPATH}/src/utils/BufferQueue.cpptest
    ${OPENMRNPATH}/src/utils/BusMaster.cpptest
    ${OPENMRNPATH}/src/utils/ByteBuffer.cpptest
    ${OPENMRNPATH}/src/utils/Crc.cpptest
    ${OPENMRNPATH}/src/utils/DataBuffer.cpptest
    ${OPENMRNPATH}/src/utils/Debouncer.cpptest
    ${OPENMRNPATH}/src/utils/DirectHub.cpptest
    ${OPENMRNPATH}/src/utils/DirectHubGc.cpptest
    ${OPENMRNPATH}/src/utils/dummy.cpptest
    ${OPENMRNPATH}/src/utils/EEPROMEmu.cpptest
    ${OPENMRNPATH}/src/utils/EEPROMEmuWithShadow.cpptest
    ${OPENMRNPATH}/src/utils/EntryModel.cpptest
    ${OPENMRNPATH}/src/utils/Fixed16.cpptest
    ${OPENMRNPATH}/src/utils/format_utils.cpptest
    ${OPENMRNPATH}/src/utils/ForwardAllocator.cpptest
    ${OPENMRNPATH}/src/utils/gc_format.cpptest
    ${OPENMRNPATH}/src/utils/GcTcpHub.cpptest
    ${OPENMRNPATH}/src/utils/GridConnect.cpptest
    ${OPENMRNPATH}/src/utils/GridConnectHub.cpptest
    ${OPENMRNPATH}/src/utils/HubDevice.cpptest
    ${OPENMRNPATH}/src/utils/HubDeviceSelect.cpptest
    ${OPENMRNPATH}/src/utils/HubStress.cpptest
    ${OPENMRNPATH}/src/utils/LimitedPool.cpptest
    ${OPENMRNPATH}/src/utils/LimitTimer.cpptest
    ${OPENMRNPATH}/src/utils/LinearMap.cpptest
    ${OPENMRNPATH}/src/utils/LruCounter.cpptest
    ${OPENMRNPATH}/src/utils/macros_ndebug.cpptest
    ${OPENMRNPATH}/src/utils/macros.cpptest
    ${OPENMRNPATH}/src/utils/Map.cpptest
    ${OPENMRNPATH}/src/utils/median.cpptest
    ${OPENMRNPATH}/src/utils/NodeHandlerMap.cpptest
    ${OPENMRNPATH}/src/utils/OpenSSLAesCcm.cpptest
    ${OPENMRNPATH}/src/utils/OptionalArgs.cpptest
    ${OPENMRNPATH}/src/utils/ScheduledQueue.cpptest
    ${OPENMRNPATH}/src/utils/ServiceLocator.cpptest
    ${OPENMRNPATH}/src/utils/SimpleQueue.cpptest
    ${OPENMRNPATH}/src/utils/Singleton.cpptest
    ${OPENMRNPATH}/src/utils/SocketClient.cpptest
    ${OPENMRNPATH}/src/utils/SortedListMap.cpptest
    ${OPENMRNPATH}/src/utils/StlMap.cpptest
    ${OPENMRNPATH}/src/utils/StlMultiMap.cpptest
    ${OPENMRNPATH}/src/utils/StoredBitSet.cpptest
    ${OPENMRNPATH}/src/utils/SyncStream.cpptest
    ${OPENMRNPATH}/src/utils/SysMap.cpptest

    PARENT_SCOPE
)
endif() # ${GTEST}

# Common openmrn library
add_library(${PROJECT_NAME} ${SOURCES})

# Common openmrn includes
target_include_directories(${PROJECT_NAME}
    PUBLIC
        ${OPENMRNPATH}/src
        ${OPENMRNPATH}/include
)

include(${OPENMRNPATH}/cmake/spiffs.cmake)
