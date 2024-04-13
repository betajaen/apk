// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "pod.h"
#include "assert.h"
#include "compat.h"

namespace apk {

    template<typename T, ssize_t N>
    class Pool {
        T m_data[N];
    public:
        Pool()
        {
            memset(m_data, 0, sizeof(m_data));
            
            for(uint32 i=0;i < N;i++) {
                m_data[i].pool_index = -1;
            }
        }

        ~Pool() {
            release();
        }

        Pool(const Pool& pool) {
            copyFrom(pool);
        }

        Pool(Pool&& pool) {
            moveFrom(pool);
        }

        Pool& operator=(const Pool& pool) {
            if (&pool == this) {
                return *this;
            }
            copyFrom(pool);
            return *this;
        }

        Pool& operator=(Pool&& pool) {
            if (&pool == this) {
                return *this;
            }
            moveFrom(pool);
            return *this;
        }

        void copyFrom(const Pool& other) {
            release();
            for(uint32_t i=0;i < N;i++) {
                new ((void*) &m_data[i]) T(other[i]);
            }
        }

        void moveFrom(Pool& other) {
            release();
            for(uint32_t i=0;i < N;i++) {
                new ((void*) &m_data[i]) T(other.m_data[i]);
            }
            other.release();
        }

        void release() {
            for(uint32_t i=0; i < N;i++) {
                m_data[i].~T();
            }
        }

        T* create() {
            for(uint32 i=0; i < N;i++) {
                T* item = &m_data[i];
                if (item->pool_index == -1) {
                    item->pool_index = i;
                    return item;
                }
            }

            T* item = apk_new T();
            item->pool_index = -2;
            return item;
        }

        void destroy(T* item) {
            if (item->pool_index == -2) {
                apk_delete(item);
                return;
            }

            apk::memset(item, 0, sizeof(T));
            item->pool_index = -1;
        }
        
        void destroyQuick(T* item) {
            if (item->pool_index == -2) {
                apk_delete(item);
                return;
            }

            item->pool_index = -1;
        }


    };


}
