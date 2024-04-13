// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "pod.h"

namespace apk { namespace text {

    uint32 string_length(const char* str);

    void string_copy(const char* src, char* dst);

    void string_copy_safe(const char* src, char* dst, uint32 dst_length);

    int string_compare(const char* lhs, const char* rhs);

    int string_compare(const char* lhs, const char* rhs, uint32_t length);

    const char* string_first_of(const char* str, char needle);

    char char_to_uppercase(char ch);

}}

namespace apk {


    // C stdlib.

    inline int strcmp(const char* lhs, const char* rhs) {
        return text::string_compare(lhs, rhs);
    }

    inline int strncmp(const char* lhs, const char* rhs, uint32 length) {
        return text::string_compare(lhs, rhs, length);
    }

    inline uint32 strlen(const char* str) {
        return text::string_length(str);
    }

    inline void strcpy( char* dst, const char* src) {
        return text::string_copy(src, dst);
    }

    inline void strlcpy(char* dst, const char* src, uint32 length) {
        return text::string_copy_safe(src, dst, length);
    }

    inline void strcpy_s(char* dst, uint32 length, const char* src) {
        return text::string_copy_safe(src, dst, length);
    }

    template<uint32 N>
    inline void strcpy_s(char (&dst)[N], const char *src) {
        return text::string_copy_safe(src, dst, N);
    }

    inline void strncpy(char* dst, const char* src, uint32 length) {
       return text::string_copy_safe(src, dst, length);
    }

    inline const char* strchr(const char* str, char search) {
        return text::string_first_of(str, search);
    }

    inline char* strchr(char* str, char search) {
        return const_cast<char*>(text::string_first_of(const_cast<const char*>(str), search));
    }
    
    inline char toupper(char ch) {
        return text::char_to_uppercase(ch);
    }

    void sprintf_s(char* dst, APK_SIZE_TYPE dst_length, const char* fmt, ...);

    bool string_startswith(const char* str, const char* suffix);
    bool string_endswith(const char* str, const char* suffix);
    uint32 string_to_uint32(const char* str);
}
