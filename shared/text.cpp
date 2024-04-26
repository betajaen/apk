// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include <apk/compat.h>

namespace apk { namespace text {

    uint32 string_length(const char* str) {
        uint32 length = 0;
        while(*str++ != '\0') {
            length++;
        }
        return length;
    }

    void string_copy(const char* src, char* dst) {
        while(*src != '\0') {
            *dst++ = *src++;
        }
        *dst = '\0';
    }

    void string_copy_safe(const char* src, char* dst, uint32 dst_length) {
        // TODO: Check bounds of src and dst are within of each other, and fail if they are.
        uint32 src_length = string_length(src);
        uint32 amount_to_copy = dst_length < src_length ? dst_length : src_length;
        while(amount_to_copy > 0) {
            *dst++ = *src++;
            amount_to_copy--;
        }
        *dst = '\0';
    }

    int string_compare(const char* lhs, const char* rhs) {
        const unsigned char *p1 = ( const unsigned char * ) lhs;
        const unsigned char *p2 = ( const unsigned char * ) rhs;

        while ( *p1 && *p1 == *p2 ) ++p1, ++p2;

        return ( *p1 > *p2 ) - ( *p2  > *p1 );
    }

    int string_compare(const char* lhs, const char* rhs, uint32_t length) {
        const unsigned char *p1 = ( const unsigned char * ) lhs;
        const unsigned char *p2 = ( const unsigned char * ) rhs;

        while (length-- && *p1 && *p1 == *p2 ) {
            ++p1, ++p2;
        }
        
        return ( *p1 > *p2 ) - ( *p2  > *p1 );
    }
    
    const char* string_first_of(const char* str, char c) {
        while(*str != '\0') {
            if (*str == c) {
                return str;
            }
            str++;
        }
        return NULL;
    }

    int string_last_of_index(const char* str, char c) {
        int len = string_length(str);
        while(len > -1) {
            if (str[len] == c)
                return len;
            len--;
        }
        return -1;
    }

    char char_to_uppercase(char ch) {
        if (ch >= 97 && ch <= 122) {
            return ch - 32;
        }
        return ch;
    }

}}
