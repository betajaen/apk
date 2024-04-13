// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#if defined (__AMIGA__)

#include <exec/types.h>

namespace apk {
    typedef UBYTE uint8;
    typedef BYTE int8;
    typedef UWORD uint16;
    typedef WORD int16;
    typedef ULONG uint32;
    typedef LONG int32;
    typedef UBYTE byte;
    typedef uint32 uint;

    typedef ULONG uint32_t;
}

#else

#include <stdint.h>
#include <stddef.h>

namespace apk {
    typedef uint8_t uint8;
    typedef uint16_t uint16;
    typedef uint32_t uint32;
    typedef int8_t int8;
    typedef int16_t int16;
    typedef int32_t int32;
    typedef uint8_t byte;
    typedef uint32 uint;

}

#endif


#if defined (__AMIGA__)
typedef unsigned int APK_SIZE_TYPE;
typedef int APK_SSIZE_TYPE;
#else
typedef size_t APK_SIZE_TYPE;
typedef ptrdiff_t APK_SSIZE_TYPE;
#endif

namespace apk {
    typedef APK_SIZE_TYPE size_t;
    typedef APK_SSIZE_TYPE ssize_t;
}


#define APK_ATTR_WEAK __attribute__((weak))
