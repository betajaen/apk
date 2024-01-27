// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "apk/pod.h"
#include "apk/pixel_format.h"

namespace apk {

    class BitmapImpl;

    class Bitmap {
        BitmapImpl* m_impl;
        PixelFormat m_pixFmt;
        uint32 m_width, m_height, m_dataSize;
        
        public:

        Bitmap();
        Bitmap(uint32 width, uint32 height, const PixelFormat& pf);
        ~Bitmap();

        void* beginAccess();

        void endAccess();

        const PixelFormat& getPixelFormat() const {
            return m_pixFmt;
        }

        uint32_t getWidth() const {
            return m_width;
        }

        uint32_t getHeight() const {
            return m_height;
        }

        uint32_t getBytesPerLine() const {
            return m_pixFmt.bytesPerLine(m_width);
        }


    };

}