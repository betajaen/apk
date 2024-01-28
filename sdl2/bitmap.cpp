// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include "apk/apk.h"
#include "apk/bitmap.h"
#include "apk/compat.h"
#include "apk/sdl2/bitmap_impl.h"

#include <SDL2/SDL.h>

namespace apk {

#define RASTER_SIZE(W, H) ((( (H * (W+15)) >> 3) & 0xFFFE) * 8)

    static uint8* AllocRaster(uint32 w, uint32 h, uint32& outStride) {
        outStride = RASTER_SIZE(w, 1);
        return (uint8*) apk_allocate(RASTER_SIZE(w,h));
    }

    static uint8* AllocChunky(uint32 w, uint32 h, uint32& outStride) {
        outStride = w;
        return (uint8*) apk_allocate(w*h);
    }

    static uint8* AllocComponent(uint32 w, uint32 h, uint32 bpp, uint32& outStride) {
        outStride = w * bpp;
        return (uint8*) apk_allocate(w*h*bpp);
    }


    BitmapImpl::BitmapImpl()
    {
        for(uint32 i=0;i < 8;i++) {
            m_planes[i] = nullptr;
        }

        m_width = 0;
        m_height = 0;
        m_depth = 0;
        m_pf = PixelFormat::None;
        m_cache_image = NULL;
    }

    BitmapImpl::BitmapImpl(uint32 w, uint32 h, const PixelFormat& pf, bool clear, bool displayable)
        : BitmapImpl()
    {
        create(w, h, pf, clear, displayable);
    }

    BitmapImpl::~BitmapImpl() {
        destroy();
    }

    void BitmapImpl::create(uint32 w, uint32 h, const PixelFormat& pf, bool clear, bool displayable) {
        destroy();

        switch(pf) {
            case PixelFormat::None:   
                return;
            case PixelFormat::Planar1:
            case PixelFormat::Planar2:
            case PixelFormat::Planar3:
            case PixelFormat::Planar4:
            case PixelFormat::Planar5:
            case PixelFormat::Planar6:
            case PixelFormat::Planar7:
            case PixelFormat::Planar8:
            {
                m_width = w;
                m_height = h;
                m_depth = ((uint32) pf) - APK_PF_PLANAR_BIT;
                for(uint32 i=0;i < m_depth;i++) {
                    m_planes[i] = AllocRaster(w, h, m_stride);
                }
                m_cache_image = AllocChunky(w, h, m_cache_stride);
            }
            break;
            case PixelFormat::Chunky8:
            {
                m_width = w;
                m_height = h;
                m_depth = 8;
                m_planes[0] = AllocChunky(w, h, m_stride);
                m_cache_image = m_planes[0];
            }
            break;
            case PixelFormat::RGB555:
            case PixelFormat::BGR555: 
            {
                m_width = w;
                m_height = h;
                m_depth = 15;
                m_planes[0] = AllocComponent(w, h, 2, m_stride);
                m_cache_image = m_planes[0];
            }
            break;
            case PixelFormat::RGB565:
            case PixelFormat::BGR565:     
            {
                m_width = w;
                m_height = h;
                m_depth = 16;
                m_planes[0] = AllocComponent(w, h, 2, m_stride);
                m_cache_image = m_planes[0];
            }   
            break; 
            case PixelFormat::RGB24:  
            case PixelFormat::BGR24:  
            {
                m_width = w;
                m_height = h;
                m_depth = 24;
                m_planes[0] = AllocComponent(w, h, 3, m_stride);
                m_cache_image = m_planes[0];
            } 
            break;
            case PixelFormat::RGBA32: 
            case PixelFormat::ARGB32: 
            case PixelFormat::BGRA32: 
            case PixelFormat::ABGR32:
            {
                m_width = w;
                m_height = h;
                m_depth = 32;
                m_planes[0] = AllocComponent(w, h, 4, m_stride);
                m_cache_image = m_planes[0];
            } 
            break;
        }
        commitToCache();
    }

