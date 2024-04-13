// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "pod.h"
#include "assert.h"
#include "compat.h"

namespace apk {

    template<typename T, ssize_t N>
    class Stack {
        T m_data[N];
        ssize_t m_top;
    public:
        Stack()
            : m_top(-1)
        {
        }

        ~Stack() {
            release();
        }

        Stack(const Stack& stack) {
            copyFrom(stack);
        }

        Stack(Stack&& stack) {
            moveFrom(stack);
        }

        Stack& operator=(const Stack& stack) {
            if (&stack == this) {
                return *this;
            }
            copyFrom(stack);
            return *this;
        }

        Stack& operator=(Stack&& stack) {
            if (&stack == this) {
                return *this;
            }
            moveFrom(stack);
            return *this;
        }

        bool isFull() const {
            return m_top == N;
        }

        bool isEmpty() const {
            return m_top == -1;
        }

        void copyFrom(const Stack& other) {
            release();
            m_top = other.m_top;
            for(uint32_t i=0;i < m_top;i++) {
                new ((void*) &m_data[i]) T(other[i]);
            }
        }

        void moveFrom(Stack& other) {
            release();
            m_top = other.m_top;
            for(uint32_t i=0;i < m_top;i++) {
                new ((void*) &m_data[i]) T(other.m_data[i]);
            }
            other.release();
        }

        void release() {
            if (m_top >= 0) {
                for(uint32_t i=0; i < m_top;i++) {
                    m_data[i].~T();
                }
                m_top = -1;
            }
        }

        void push_back(const T& v) {
            assert(m_top >= -1 && m_top < N)
            m_top++;
            new ((void*) &m_data[m_top]) T(v);
        }

        void pop_back() {
            assert(m_top >= 0)
            m_data[m_top].~T();
            m_top--;
        }

        T& top() {
            assert(m_top >= 0);
            return m_data[m_top];
        }

        const T& top() const {
            assert(m_top >= 0);
            return m_data[m_top];
        }

    };


}
