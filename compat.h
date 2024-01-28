// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "apk/pod.h"

#define _apk_to_str(x) #x
#define apk_to_str(x) _apk_to_str(x)

namespace apk {
    void* _apk_allocate(APK_SIZE_TYPE size, const char* comment);
    void _apk_deallocate(void* mem, const char* comment);

    template<typename T>
    inline void _apk_delete(T*& mem, const char* comment) {
        if (mem) {
            mem->~T();
            _apk_deallocate(mem, comment);
            mem = NULL;
        }
    }
}

void* APK_ATTR_WEAK operator new(APK_SIZE_TYPE size, void* p);
void* APK_ATTR_WEAK operator new(APK_SIZE_TYPE size, const char* comment);

#define apk_new new(__FILE__ ":" apk_to_str(__LINE__))
#define apk_delete(MEM) ::apk::_apk_delete(MEM, __FILE__ ":" apk_to_str(__LINE__))
#define apk_allocate(SIZE) ::apk::_apk_allocate(SIZE, __FILE__ ":" apk_to_str(__LINE__))
#define apk_deallocate(MEM) ::apk::_apk_deallocate(MEM, __FILE__ ":" apk_to_str(__LINE__))

#define apk_no_copy(T) \
    T(const T&) = delete; \
    T& operator=(const T&) = delete;

#define apk_no_move(T) \
    T(T&&) = delete; \
    T& operator=(T&&) = delete;

#define apk_no_copy_move(T) \
    T(const T&) = delete; \
    T& operator=(const T&) = delete; \
    T(T&&) = delete; \
    T& operator=(T&&) = delete;
