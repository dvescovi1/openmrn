# CMSIS-RTOS2 Support for OpenMRN

This document describes the CMSIS-RTOS2 (CMSIS-OS2) support added to OpenMRN.

## Overview

CMSIS-RTOS2 is a common API for real-time operating systems provided by ARM for Cortex-M microcontrollers. This implementation adds full support for CMSIS-RTOS2 alongside the existing FreeRTOS and POSIX thread implementations.

## Configuration

To enable CMSIS-RTOS2 support, define `__CMSIS_RTOS2` before including OpenMRN headers:

```c
#define __CMSIS_RTOS2
#include "os/OS.hxx"
```

## Features Enabled

When `__CMSIS_RTOS2` is defined, the following features are automatically enabled:

- `OPENMRN_FEATURE_MUTEX_CMSIS_OS2` - CMSIS-RTOS2 mutex and semaphore implementation
- `OPENMRN_FEATURE_THREAD_CMSIS_OS2` - CMSIS-RTOS2 thread management
- `OPENMRN_FEATURE_RTOS_FROM_ISR` - ISR-safe semaphore and queue operations
- `OPENMRN_FEATURE_SEM_TIMEDWAIT` - Timed semaphore wait operations

## API Support

The following OpenMRN OS abstraction APIs are fully supported with CMSIS-RTOS2:

### Thread Management
- `os_thread_create()` - Create threads (requires platform-specific implementation)
- `os_thread_self()` - Get current thread handle
- `os_thread_get_priority()` - Get thread priority
- `os_thread_get_priority_min()` - Get minimum priority value
- `os_thread_get_priority_max()` - Get maximum priority value

### Mutex Operations
- `os_mutex_init()` - Initialize mutex
- `os_recursive_mutex_init()` - Initialize recursive mutex
- `os_mutex_destroy()` - Destroy mutex
- `os_mutex_lock()` - Lock mutex
- `os_mutex_unlock()` - Unlock mutex

### Semaphore Operations
- `os_sem_init()` - Initialize semaphore
- `os_sem_destroy()` - Destroy semaphore
- `os_sem_post()` - Post (increment) semaphore
- `os_sem_post_from_isr()` - Post semaphore from ISR context
- `os_sem_wait()` - Wait for semaphore
- `os_sem_timedwait()` - Wait for semaphore with timeout

### Message Queue Operations
- `os_mq_create()` - Create message queue
- `os_mq_send()` - Send message to queue
- `os_mq_timedsend()` - Send message with timeout
- `os_mq_receive()` - Receive message from queue
- `os_mq_timedreceive()` - Receive message with timeout
- `os_mq_send_from_isr()` - Send message from ISR context

## Implementation Notes

### Timing Resolution
CMSIS-RTOS2 uses millisecond-resolution timeouts. The `NSEC_TO_TICK()` macro automatically converts nanosecond values to milliseconds.

### ISR Context
CMSIS-RTOS2 handles ISR context internally, so explicit wakeup tracking in `*_from_isr()` functions indicates success/failure but doesn't require task context switching management.

### Thread Creation
The `os_thread_create()` function declaration is provided but requires a platform-specific implementation that maps OpenMRN's thread creation parameters to CMSIS-RTOS2's `osThreadNew()` API.

## Files Modified

- `inc/openmrn_features.h` - Added CMSIS-RTOS2 feature detection
- `inc/cmsis_os2_includes.h` - New header for CMSIS-RTOS2 includes and macros
- `src/os/os.h` - Added CMSIS-RTOS2 implementations for all OS abstractions

## Platform Requirements

To use CMSIS-RTOS2 with OpenMRN, your platform must provide:

1. CMSIS-RTOS2 compliant RTOS (e.g., RTX5, FreeRTOS with CMSIS-RTOS2 wrapper, Zephyr)
2. Standard CMSIS-RTOS2 header (`cmsis_os2.h`)
3. Platform-specific implementation of `os_thread_create()` that calls `osThreadNew()`

## Example Platform Integration

```c
#include "cmsis_os2.h"
#include "os/os.h"

// Platform-specific thread creation implementation
int os_thread_create(os_thread_t *thread, const char *name, int priority,
                     size_t stack_size, void *(*start_routine)(void*), void *arg)
{
    osThreadAttr_t attr = {
        .name = name,
        .stack_size = stack_size,
        .priority = (osPriority_t)priority
    };
    
    *thread = osThreadNew((osThreadFunc_t)start_routine, arg, &attr);
    return (*thread != NULL) ? 0 : -1;
}
```

## Testing

The implementation follows the same patterns as the existing FreeRTOS implementation and should work with all existing OpenMRN applications when properly configured for a CMSIS-RTOS2 platform.
