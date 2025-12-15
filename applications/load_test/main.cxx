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
 * Main file for the load generator application for network performance testing.
 * Adapted for STM32H563 with CMSIS RTOS v2.
 *
 * @author Balazs Racz
 * @date 5 Jun 2015
 */

#include "cmsis_os2.h"
#include "os/os.h"
#include "openlcb/SimpleStack.hxx"
#include "openlcb/EventHandlerTemplates.hxx"
#include "utils/format_utils.hxx"

// Node ID for load test application
extern const openlcb::NodeID NODE_ID = 0x050101011419ULL;

// OpenLCB stack
openlcb::SimpleCanStack stack(NODE_ID);

// Load test configuration
const int PACKETS_PER_SEC = 100;  // Target packet rate
const uint64_t TEST_EVENT_BASE = 0x0501010114DD0000ULL;

// Statistics tracking
struct LoadTestStats {
    uint32_t packets_sent;
    uint32_t send_errors;
    uint32_t uptime_seconds;
    
    LoadTestStats() : packets_sent(0), send_errors(0), uptime_seconds(0) {}
};

LoadTestStats g_stats;

// Packet generator flow
class PacketGeneratorFlow : public StateFlowBase
{
public:
    PacketGeneratorFlow() : StateFlowBase(stack.service())
    {
        start_flow(STATE(generate_loop));
    }

private:
    Action generate_loop()
    {
        // Generate test event
        uint64_t event_id = TEST_EVENT_BASE + (g_stats.packets_sent & 0xFFFF);
        
        // Send event
        stack.send_event(event_id);
        g_stats.packets_sent++;
        
        // Calculate delay for target packet rate
        uint32_t delay_ms = 1000 / PACKETS_PER_SEC;
        osDelay(delay_ms);
        
        return call_immediately(STATE(generate_loop));
    }
} packet_generator;

// Statistics printer flow
class StatsPrinterFlow : public StateFlowBase
{
public:
    StatsPrinterFlow() : StateFlowBase(stack.service())
    {
        start_flow(STATE(print_loop));
    }

private:
    Action print_loop()
    {
        g_stats.uptime_seconds++;
        
        LOG(INFO, "[LoadTest] Uptime: %u sec | Packets: %u | Errors: %u | Rate: %u pkt/s",
            g_stats.uptime_seconds,
            g_stats.packets_sent,
            g_stats.send_errors,
            g_stats.packets_sent / (g_stats.uptime_seconds > 0 ? g_stats.uptime_seconds : 1));
        
        osDelay(1000); // Print stats every second
        return call_immediately(STATE(print_loop));
    }
} stats_printer;

/** Entry point to application.
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return 0, should never return
 */
int appl_main(int argc, char *argv[])
{
    LOG(INFO, "[LoadTest] Initializing Load Test Generator on STM32H563");
    LOG(INFO, "[LoadTest] Target rate: %d packets/sec", PACKETS_PER_SEC);
    LOG(INFO, "[LoadTest] Node ID: 0x%012llx", NODE_ID);
    
    // Note: In embedded context, would need to initialize CAN interface here
    // For example: stack.add_can_port_select("/dev/can0");
    
    LOG(INFO, "[LoadTest] Starting packet generation...");
    
    // Main loop - flows handle packet generation and statistics
    while (true)
    {
        osDelay(10000); // Main loop runs every 10 seconds
    }
    
    return 0;
}
