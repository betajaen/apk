// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.


#include "apk/apk.h"
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <ctype.h>

namespace apk {

#if 1

    struct Header;
    struct Footer;

    struct Header {
        uint32  magic;
        uint32  size;
        const char* comment;
        Footer* footer;
    };

    struct Footer {
        Header* header;
    };

    static const uint32 kMagic = 'A' << 24 | 'P' << 16 | 'K' << 8 | 'A';

    static void* _allocMem(uint32 size, const char* comment) {
        uint32 totalSize = sizeof(Header) + sizeof(Footer) + size + (sizeof(void*) == 8 ? (size & 7) : (size & 3));
        uint8* bytes = (uint8*) ::malloc(totalSize);
        assert(bytes);
        ::memset(bytes, 0, totalSize);
        Header* header = (Header*) bytes;
        header->magic = kMagic;
        header->comment = comment;
        header->size = totalSize;
        header->footer = (Footer*) (bytes + sizeof(Header) + size);
        header->footer->header = header;
        /// ::printf("[MEM] Allocate %s (%ld)\n", comment, totalSize);
        return (void*) (bytes + sizeof(Header));
    }

    static void _freeMem(void* mem, const char* comment) {
        // ::printf("[MEM] Free from %s (%p)\n", comment);
        assert(mem);
        Header* header = (((Header*) mem) - 1);
        // ::printf("[MEM] Free %s (%ld)\n", header->comment, header->size);
        assert(header->magic == kMagic);
        assert(header->footer->header == header);
        byte* b = (byte*) header;
        memset(b, 0xCD, header->size);
        ::free(header);
    }

#else

    void* _allocMem(uint32 size, const char* comment) {
        uint8* bytes = (uint8*) ::malloc(size);
        ::memset(bytes, 0, size);
        return (void*) bytes;
    }

    void _freeMem(void* mem, const char* comment) {
        ::free(mem);
    }
#endif

    void* _apk_allocate(APK_SIZE_TYPE size, const char* comment) {
        return _allocMem(size, comment);
    }

    void _apk_deallocate(void* mem, const char* comment) {
        if (mem) {
            _freeMem(mem,  comment);
        }
    }
    
    void* apk_allocate_chip(APK_SIZE_TYPE size) {
        return _allocMem(size, "CHIP");
    }

    void apk_deallocate_chip(void* mem) {
        if (mem) {
            _freeMem(mem, "CHIP");
        }
    }

    void memcpy(void* dst, const void* src, APK_SIZE_TYPE length) {
        ::memcpy(dst, src, length);
    }

    void memcpy_aligned(void* dst, const void* src, APK_SIZE_TYPE length) {
        ::memcpy(dst, src, length);
    }

    void memset(void* dst, uint32 val, APK_SIZE_TYPE length) {
        ::memset(dst, val, length);
    }

    void memset_aligned(void* dst, uint32 val, APK_SIZE_TYPE length) {
        ::memset(dst, val, length);
    }

    void sprintf_s(char* dst, APK_SIZE_TYPE dst_length, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ::vsnprintf(dst, dst_length, fmt, args);
        va_end(args);
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
