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
        uint32 m_width, m_height;
        
        public:

        Bitmap();
        Bitmap(uint32 width, uint32 height, const PixelFormat& pf, bool displayable = true, bool clear = true);
        ~Bitmap();

        void create(uint32 width, uint32 height, const PixelFormat& pf, bool displayable = true, bool clear = true);
        void destroy();

        void* beginAccess(uint8 plane = 0);
        void endAccess(bool changed);

        const PixelFormat& getPixelFormat() const {
            return m_pixFmt;
        }

        uint32_t getWidth() const {
            return m_width;
        }

        uint32_t getHeight() const {
            return m_height;
        }

        uint32_t getBytesPerLine() const;


    };

}