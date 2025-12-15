/** \copyright
 * Copyright (c) 2018, Stuart W. Baker
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
 * OpenLCB Broadcast Time Server for STM32H563 with CMSIS RTOS v2.
 * Broadcasts time/date information to all nodes on the LCC network.
 *
 * @author Stuart Baker
 * @author OpenMRN Contributors
 * @date 25 Nov 2018
 */

#define __CMSIS_RTOS2

extern "C" {
#include "cmsis_os2.h"
}

#include "os/os.h"
#include "nmranet_config.h"

#include "openlcb/SimpleStack.hxx"
#include "openlcb/BroadcastTimeServer.hxx"

// Changes the default behavior by adding a newline after each gridconnect
// packet. Makes it easier for debugging the raw device.
OVERRIDE_CONST(gc_generate_newlines, 1);

// Specifies the 48-bit OpenLCB node identifier. This must be unique for every
// hardware manufactured, so in production this should be replaced by some
// easily incrementable method.
extern const openlcb::NodeID NODE_ID = 0x05010101181aULL;

// Sets up a comprehensive OpenLCB stack for a single virtual node.
openlcb::SimpleCanStack stack(NODE_ID);

// Main time protocol server.
openlcb::BroadcastTimeServer
    timeServer(stack.node(), openlcb::BroadcastTimeDefs::DEFAULT_FAST_CLOCK_ID);

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

    // Initialize time server with default epoch time (1970-01-01 00:00)
    // These values can be updated via OpenLCB configuration or RTC
    timeServer.set_time(0, 0);      // hours, minutes
    timeServer.set_date(1, 1);      // month, day
    timeServer.set_year(1970);      // year
    timeServer.set_rate_quarters(4); // 1x real-time (4 quarters = 1.0)
    timeServer.start();

    // This command donates the main thread to the operation of the stack.
    stack.loop_executor();
    return 0;
}
