// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "pod.h"

namespace apk {

    inline uint32 mul_320(uint32 x) {
        return (x << 8) + (x << 6);
    }

    template<typename T>
    inline T min(const T& lhs, const T& rhs) {
        return (lhs < rhs ? lhs : rhs);
    }

    template<typename T1, typename T2>
    inline T1 min(const T1& lhs, const T2& rhs) {
        return (lhs < rhs ? lhs : rhs);
    }

    template<typename T>
    inline T abs(T v) {
        return v < 0 ? -v : v;
    }

    template<typename T>
    inline T max(T lhs, T rhs) {
        return (lhs > rhs) ? lhs : rhs;
    }

    template<typename T>
    inline T clip(T x, T min, T max) {
        if (x < min)
            x = min;
        else if (x > max)
            x = max;
        return x;
    }

    struct Point {
        int16 x, y;

        Point() : x(0), y(0) {}
        Point(int16 X, int16 Y) : x(X), y(Y) {}
    };

    struct Rect {
        int16 top, left, right, bottom;

        Rect() : top(0), left(0), right(0), bottom(0) {}


        Rect(int16 L, int16 T, int16 R, int16 H) : top(L), left(T), right(R), bottom(H) {}
        Rect(int16 R, int16 B) : top(0), left(0), right(R), bottom(B) {}

        bool contains(int16 x, int16 y) const {
            return (left <= x) && (x < right) && (top <= y) && (y < bottom);
        }

        bool isEmpty() const {
            return (left >= right || top >= bottom);
        }

        bool intersects(Rect& rect) {
            return (left < rect.right) && (rect.left < right) && (top < rect.bottom) && (rect.top < bottom);
        }

        int16 width() const {
            return right - left;
        }

        int16 height() const {
            return bottom - top;
        }
    };



}
