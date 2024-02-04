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

        template<typename T>
        inline void delete_implementation(T*& mem) {
            delete_implementation(mem, nullptr);
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
        return static_cast< typename remove_reference<T>::Type &&>(x);
    }

    template<typename T>
    void swap(T& x, T& y) {
        T tmp = move(y);
        y = move(x);
        x = move(tmp);
    }

    template<typename T, void(*FDeallocator)(T*) = impl::delete_implementation, void(*FSwap)(T*&,T*&) = swap>
    struct unique_ptr {
        private:
            T* m_ptr;
        public:
            unique_ptr() : m_ptr(nullptr) {}
            unique_ptr(T*&& ptr) noexcept {
                m_ptr = nullptr;
                FSwap(m_ptr, ptr);
            }
            
            unique_ptr(unique_ptr<T>&& s) noexcept {
                m_ptr = nullptr;
                FSwap(m_ptr, s.m_ptr);
            }

            unique_ptr(unique_ptr<T>& s) = delete;
            unique_ptr(const unique_ptr<T>& s) = delete;

            unique_ptr(T*&) = delete;

            ~unique_ptr() {
                release();
            }

            unique_ptr<T>& operator=(unique_ptr<T>& s) = delete;
            unique_ptr<T>& operator=(const unique_ptr<T>& s) = delete;

            unique_ptr<T>& operator=(unique_ptr<T>&& s) noexcept {
                release();
                FSwap(m_ptr, s.m_ptr);
                return *this;
            }

            void release() {
                if (m_ptr) {
                    FDeallocator(m_ptr);
                    m_ptr = nullptr;
                }
            }

            void swap(unique_ptr<T>& s) {
                FSwap(m_ptr, s.m_ptr);
            }

            T* get() {
                return m_ptr;
            }

            const T* get() const {
                return m_ptr;
            }

            T* operator*() {
                return m_ptr;
            }

            const T* operator*() const {
                return m_ptr;
            }

            T* operator->() {
                return m_ptr;
            }

            const T* operator->() const {
                return m_ptr;
            }
    };

    template<typename T>
    void swap(unique_ptr<T>& x, unique_ptr<T>& y) {
        x.swap(y);
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


#define apk_unique_like(ThisClass, FNullFunction, FReleaseFunction, FSwapFunction) \
        ThisClass() { \
            FNullFunction(); \
        } \
        ThisClass(ThisClass&& s) noexcept { \
            FNullFunction(); \
            FSwapFunction(s); \
        } \
        ThisClass(ThisClass& s) = delete; \
        ThisClass(const ThisClass& s) = delete; \
        ~ThisClass() { \
            FReleaseFunction(); \
        }\
        ThisClass& operator=(ThisClass& s) = delete; \
        ThisClass& operator=(const ThisClass& s) = delete; \
        ThisClass& operator=(ThisClass&& s) noexcept { \
            FReleaseFunction(); \
            FSwapFunction(s); \
            return *this; \
        }