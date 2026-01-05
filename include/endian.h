/**
 * @file endian.h
 * @brief Endianness macros for embedded ARM systems without POSIX libc
 *
 * This header provides endianness detection and conversion macros for
 * embedded ARM systems when FreeRTOS/POSIX libc is not available.
 */

#ifndef _ENDIAN_H
#define _ENDIAN_H

#include <stdint.h>

/* Endianness constants */
#define __LITTLE_ENDIAN 1234
#define __BIG_ENDIAN    4321
#define __PDP_ENDIAN    3412

/* ARM Cortex-M devices are always little-endian */
#define __BYTE_ORDER __LITTLE_ENDIAN

/* Macro versions for compile-time use */
#define LITTLE_ENDIAN __LITTLE_ENDIAN
#define BIG_ENDIAN    __BIG_ENDIAN
#define PDP_ENDIAN    __PDP_ENDIAN
#define BYTE_ORDER    __BYTE_ORDER

/* Endianness conversion macros */

/* Host to network byte order (big-endian) */
#define htonl(x) __builtin_bswap32(x)
#define htons(x) __builtin_bswap16(x)

/* Network to host byte order */
#define ntohl(x) __builtin_bswap32(x)
#define ntohs(x) __builtin_bswap16(x)

/* Byte order conversion functions */
static inline uint16_t be16toh(uint16_t x) { return __builtin_bswap16(x); }
static inline uint16_t htobe16(uint16_t x) { return __builtin_bswap16(x); }
static inline uint16_t le16toh(uint16_t x) { return x; }
static inline uint16_t htole16(uint16_t x) { return x; }

static inline uint32_t be32toh(uint32_t x) { return __builtin_bswap32(x); }
static inline uint32_t htobe32(uint32_t x) { return __builtin_bswap32(x); }
static inline uint32_t le32toh(uint32_t x) { return x; }
static inline uint32_t htole32(uint32_t x) { return x; }

static inline uint64_t be64toh(uint64_t x) { return __builtin_bswap64(x); }
static inline uint64_t htobe64(uint64_t x) { return __builtin_bswap64(x); }
static inline uint64_t le64toh(uint64_t x) { return x; }
static inline uint64_t htole64(uint64_t x) { return x; }

#endif /* _ENDIAN_H */
