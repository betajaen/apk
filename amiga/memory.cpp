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

    const char* strchr(const char* str, char c) {
        while(*str != '\0') {
            if (*str == c) {
                return str;
            }
            str++;
        }
        return NULL;
    }

    const char* strrchr(const char* str, char c) {
        debug("strrchr not implemented");
        return NULL;
    }

    char* strchr(char* str, char c) {
        while(*str != '\0') {
            if (*str == c) {
                return str;
            }
            str++;
        }
        return NULL;
    }

    char* strrchr(char* str, char c) {
        debug("strrchr not implemented");
        return NULL;
    }

    int strcmp(const char* lhs, const char* rhs) {

        const unsigned char *p1 = ( const unsigned char * ) lhs;
        const unsigned char *p2 = ( const unsigned char * ) rhs;

        while ( *p1 && *p1 == *p2 ) ++p1, ++p2;

        return ( *p1 > *p2 ) - ( *p2  > *p1 );
    }

    int strncmp(const char* lhs, const char* rhs, APK_SIZE_TYPE rhsLength) {

        const unsigned char *p1 = ( const unsigned char * ) lhs;
        const unsigned char *p2 = ( const unsigned char * ) rhs;

        while ( *p1 && *p1 == *p2 && rhsLength) ++p1, ++p2, --rhsLength;

        return ( *p1 > *p2 ) - ( *p2  > *p1 );
    }

    uint32 strlen(const char* str) {
        uint32 length = 0;
        while(*str++ != '\0') {
            length++;
        }
        return length;
    }

    void strcpy(char* dst, const char* src) {
        while(*src != '\0') {
            *dst++ = *src++;
        }
        *dst = '\0';
    }

    void strlcpy(char* dst, const char* src, uint32 length) {
        uint32 src_length = strlen(src);
        uint32 amount_to_copy = length < src_length ? length : src_length;
        while(amount_to_copy > 0) {
            *dst++ = *src++;
            amount_to_copy--;
        }
        *dst = '\0';
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

    char toupper(char c) {
        if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 'A';
        }
        return c;
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

}