// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "pod.h"

namespace apk {


    int strcmp(const char* lhs, const char* rhs);


    uint32 strlen(const char* str);
    void strcpy( char* dst, const char* src);
    void strlcpy(char* dst, const char* src, uint32 length);

    inline void strcpy_s(char* dst, uint32 length, const char* src) {
        strlcpy(dst, src, length);
    }

    template<uint32 N>
    inline void strcpy_s(char (&dst)[N], const char *src) {
        strlcpy(dst, src, N);
    }

    inline void strncpy(char* dst, const char* src, uint32 length) {
        strlcpy(dst, src, length);
    }

    const char* strchr(const char* str, char search);
    const char* strrchr(const char* str, char search);

    char* strchr(char* str, char search);
    char* strrchr(char* str, char search);

    char toupper(char);

    void sprintf_s(char* dst, APK_SIZE_TYPE dst_length, const char* fmt, ...);

    bool string_startswith(const char* str, const char* suffix);
    bool string_endswith(const char* str, const char* suffix);
}
