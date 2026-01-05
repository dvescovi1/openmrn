/**
 * @file semaphore.h
 * @brief POSIX-compatible semaphore definitions for embedded systems
 *
 * This header provides minimal POSIX semaphore definitions for embedded
 * systems when FreeRTOS is not available.
 */

#ifndef _SEMAPHORE_H
#define _SEMAPHORE_H

#include <stdint.h>
#include <sys/types.h>

/* Opaque semaphore type */
typedef struct {
    uint32_t value;
} sem_t;

/* Semaphore operation functions (stubbed for bare-metal/non-FreeRTOS builds) */

static inline int sem_init(sem_t *sem, int pshared, unsigned int value) {
    if (sem) {
        sem->value = value;
        return 0;
    }
    return -1;
}

static inline int sem_destroy(sem_t *sem) {
    return 0;
}

static inline int sem_post(sem_t *sem) {
    if (sem && sem->value < 0xFFFFFFFFU) {
        sem->value++;
        return 0;
    }
    return -1;
}

static inline int sem_wait(sem_t *sem) {
    if (sem && sem->value > 0) {
        sem->value--;
        return 0;
    }
    return -1;
}

static inline int sem_trywait(sem_t *sem) {
    return sem_wait(sem);
}

static inline int sem_timedwait(sem_t *sem, const struct timespec *abs_timeout) {
    return sem_wait(sem);
}

static inline int sem_getvalue(sem_t *sem, int *sval) {
    if (sem && sval) {
        *sval = (int)sem->value;
        return 0;
    }
    return -1;
}

#endif /* _SEMAPHORE_H */
