// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include "apk/apk.h"

#include <proto/exec.h>

typedef VOID(*PUTCHARPROC)();
static const ULONG PutChar = 0x16c04e75;
static const ULONG LenChar = 0x52934e75;

namespace apk {

    void* _apk_allocate(APK_SIZE_TYPE size, const char* comment) {
        size += size & 3; // align up.
        return AllocVec(size, MEMF_CLEAR);
    }

    void _apk_deallocate(void* mem, const char* comment) {
        if (mem != NULL) {
            FreeVec(mem);
        }
    }

    void memcpy(void* dst, const void* src, APK_SIZE_TYPE length) {
        CopyMem(src, dst, length);
    }

    void memcpy_aligned(void* dst, const void* src, APK_SIZE_TYPE length) {
        CopyMemQuick(src, dst, length);
    }

    void memset(void* dst, uint32 val, APK_SIZE_TYPE length) {
        if (dst == NULL) {
            return;
        }

        BYTE* d = (BYTE*) dst;
        BYTE v = (BYTE) val;
        for(APK_SIZE_TYPE i=0;i < length;i++) {
            *d++ = v;
        }
    }

    void memset_aligned(void* dst, uint32 val, APK_SIZE_TYPE length) {
        uint32* d = (uint32*) dst;
        length >>= 2;
        while(length--) {
            *d++ = val;
        }
    }

    void sprintf_s(char* dst, APK_SIZE_TYPE dst_length, const char* fmt, ...) {

		uint32 length = 0;
		const char* arg = (const char*)(&fmt + 1);

		RawDoFmt((CONST_STRPTR)fmt, (APTR)arg, (PUTCHARPROC)&LenChar, &length);

        if (length == 0 || length > dst_length) {
            *dst = '\0';
            return;
		}

        RawDoFmt((CONST_STRPTR)fmt, (APTR)arg, (PUTCHARPROC)&PutChar, dst);
    }

    bool string_startswith(const char* str, const char* prefix) {
        APK_SIZE_TYPE strLen = strlen(str);
        APK_SIZE_TYPE prefixLen = strlen(prefix);

        if (prefixLen > strLen) {
            return false;
        }

        return strncmp(str, prefix, prefixLen) == 0;
    }

    bool string_endswith(const char* str, const char* suffix) {
        APK_SIZE_TYPE strLen = strlen(str);
        APK_SIZE_TYPE suffixLen = strlen(suffix);

        if (suffixLen > strLen) {
            return false;
        }

        return strncmp(str + (strLen - suffixLen), suffix, suffixLen) == 0;
    }

    uint32 string_to_uint32(const char* str) {
        if (str == NULL) {
            return 0;
        }

        if ((str == NULL) || (str[0] == '\0') || (str[0] == '0' && str[1] == '\0')) {
            return 0;
        }

        if (str[0] == '1' && str[1] == '\0') {
            return 1;
        }
        
        uint32 rv = 0;
        while((*str != '\0') && (*str >= '0' && *str <= '9')) {
            rv = rv * 10 + (*str - '0');
            str++;
        }

        return rv;
    }

}