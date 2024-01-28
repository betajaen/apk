// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "pod.h"

namespace apk::endian {

    constexpr int Little = 0;
    constexpr int Big = 1;
    constexpr int NoChange = -1;
    
#if defined(__AMIGA__)
    constexpr int Native = Big;
#else
    constexpr int Native = Little;
#endif

    template<typename T>
    struct unaligned_pod {
        T val;
    } __attribute__((__packed__, __may_alias__));

    template<typename T, int EFrom, int ETo>
    T endian_swap(const T& value);

    template<>  inline uint16 endian_swap<uint16, Little, Little>(const uint16& v) { return v; }
    template<>  inline uint16 endian_swap<uint16, Big, Big>(const uint16& v)       { return v; }
    template<>  inline uint16 endian_swap<uint16, Little, Big>(const uint16& v)    { return __builtin_bswap16(v); }
    template<>  inline uint16 endian_swap<uint16, Big, Little>(const uint16& v)    { return __builtin_bswap16(v); }
    template<>  inline int16  endian_swap<int16, Little, Little>(const int16& v) { return v; }
    template<>  inline int16  endian_swap<int16, Big, Big>(const int16& v)       { return v; }
    template<>  inline int16  endian_swap<int16, Little, Big>(const int16& v)    { return __builtin_bswap16(v); }
    template<>  inline int16  endian_swap<int16, Big, Little>(const int16& v)    { return __builtin_bswap16(v); }

    template<>  inline uint32 endian_swap<uint32, Little, Little>(const uint32& v) { return v; }
    template<>  inline uint32 endian_swap<uint32, Big, Big>(const uint32& v)       { return v; }
    template<>  inline uint32 endian_swap<uint32, Little, Big>(const uint32& v)    { return __builtin_bswap32(v); }
    template<>  inline uint32 endian_swap<uint32, Big, Little>(const uint32& v)    { return __builtin_bswap32(v); }
    template<>  inline int32  endian_swap<int32, Little, Little>(const int32& v)  { return v; }
    template<>  inline int32  endian_swap<int32, Big, Big>(const int32& v)       { return v; }
    template<>  inline int32  endian_swap<int32, Little, Big>(const int32& v)    { return __builtin_bswap32(v); }
    template<>  inline int32  endian_swap<int32, Big, Little>(const int32& v)    { return __builtin_bswap32(v); }



    template<typename T, APK_SIZE_TYPE ESourceEndian, APK_SIZE_TYPE EDestEndian = Native>
    inline static T peek(const void* mem) {
        const endian::unaligned_pod<T>* v = (const endian::unaligned_pod<T>*) mem;
        return endian_swap<T, ESourceEndian, EDestEndian>(v->val);
    }

    template<typename T, APK_SIZE_TYPE ESourceEndian, APK_SIZE_TYPE EDestEndian = Native>
    inline static void poke(void* mem, T value) {
        endian::unaligned_pod<T>* v = (endian::unaligned_pod<T>*) mem;
        v->val = endian_swap<T, ESourceEndian, EDestEndian>(value);
    }

    template<typename T, APK_SIZE_TYPE ESourceEndian, APK_SIZE_TYPE EDestEndian = Native>
    inline static void poke_inplace(void* mem) {
        endian::unaligned_pod<T>* v = (endian::unaligned_pod<T>*) mem;
        v->val = endian_swap<T, ESourceEndian, EDestEndian>(v->val);
    }

    template<typename T, APK_SIZE_TYPE ESourceEndian, APK_SIZE_TYPE EDestEndian = Native>
    inline static T pod(const T& value) {
        return endian_swap<T, ESourceEndian, EDestEndian>(value);
    }

    template<typename T, APK_SIZE_TYPE ESourceEndian, APK_SIZE_TYPE EDestEndian = Native>
    inline static void array(T* array, uint32 length) {
        for(uint32 i=0;i < length;i++) {
            *array = endian_swap<T, ESourceEndian, EDestEndian>(*array);
            array++;
        }
    }

    template<typename T, int ETo = Native>
    inline static T endian_swap_rt(const T& v, int endian) {
        switch(endian) {
            default:
            case NoChange:
                return v;
            case Little:
                return endian_swap<T, Little, ETo>(v);
            case Big:
                return endian_swap<T, Big, ETo>(v);
        }
    }

}
