// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include <apk/apk.h>
#include <apk/chunky.h>
#include <apk/memory.h>

namespace apk {
    
    ChunkyBitMap::ChunkyBitMap(uint16 width, uint16 height) 
        : m_Width(width), m_Height(height)
    {
        uint32 size = (uint32) width * (uint32) height;
        size += size & 3;

        m_Data = (uint8*) apk_allocate(size);
    }

    ChunkyBitMap::~ChunkyBitMap() {
        if (m_Data) {
            apk_deallocate(m_Data);
            m_Data = NULL;
        }

        m_Width = 0;
        m_Height = 0;
    }

}