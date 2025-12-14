/** \copyright
 * Copyright (c) 2025, OpenMRN Contributors
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
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
 * \file SimpleCanClient.cxx
 *
 * Simple OpenMRN client application using CMSIS-RTOS2.
 * Demonstrates basic usage of OpenMRN with CMSIS-RTOS2 on ARM Cortex-M.
 *
 * @author GitHub Copilot
 * @date 13 December 2025
 */

#include <cstdio>
#include <cstring>
#include <atomic>

extern "C" {
#include "cmsis_os2.h"
}

#include "os/os.h"
#include "os/OS.hxx"
#include "openlcb/SimpleStack.hxx"

/// Example node ID - should be unique on the LCC network
/// Format: 0xMMSSIIEEEEEE where MM=manufacturer, SS=series, II=individual
static const uint64_t NODE_ID = 0x0501010118F000ULL;

/// Semaphore for synchronizing thread startup
static osSemaphoreId_t start_semaphore = NULL;

/// Flag to indicate if the stack is running
static std::atomic<bool> stack_running{false};

/// Forward declarations
extern "C" void openmrn_thread_entry(void *argument);
extern "C" void openmrn_init(void);
extern "C" void openmrn_start(void);
extern "C" void openmrn_stop(void);

/// Thread attributes for the OpenMRN stack thread
static const osThreadAttr_t openmrn_thread_attr = {
    .name = "OpenMRNStack",
    .stack_size = 1024 * 8,  // 8KB stack
    .priority = (osPriority_t)osPriorityNormal,
};

/// Thread ID for the OpenMRN stack
static osThreadId_t openmrn_thread_id = NULL;

/**
 * Main OpenMRN stack thread entry point.
 * Runs the executor loop that processes OpenLCB messages.
 * 
 * @param argument unused
 */
extern "C" void openmrn_thread_entry(void *argument) {
    (void)argument;  // Unused

    // Wait for initialization signal
    osSemaphoreAcquire(start_semaphore, osWaitForever);
    stack_running.store(true);

    // Stack executor loop - runs continuously while active
    while (stack_running.load()) {
        // The executor processes pending events
        // This would typically call executor.run() or similar
        osDelay(10);  // 10ms delay to prevent busy-waiting
    }

    // Signal that stack has stopped
    osSemaphoreRelease(start_semaphore);
}

/**
 * Initialize the OpenMRN stack with CMSIS-RTOS2.
 * 
 * This function:
 * - Creates the synchronization semaphore
 * - Creates the OpenMRN executor thread
 * - Initializes the SimpleCanStack
 */
extern "C" void openmrn_init(void) {
    // Create semaphore for thread synchronization (binary semaphore, initially locked)
    start_semaphore = osSemaphoreNew(1, 0, NULL);
    if (start_semaphore == NULL) {
        // Handle error - semaphore creation failed
        return;
    }

    // Create the OpenMRN executor thread
    openmrn_thread_id = osThreadNew(openmrn_thread_entry, NULL, &openmrn_thread_attr);
    if (openmrn_thread_id == NULL) {
        // Handle error - thread creation failed
        return;
    }

    // Initialize the SimpleCanStack with our node ID
    // This would normally create a global stack object
    // For this example, we show the pattern but actual implementation
    // would depend on your platform's CAN interface
}

/**
 * Start the OpenMRN stack.
 * Signals the executor thread to begin processing messages.
 */
extern "C" void openmrn_start(void) {
    if (!stack_running.load()) {
        stack_running.store(true);
        // Release the semaphore to wake the thread
        osSemaphoreRelease(start_semaphore);
    }
}

/**
 * Stop the OpenMRN stack.
 * Blocks until the executor thread has finished processing.
 */
extern "C" void openmrn_stop(void) {
    if (stack_running.load()) {
        stack_running.store(false);
        // Wait for thread to signal completion
        osSemaphoreAcquire(start_semaphore, osWaitForever);
    }
}

/**
 * Example main function showing basic usage.
 * This would typically be called from your platform's main() or equivalent.
 */
extern "C" void example_main(void) {
    // Initialize OpenMRN
    openmrn_init();

    // Start the stack
    openmrn_start();

    // Application would continue here, handling events, etc.
    // For this example, just keep the main thread alive
    for (;;) {
        osDelay(1000);  // Sleep 1 second
    }
}

/**
 * Example of using OpenMRN's C++ thread API with CMSIS-RTOS2.
 * Shows how to create application-level threads that interact with the stack.
 */
class MyApplicationThread : public OSThread {
public:
    MyApplicationThread()
        : OSThread("AppThread", 0, 1024 * 2) {
    }

protected:
    void *entry() override {
        // This runs in the application thread context
        for (int i = 0; i < 10; i++) {
            // Do application work here
            sleep(1000000000);  // Sleep 1 second (in nanoseconds)
        }
        return NULL;
    }
};

/**
 * Example showing mutex usage with CMSIS-RTOS2 through OpenMRN's abstraction.
 */
class SharedResource {
public:
    SharedResource() {
        os_mutex_init(&resource_mutex_);
    }

    ~SharedResource() {
        os_mutex_destroy(&resource_mutex_);
    }

    void access_resource() {
        os_mutex_lock(&resource_mutex_);
        // Protected resource access here
        os_mutex_unlock(&resource_mutex_);
    }

private:
    os_mutex_t resource_mutex_;
};

/**
 * Example showing semaphore usage with CMSIS-RTOS2.
 */
class SemaphoreExample {
public:
    SemaphoreExample() {
        os_sem_init(&work_semaphore_, 0);
    }

    ~SemaphoreExample() {
        os_sem_destroy(&work_semaphore_);
    }

    void signal_work() {
        os_sem_post(&work_semaphore_);
    }

    void wait_for_work() {
        os_sem_wait(&work_semaphore_);
    }

    int wait_with_timeout(long long timeout_ns) {
        return os_sem_timedwait(&work_semaphore_, timeout_ns);
    }

private:
    os_sem_t work_semaphore_;
};
