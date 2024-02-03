// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "apk/pod.h"

#define _apk_to_str(x) #x
#define apk_to_str(x) _apk_to_str(x)

namespace apk {
    void* heap_allocate(APK_SIZE_TYPE size, const char* comment);
    void heap_deallocate(void* mem, const char* comment);

    namespace impl {

        template<typename T>
        inline void delete_implementation(T*& mem, const char* comment) {
            if (mem) {
                mem->~T();
                heap_deallocate(mem, comment);
                mem = NULL;
            }
        }

    }
    
    template<typename T>
    struct remove_pointer {
        typedef T Type;
    };

    template<typename T>
    struct remove_pointer<T*> {
        typedef T Type;
    };

    template<typename T>
    struct remove_pointer<T**> {
        typedef T Type;
    };

    template<typename T>
    struct remove_reference {
        typedef T Type;
    };

    template<typename T>
    struct remove_reference<T&> {
        typedef T Type;
    };

    template<typename T>
    struct remove_reference<T&&> {
        typedef T Type;
    };

    template<typename T>
    typename remove_reference<T>::Type&& move(T&& x) noexcept {
        return static_cast<typename remove_reference<T>::type&&>(x);
    }

    template<typename T>
    void swap(T& x, T& y) {
        T tmp = move(y);
        y = move(x);
        x = move(tmp);
    }


}

void* APK_ATTR_WEAK operator new(APK_SIZE_TYPE size, void* p);
void* APK_ATTR_WEAK operator new(APK_SIZE_TYPE size, const char* comment);

#define apk_new new(__FILE__ ":" apk_to_str(__LINE__))
#define apk_delete(MEM) ::apk::impl::delete_implementation(MEM, __FILE__ ":" apk_to_str(__LINE__))
#define apk_allocate(SIZE) ::apk::heap_allocate(SIZE, __FILE__ ":" apk_to_str(__LINE__))
#define apk_deallocate(MEM) ::apk::heap_deallocate(MEM, __FILE__ ":" apk_to_str(__LINE__))

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
