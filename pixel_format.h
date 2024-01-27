// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include <apk/pod.h>

namespace apk {
    
    struct PixelFormat {

        enum class Storage : uint8 {
            None,
            Planar,
            Interleaved,
            Chunky
        };

        enum class Colour : uint8 {
            None,
            LUT,
            RGB24,
            RGB32
        };

        enum Flags {
            Flag_EHB
        };

        Storage  m_storage;
        Colour   m_colour;
        uint8    m_numPlanes;
        uint8    m_flags;

        constexpr PixelFormat()
            : m_storage(Storage::None), m_colour(Colour::None), m_numPlanes(0), m_flags(0)
        {
        }

        constexpr PixelFormat(Storage s, Colour c, uint8 p = 0, uint8 f = 0) 
            : m_storage(s), m_colour(c), m_numPlanes(p), m_flags(f)
        {
        }

        constexpr bool isNone() const {
            return m_storage == Storage::None && m_colour == Colour::None && m_numPlanes == 0 && m_flags == 0;
        }

        constexpr bool equals(const PixelFormat& other) const {
            return  m_storage == other.m_storage &&
                    m_colour  == other.m_colour &&
                    m_numPlanes == other.m_numPlanes &&
                    m_flags == other.m_flags;
        }

        uint32_t bytesPerLine(uint32 width) const;
    };

    static_assert(sizeof(PixelFormat) == 4);

    namespace pixelformats {
        constexpr PixelFormat Chunky8 = PixelFormat(PixelFormat::Storage::Chunky, PixelFormat::Colour::LUT);
        constexpr PixelFormat Planar1 = PixelFormat(PixelFormat::Storage::Planar, PixelFormat::Colour::LUT, 1); 
        constexpr PixelFormat Planar2 = PixelFormat(PixelFormat::Storage::Planar, PixelFormat::Colour::LUT, 2); 
        constexpr PixelFormat Planar3 = PixelFormat(PixelFormat::Storage::Planar, PixelFormat::Colour::LUT, 3); 
        constexpr PixelFormat Planar4 = PixelFormat(PixelFormat::Storage::Planar, PixelFormat::Colour::LUT, 4); 
        constexpr PixelFormat Planar5 = PixelFormat(PixelFormat::Storage::Planar, PixelFormat::Colour::LUT, 5); 
        constexpr PixelFormat Planar6 = PixelFormat(PixelFormat::Storage::Planar, PixelFormat::Colour::LUT, 6); 
        constexpr PixelFormat Planar7 = PixelFormat(PixelFormat::Storage::Planar, PixelFormat::Colour::LUT, 7); 
        constexpr PixelFormat Planar8 = PixelFormat(PixelFormat::Storage::Planar, PixelFormat::Colour::LUT, 8); 

    }

}