    void BitmapImpl::destroy() {

        if (m_cache_image != m_planes[0] && m_cache_image != nullptr) {
            apk_deallocate(m_cache_image);
            m_cache_image = nullptr;
        }

        for(uint32 i=0;i < 8;i++) {
            if (m_planes[i]) {
                apk_deallocate(m_planes[i]);
                m_planes[i] = nullptr;
            }
        }

        m_width = 0;
        m_height = 0;
        m_depth = 0;
    }

    void* BitmapImpl::beginAccess(uint8 plane) {
        assert(m_planes[plane]);    // At least one plane.
        return m_planes[0];
    }

    void  BitmapImpl::endAccess(bool commit) {
        if (commit) {
            commitToCache();
        }
    }

    void BitmapImpl::commitToCache() {
        switch(m_pf) {
            case PixelFormat::None:
                return;
            case PixelFormat::Planar1:
            case PixelFormat::Planar2:
            case PixelFormat::Planar3:
            case PixelFormat::Planar4:
            case PixelFormat::Planar5:
            case PixelFormat::Planar6:
            case PixelFormat::Planar7:
            case PixelFormat::Planar8:
            {
                uint8* dst = m_cache_image;
                int32 bitPosition = 7;

                for (int y = 0; y < m_height; y++) {
                    for (int x = 0; x < m_width; x++) {
                        uint8_t pixel = 0;
                        uint32 byteIndex = (y * m_stride) + (x >> 3);

                        for (int p = 0; p < m_depth; p++) {
                            if (m_planes[p][byteIndex] & (1 << bitPosition)) {
                                pixel |= (1 << p);
                            }
                        }

                        *dst++ = pixel;
                        bitPosition = (bitPosition - 1) & 7;
                    }
                    if(m_width & 7) {
                        bitPosition = 7;
                    }
                }
            }
            break;
            case PixelFormat::Chunky8:
            case PixelFormat::RGB555:
            case PixelFormat::BGR555:
            case PixelFormat::RGB565:
            case PixelFormat::BGR565:
            case PixelFormat::RGB24:
            case PixelFormat::BGR24:
            case PixelFormat::RGBA32:
            case PixelFormat::ARGB32:
            case PixelFormat::BGRA32:
            case PixelFormat::ABGR32:
            {  
                if (m_planes[0] == m_cache_image)
                    return;
                if (m_cache_stride == m_stride) {
                    memcpy(m_cache_image, m_planes[0], m_stride * m_height);
                }
                else {
                    uint8* dst = m_cache_image;
                    uint8* src = m_planes[0];
                    for(uint32 y=0;y < m_height;y++) {
                        memcpy(dst, src, m_width);
                        dst += m_cache_stride;
                        src += m_stride;
                    }
                }
            }
            break;
        }
    }

    Bitmap::Bitmap() 
        : m_impl(nullptr), m_width(0), m_height(0), m_pixFmt()
    {
    }

    Bitmap::Bitmap(uint32 width, uint32 height, const PixelFormat& pf, bool displayable, bool clear)
        : m_impl(nullptr), m_width(0), m_height(0), m_pixFmt()
    {
        create(width, height, pf, displayable, clear);
    }

    Bitmap::~Bitmap() {
        destroy();
    }

    void Bitmap::create(uint32 width, uint32 height, const PixelFormat& pf, bool displayable, bool clear) {
        destroy();

        m_impl = apk_new BitmapImpl(width, height, pf, displayable, clear);
        assert(m_impl);
        m_width = width;
        m_height = height;
        m_pixFmt = pf;
    }

    void Bitmap::destroy() {
        if (m_impl) {
            apk_delete(m_impl);
            m_impl = nullptr;
        }
        m_width = 0;
        m_height = 0;
        m_pixFmt = PixelFormat();
    }

    void* Bitmap::beginAccess(uint8 plane) {
        assert(plane < 8);
        return m_impl->beginAccess(plane);
    }

    void Bitmap::endAccess(bool commit) {
        m_impl->endAccess(commit);
    }

}
