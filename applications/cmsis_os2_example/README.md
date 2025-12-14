# OpenMRN CMSIS-RTOS2 Example Application

This directory contains a simple example application demonstrating how to use OpenMRN with CMSIS-RTOS2 on ARM Cortex-M microcontrollers.

## Overview

This example shows:
- How to initialize OpenMRN with CMSIS-RTOS2
- How to create and manage threads using `osThreadNew()`
- How to use mutexes and semaphores for synchronization
- How to integrate OpenMRN with your platform's CAN interface

## Files

- **CMakeLists.txt** - Build configuration for the example
- **SimpleCanClient.cxx** - Main example application code

## Building

### Prerequisites

- ARM GCC toolchain (arm-none-eabi-gcc)
- CMake 3.20 or later
- OpenMRN repository

### Build Steps

```bash
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=<path-to-arm-toolchain>.cmake ..
cmake --build .
```

### CMake Toolchain

You'll need to provide an ARM toolchain file. Example:

```cmake
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR ARM)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS}")
```

## Key Concepts

### Thread Management

Using CMSIS-RTOS2 directly:
```cpp
osThreadAttr_t attr = {
    .name = "MyThread",
    .stack_size = 2048,
    .priority = osPriorityNormal
};
osThreadId_t thread = osThreadNew(thread_func, NULL, &attr);
```

Using OpenMRN's C++ wrapper:
```cpp
class MyThread : public OSThread {
protected:
    void *entry() override {
        // Thread code here
        return NULL;
    }
};

MyThread thread;
thread.start("MyThread", 0, 2048);
```

### Synchronization

**Semaphores:**
```cpp
os_sem_t sem;
os_sem_init(&sem, 0);  // Initial count of 0

// Signal
os_sem_post(&sem);

// Wait
os_sem_wait(&sem);

// Wait with timeout (nanoseconds)
int result = os_sem_timedwait(&sem, 1000000000);  // 1 second
```

**Mutexes:**
```cpp
os_mutex_t mutex;
os_mutex_init(&mutex);

// Lock
os_mutex_lock(&mutex);
// Critical section
os_mutex_unlock(&mutex);

os_mutex_destroy(&mutex);
```

## Integration with Your Platform

To integrate this with your STM32 or other ARM Cortex-M platform:

1. **Define `__CMSIS_RTOS2`** in your build system:
   ```cmake
   add_definitions(-D__CMSIS_RTOS2)
   ```

2. **Add include path** for CMSIS-OS2 headers:
   ```cmake
   include_directories(${openmrn_SOURCE_DIR}/inc/cmsis_os2)
   ```

3. **Implement platform-specific `os_thread_create()`**:
   ```cpp
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

4. **Initialize your CAN interface** and pass it to the stack

## CMSIS-RTOS2 Requirements

Your platform must provide:
- **cmsis_os2.h** - Standard CMSIS-RTOS2 header
- **CMSIS-RTOS2 kernel** - RTX5, FreeRTOS with CMSIS wrapper, Zephyr, etc.

## OpenMRN Configuration

Set the following in your build configuration:

```cmake
# Enable CMSIS-RTOS2 support
add_definitions(-D__CMSIS_RTOS2)

# Include CMSIS-OS2 specific headers
include_directories(${openmrn_SOURCE_DIR}/inc/cmsis_os2)
```

This automatically enables:
- `OPENMRN_FEATURE_MUTEX_CMSIS_OS2` - CMSIS-RTOS2 mutex implementation
- `OPENMRN_FEATURE_THREAD_CMSIS_OS2` - CMSIS-RTOS2 thread support
- `OPENMRN_FEATURE_RTOS_FROM_ISR` - ISR-safe operations
- `OPENMRN_FEATURE_SEM_TIMEDWAIT` - Timed waits

## Example Usage

See `SimpleCanClient.cxx` for complete examples of:

1. **`openmrn_init()`** - Initializes the stack
2. **`openmrn_start()`** - Starts the executor thread
3. **`openmrn_stop()`** - Stops the executor thread
4. **`MyApplicationThread`** - Shows custom thread creation
5. **`SharedResource`** - Shows mutex usage
6. **`SemaphoreExample`** - Shows semaphore usage

## Testing

To test this example:

1. Set up your platform's build environment
2. Build the example
3. Flash to your target board
4. Verify threads are created and running
5. Monitor CAN bus activity if applicable

## Troubleshooting

**Issue:** `undefined reference to 'os_thread_create'`
- **Solution:** Implement the platform-specific `os_thread_create()` function

**Issue:** Build errors about missing headers
- **Solution:** Ensure `-D__CMSIS_RTOS2` is defined and include path includes `inc/cmsis_os2`

**Issue:** Semaphore/mutex operations failing
- **Solution:** Verify your CMSIS-RTOS2 kernel is properly initialized before calling OpenMRN functions

## References

- [OpenMRN Documentation](https://github.com/bakerstu/openmrn)
- [CMSIS-RTOS2 Standard](https://arm-software.github.io/CMSIS_5/RTOS2/html/index.html)
- [ARM CMSIS Project](https://github.com/ARM-software/CMSIS_5)

## License

This example is provided under the same 2-clause BSD license as OpenMRN.
