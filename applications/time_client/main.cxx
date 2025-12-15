/** \copyright
 * Copyright (c) 2018, Balazs Racz
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
 * OpenLCB Broadcast Time Client for STM32H563 with CMSIS RTOS v2.
 * Receives and displays time/date from a time server on the LCC network.
 *
 * @author Balazs Racz
 * @author OpenMRN Contributors
 * @date 19 Nov 2018
 */

#define __CMSIS_RTOS2

extern "C" {
#include "cmsis_os2.h"
}

#include "os/os.h"
#include "nmranet_config.h"

#include "openlcb/SimpleStack.hxx"
#include "openlcb/BroadcastTimeClient.hxx"

// Changes the default behavior by adding a newline after each gridconnect
// packet. Makes it easier for debugging the raw device.
OVERRIDE_CONST(gc_generate_newlines, 1);

// Specifies the 48-bit OpenLCB node identifier. This must be unique for every
// hardware manufactured, so in production this should be replaced by some
// easily incrementable method.
extern const openlcb::NodeID NODE_ID = 0x05010101181bULL;

// Sets up a comprehensive OpenLCB stack for a single virtual node.
openlcb::SimpleCanStack stack(NODE_ID);

// Forward declarations
void time_update_callback(time_t old, time_t current);
void minute_update_callback(BarrierNotifiable *done);

// Main time protocol client.
openlcb::BroadcastTimeClient
    timeClient(stack.node(), openlcb::BroadcastTimeDefs::DEFAULT_FAST_CLOCK_ID);

// Alarm that triggers every minute
openlcb::BroadcastTimeAlarmMinute timeAlarmMinute(stack.node(), &timeClient,
                                                  &minute_update_callback);

void print_time(const char* prefix) {
    auto* tm = timeClient.gmtime_recalculate();
    LOG(INFO, "%s%04d-%02d-%02d %02d:%02d, %s, rate %.2f", prefix,
        tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
        tm->tm_hour, tm->tm_min,
        timeClient.is_running() ? "running" : "stopped",
        timeClient.get_rate_quarters() * 1.0f / 4);
}

/// Callback from the alarm that gets called on every minute change.
/// @param done used to notify we are finished
void minute_update_callback(BarrierNotifiable *done)
{
    print_time("");
    done->notify();
}

/// Callback from the time client when the time jumps or is reset.
void time_update_callback(time_t old, time_t current)
{
    print_time("update: ");
}

/** Entry point to application - STM32H563 with CMSIS RTOS v2
 * @param argc number of command line arguments
 * @param argv array of command line arguments
 * @return 0, should never return
 */
int appl_main(int argc, char *argv[])
{
#ifdef __CMSIS_RTOS2
    // STM32H563 with CMSIS RTOS v2 - use CAN port
    stack.add_can_port_select("/dev/can0");
#else
    // Connects to a TCP hub
    stack.connect_tcp_gridconnect_hub("localhost", 12021);
#endif

    // Subscribe to time update notifications
    timeClient.update_subscribe_add(&time_update_callback);
    
    // This command donates the main thread to the operation of the stack.
    stack.loop_executor();
    return 0;
}
