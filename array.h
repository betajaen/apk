// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "pod.h"
#include "assert.h"
#include "compat.h"

namespace apk {

    void memcpy(void* dst, const void* src, APK_SIZE_TYPE size);

    template<typename T>
    class Array {
        T* m_data;
        uint32 m_size, m_capacity;
    public:
        Array()
            : m_data(NULL), m_size(0), m_capacity(0)
        {
        }

        ~Array() {
            release();
        }

        Array(const Array& ary) {
            copyFrom(ary);
        }

        Array(Array&& ary) {
            moveFrom(ary);
        }

        Array& operator=(const Array& ary) {
            if (&ary == this) {
                return *this;
            }
            copyFrom(ary);
            return *this;
        }

        Array& operator=(Array&& ary) {
            if (&ary == this) {
                return *this;
            }
            moveFrom(ary);
            return *this;
        }

        void copyFrom(const Array& other) {
            release();
            capacity(other.m_size);
            m_size = other.m_size;
            for(uint32_t i=0;i < m_size;i++) {
                new ((void*) &m_data[i]) T(other[i]);
            }
        }

        void moveFrom(Array& other) {
            release();
            m_capacity = other.m_capacity;
            m_size = other.m_size;
            m_data = other.m_data;
            other.m_capacity = 0;
            other.m_size = 0;
            other.m_data = NULL;
        }

        void release() {
            if (m_data) {
                for(uint32_t i=0; i < m_size;i++) {
                    m_data[i].~T();
                }
                apk_deallocate(m_data);
                m_capacity = 0;
                m_size = 0;
                m_data = NULL;
            }
        }

        void clear() {
            for(uint32 i=0; i < m_size;i++) {
                m_data[i].~T();
            }
            m_size = 0;
        }

        void push_back(const T& v) {
            if (m_size == m_capacity) {
                capacity(m_capacity == 0 ? 8 : m_capacity * 2);
            }
            new ((void*) &m_data[m_size]) T(v);
            m_size++;
        }

        void pop_back() {
            assert(m_size > 0);
            m_size--;
            m_data[m_size].~T();
        }

        T& operator[](APK_SIZE_TYPE index) {
            assert(index < m_size);
            return m_data[index];
        }

        const T& operator[](APK_SIZE_TYPE index) const {
            assert(index < m_size);
            return m_data[index];
        }

        void capacity(APK_SIZE_TYPE newCapacity) {
            if (newCapacity == m_capacity)
                return;

            if (newCapacity == 0) {
                release();
                return;
            }

            T* tmp = (T*) apk_allocate(sizeof(T) * newCapacity);

            if (m_size && m_data) {
                APK_SIZE_TYPE amountToCopy = 0;
                if (newCapacity < m_size) {
                    amountToCopy = newCapacity;
                    m_size = amountToCopy;
                }
                else {
                    amountToCopy = m_size;
                }

                for(uint32_t i=0;i < m_size;i++) {
                    new ((void*) &tmp[i]) T(m_data[i]);
                }

            }

            if (m_data) {
                for(uint32_t i=0; i < m_size;i++) {
                    m_data[i].~T();
                }
                apk_deallocate(m_data);
            }

            m_data = tmp;
            m_capacity = newCapacity;
        }

        APK_SIZE_TYPE capacity() const {
            return m_capacity;
        }

        APK_SIZE_TYPE size() const {
            return m_size;
        }
    };


}
