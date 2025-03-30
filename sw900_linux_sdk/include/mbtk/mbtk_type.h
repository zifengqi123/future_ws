#ifndef MBTK_TYPE_INCLUDE
#define MBTK_TYPE_INCLUDE
#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>

#ifndef MBTK_PLATFORM_LINUX
#define MBTK_PLATFORM_LINUX
#endif

#ifndef MBTK_PLATFORM_QCOMM
//#define MBTK_PLATFORM_QCOMM
#endif

#define MBTK_SUCCESS 0
#define MBTK_FAILE (-1)

#ifndef UNUSED
#define UNUSED(param) ((void) param)
#define UNUSEDPARAM UNUSED
#endif

#ifndef TRUE
#define TRUE   1   /* Boolean true value. */
#endif

#ifndef true
#define true   1   /* Boolean true value. */
#endif

#ifndef FALSE
#define FALSE  0   /* Boolean false value. */
#endif

#ifndef false
#define false  0   /* Boolean false value. */
#endif


#ifndef NULL
#define NULL  0
#endif


#ifndef null
#define null  0
#endif

/**
 * Compiler-digit : 16
 * char : 1     (%c)
 * char* : 2
 * short int : 2
 * int : 2      (%d)
 * unsigned int : 2  (%u)
 * float : 4    (%f)
 * double : 8   (%f)
 * long : 4
 * unsigned long : 4
 * long long : 8
 * unsigned long long : 8
 *
 *
 * Compiler-digit : 32
 * char : 1
 * char* : 4
 * short int : 2
 * int : 4
 * unsigned int : 4
 * float : 4
 * double : 8
 * long : 4
 * unsigned long : 4
 * long long : 8
 * unsigned long long : 8
 *
 *
 * Compiler-digit : 64
 * char : 1
 * char* : 8
 * short int : 2
 * int : 4
 * unsigned int : 4
 * float : 4
 * double : 8
 * long : 8
 * unsigned long : 8
 * long long : 8
 * unsigned long long : 8
 */
typedef unsigned char boolean; /* Boolean value type. */
// typedef unsigned char bool; /* Boolean value type. */
typedef unsigned long long uint64; /* Unsigned 64 bit value */
typedef unsigned long long uint64_t; /* Unsigned 64 bit value */
typedef unsigned int uint32; /* Unsigned 32 bit value */
typedef unsigned int uint32_t; /* Unsigned 32 bit value */
typedef unsigned short uint16; /* Unsigned 16 bit value */
typedef unsigned short uint16_t;
typedef unsigned char uint8; /* Unsigned 8  bit value */
typedef unsigned char uint8_t;
typedef signed long long int64; /* Signed 64 bit value */
typedef signed long long sint64; /* Signed 64 bit value */
typedef signed int int32; /* Signed 32 bit value */
typedef signed int sint32; /* Signed 32 bit value */
typedef signed short int16; /* Signed 16 bit value */
typedef signed short sint16; /* Signed 16 bit value */
typedef signed char int8; /* Signed 8  bit value */
typedef signed char sint8; /* Signed 8  bit value */
typedef unsigned char byte; /* byte type */


typedef struct
{
    char *buffer;
    int size;
    int size_max;
} mbtk_buffer_t;


#endif /* MBTK_TYPE_INCLUDE */
