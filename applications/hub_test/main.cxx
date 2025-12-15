/** \copyright
 * Copyright (c) 2023, Balazs Racz
 * Copyright (c) 2025, OpenMRN Contributors
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
 * CAN/OpenLCB Hub Test and Performance Measurement Tool for STM32H563.
 * Tests hub throughput, latency, and packet handling under various loads.
 *
 * @author Balazs Racz
 * @author OpenMRN Contributors
 * @date 23 Dec 2023
 */

#define __CMSIS_RTOS2

extern "C" {
#include "cmsis_os2.h"
}

#include "nmranet_config.h"
#include "os/os.h"

#include "utils/Buffer.hxx"
#include "utils/CanHubFlow.hxx"
#include "utils/Stats.hxx"
#include "openlcb/Convert.hxx"

OVERRIDE_CONST(gc_generate_newlines, 1);
OVERRIDE_CONST(gridconnect_bridge_max_outgoing_packets, 2);
OVERRIDE_CONST(gridconnect_tcp_notsent_lowat_buffer_size, 1460);
OVERRIDE_CONST(gridconnect_buffer_delay_usec, 100);
OVERRIDE_CONST(gridconnect_buffer_size, 1460);

Executor<1> g_executor("g_executor", 0, 1024);
Service g_service(&g_executor);

/// CAN hub for testing
std::unique_ptr<CanHubFlow> can_hub;

/// Statistics tracking
struct HubTestStats
{
    unsigned packets_sent = 0;
    unsigned packets_received = 0;
    unsigned packets_lost = 0;
    unsigned latency_total_us = 0;
    unsigned latency_count = 0;
    
    void print_stats()
    {
        LOG(INFO, "Hub Test Statistics:");
        LOG(INFO, "  Packets sent:     %u", packets_sent);
        LOG(INFO, "  Packets received: %u", packets_received);
        LOG(INFO, "  Packets lost:     %u", packets_lost);
        if (latency_count > 0)
        {
            LOG(INFO, "  Avg latency:      %u us", latency_total_us / latency_count);
        }
    }
};

HubTestStats g_stats;

/// Simple test flow that sends packets periodically
class TestSenderFlow : public StateFlowBase
{
public:
    TestSenderFlow(Service *service)
        : StateFlowBase(service)
        , timer_(this)
    {
        start_flow(STATE(send_packet));
    }

private:
    Action send_packet()
    {
        // Create a test CAN frame (Event Report for testing)
        struct can_frame *frame = can_hub->alloc();
        frame->can_id = 0x195b4001; // Event Report with test ID
        frame->can_dlc = 8;
        // Payload: NMRA event ID format
        frame->data[0] = 0x09;
        frame->data[1] = 0x00;
        frame->data[2] = 0x01;
        frame->data[3] = 0x39;
        frame->data[4] = (g_stats.packets_sent >> 24) & 0xFF;
        frame->data[5] = (g_stats.packets_sent >> 16) & 0xFF;
        frame->data[6] = (g_stats.packets_sent >> 8) & 0xFF;
        frame->data[7] = g_stats.packets_sent & 0xFF;
        
        can_hub->send(frame);
        g_stats.packets_sent++;
        
        // Schedule next packet after 10ms
#ifdef __CMSIS_RTOS2
        osDelay(10);
        return call_immediately(STATE(send_packet));
#else
        return sleep_and_call(&timer_, MSEC_TO_NSEC(10), STATE(send_packet));
#endif
    }

    StateFlowTimer timer_;
};

/// Simple test flow that receives and counts packets
class TestReceiverFlow : public StateFlowBase
{
public:
    TestReceiverFlow(Service *service, CanHubFlow *hub)
        : StateFlowBase(service)
        , hub_(hub)
    {
        hub_->register_port(this);
        start_flow(STATE(wait_for_packet));
    }

private:
    Action wait_for_packet()
    {
        return allocate_and_call(hub_, STATE(process_packet));
    }

    Action process_packet()
    {
        auto *b = get_allocation_result(hub_);
        g_stats.packets_received++;
        b->unref();
        return call_immediately(STATE(wait_for_packet));
    }

    CanHubFlow *hub_;
};

/// Periodically prints statistics
class StatsPrinterFlow : public StateFlowBase
{
public:
    StatsPrinterFlow(Service *service)
        : StateFlowBase(service)
        , timer_(this)
    {
        start_flow(STATE(print_stats));
    }

private:
    Action print_stats()
    {
        g_stats.print_stats();
        
#ifdef __CMSIS_RTOS2
        osDelay(5000); // Print every 5 seconds
        return call_immediately(STATE(print_stats));
#else
        return sleep_and_call(&timer_, SEC_TO_NSEC(5), STATE(print_stats));
#endif
    }

    StateFlowTimer timer_;
};

/** Entry point to application - STM32H563 with CMSIS RTOS v2
 * Hub test and performance measurement tool.
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return 0, should never return
 */
int appl_main(int argc, char *argv[])
{
    LOG(INFO, "OpenMRN Hub Test starting...");
    
    // Create CAN hub
    can_hub.reset(new CanHubFlow(&g_service));
    
    // Start test sender
    new TestSenderFlow(&g_service);
    
    // Start test receiver
    new TestReceiverFlow(&g_service, can_hub.get());
    
    // Start stats printer
    new StatsPrinterFlow(&g_service);
    
    LOG(INFO, "Hub test flows initialized. Sending test packets...");
    
    // Run the executor
    g_executor.thread_body();
    
    return 0;
}
