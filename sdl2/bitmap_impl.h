// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "apk/apk.h"

typedef struct SDL_Surface SDL_Surface;

namespace apk {

    enum class PixelFormat : uint32;

    class BitmapImpl {
        public:
            uint8*       m_planes[8];
            uint8*       m_cache_image;
            uint32       m_width, m_height, m_depth, m_stride, m_cache_stride;
            PixelFormat  m_pf;

            BitmapImpl();
            BitmapImpl(uint32 w, uint32 h, const PixelFormat& pf, bool clear, bool displayable);
            ~BitmapImpl();

            void create(uint32 w, uint32 h, const PixelFormat& pf, bool clear, bool displayable);
            void destroy();

            void* beginAccess(uint8 plane = 0);
            void  endAccess(bool commitChanges);
        private:
            void  commitToCache();
    };

}