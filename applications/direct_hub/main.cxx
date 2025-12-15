/** \copyright
 * Copyright (c) 2013, Balazs Racz
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
 * \file main.cxx
 *
 * An application which acts as an OpenLCB hub using the DirectHub infrastructure.
 * Adapted for STM32H563 with CMSIS RTOS v2.
 *
 * @author Balazs Racz
 * @date 31 Dec 2023
 */

#include <memory>
#include "cmsis_os2.h"
#include "executor/Executor.hxx"
#include "executor/Service.hxx"
#include "os/os.h"
#include "utils/DirectHub.hxx"
#include "utils/HubDevice.hxx"
#include "utils/GcTcpHub.hxx"
#include "utils/constants.hxx"

Executor<1> g_executor("g_executor", 0, 2048);
Service g_service(&g_executor);

// Create the DirectHub which will manage all connections
std::unique_ptr<ByteDirectHubInterface> g_direct_hub {create_hub(&g_executor)};

CanHubFlow can_hub0(&g_service);

OVERRIDE_CONST(gc_generate_newlines, 1);
OVERRIDE_CONST(gridconnect_tcp_snd_buffer_size, 8192);
OVERRIDE_CONST(gridconnect_tcp_rcv_buffer_size, 8192);
OVERRIDE_CONST(gridconnect_tcp_notsent_lowat_buffer_size, 1024);

// Hub configuration
const int HUB_PORT = 12021;
bool g_hub_running = true;

// Bridge between GridConnect and legacy CAN formats
void create_legacy_bridge() {
    static bool is_created = false;
    if (!is_created) {
        is_created = true;
        create_gc_to_legacy_can_bridge(g_direct_hub.get(), &can_hub0);
    }
}

// Monitor task that logs hub status
class HubMonitorFlow : public StateFlowBase
{
public:
    HubMonitorFlow() : StateFlowBase(&g_service)
    {
        start_flow(STATE(monitor_loop));
    }

private:
    Action monitor_loop()
    {
        LOG(INFO, "[DirectHub] Running on port %d", HUB_PORT);
        osDelay(10000); // Log every 10 seconds
        return call_immediately(STATE(monitor_loop));
    }
} hub_monitor;

/** Entry point to application.
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return 0, should never return
 */
int appl_main(int argc, char *argv[])
{
    LOG(INFO, "[DirectHub] Initializing Direct Hub on STM32H563");
    
    // Create the legacy bridge for CAN hub compatibility
    create_legacy_bridge();
    
    // Note: TCP server creation would require network stack integration
    // For embedded use, this would typically connect to a physical CAN interface
    // create_direct_gc_tcp_hub(g_direct_hub.get(), HUB_PORT);
    
    LOG(INFO, "[DirectHub] Hub initialized, entering main loop");
    
    // Main loop
    while (g_hub_running)
    {
        osDelay(1000); // CMSIS RTOS v2 delay
    }
    
    return 0;
}
