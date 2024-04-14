// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include <apk/apk.h>
#include <apk/bitmap.h>
#include <apk/memory.h>
#include <apk/compat.h>

namespace apk {
    

    static bool blitClamp(int32 x, int32 y, int32 dstW, int32 dstH, int32 srcW, int32 srcH, int32& out_x0, int32& out_y0, int32& out_x1, int32& out_y1) {

        out_x0 = MAX<int32>(0, x);
        out_y0 = MAX<int32>(0, y);
        out_x1 = MIN<int32>(out_x0 + srcW, dstW);
        out_y1 = MIN<int32>(out_y0 + srcH, dstH);

        return out_x0 != out_x1 && out_y0 != out_y1;
    }

    static void blitNoTransparentNoShift(uint8* dst, int32 dstX, int32 dstY, int32 dstW, int32 dstH, int32 dstStride, uint8* src, int32 srcW, int32 srcH) {
        int32 x0, y0, x1, y1;
        blitClamp(dstX, dstY, dstW, dstH, srcW, srcH, x0, y0, x1, y1);

        uint32 srcX0 = 0, srcY0 = 0;
        for(int32 j=y0;j < y1;j++) {
			uint32 dstIdx = (j * dstW);
            uint32 srcIdx = (srcY0 * srcW);
            uint32 srcX0 = 0;
			for(int32 i=x0;i < x1;i++) {
				uint8 colour = src[srcIdx + srcX0];
				dst[dstIdx + i] = colour;
                srcX0++;
			}
            srcY0++;
		}
    }

    static void blitNoTransparentShift(uint8* dst, int32 dstX, int32 dstY, int32 dstW, int32 dstH, int32 dstStride, uint8* src, int32 srcW, int32 srcH, int16 shift) {
        int32 x0, y0, x1, y1;
        blitClamp(dstX, dstY, dstW, dstH, srcW, srcH, x0, y0, x1, y1);

        uint32 srcX0 = 0, srcY0 = 0;
        for(int32 j=y0;j < y1;j++) {
			uint32 dstIdx = (j * dstW);
            uint32 srcIdx = (srcY0 * srcW);
            uint32 srcX0 = 0;
			for(int32 i=x0;i < x1;i++) {
				int16 colour = src[srcIdx + srcX0] + shift;
				dst[dstIdx + i] = (colour) & 0xFF;
                srcX0++;
			}
            srcY0++;
		}        
    }

    static void blitTransparentShift(uint8* dst, int32 dstX, int32 dstY, int32 dstW, int32 dstH, int32 dstStride, uint8* src, int32 srcW, int32 srcH, uint8 transparent, int16 shift) {
        int32 x0, y0, x1, y1;
        blitClamp(dstX, dstY, dstW, dstH, srcW, srcH, x0, y0, x1, y1);

        uint32 srcX0 = 0, srcY0 = 0;
        for(int32 j=y0;j < y1;j++) {
			uint32 dstIdx = (j * dstW);
            uint32 srcIdx = (srcY0 * srcW);
            uint32 srcX0 = 0;
			for(int32 i=x0;i < x1;i++) {
				int16 colour = src[srcIdx + srcX0] + shift;
                if (colour != transparent) {
				    dst[dstIdx + i] = (colour) & 0xFF;
                }
                srcX0++;
			}
            srcY0++;
		}           
    }

    static void blitTransparentNoShift(uint8* dst, int32 dstX, int32 dstY, int32 dstW, int32 dstH, int32 dstStride, uint8* src, int32 srcW, int32 srcH, uint8 transparent) {
        int32 x0, y0, x1, y1;
        blitClamp(dstX, dstY, dstW, dstH, srcW, srcH, x0, y0, x1, y1);
        
        uint32 srcX0 = 0, srcY0 = 0;
        for(int32 j=y0;j < y1;j++) {
			uint32 dstIdx = (j * dstW);
            uint32 srcIdx = (srcY0 * srcW);
            uint32 srcX0 = 0;
			for(int32 i=x0;i < x1;i++) {
				uint8 colour = src[srcIdx + srcX0];
                if (colour != transparent) {
				    dst[dstIdx + i] = colour;
                }
                srcX0++;
			}
            srcY0++;
		}         
    }

    BitMap::BitMap(uint16 width, uint16 height, uint16 planes) 
        : m_Width(width), m_Height(height), m_Planes(planes), m_Id(0)
    {
        uint32 size;
        
        if (planes == 0) {
            size = (uint32) width * (uint32) height;
            size += size & 3;
            m_Data = (uint8*) apk_allocate(size);
        }
        else {
            size = (sizeof(uint16) * 4) + ((width / 8) * height) * planes;
            m_Data = (uint8*) apk_allocate_chip(size);
        }
        
    }

    BitMap::~BitMap() {
        if (m_Data) {
            if (m_Planes == 0) {
                apk_deallocate(m_Data);
            }
            else {
                apk_deallocate_chip(m_Data);
            }
            m_Data = NULL;
        }

        m_Width = 0;
        m_Height = 0;
        m_Id = 0;
    }

    void BitMap::copyFrom(const uint8* src, BitMapFormat dstFormat) {
        switch(dstFormat) {
            case BitMapFormat::Chunky:
            {
                apk::memcpy(m_Data, src, m_Width * m_Height);
            }
            break;
            case BitMapFormat::AmigaSprite:
            {
                uint8* dst = m_Data;
                uint32 dstIdx = 0;
                dst[dstIdx++] = 0; // Header (2 * UWORD)
                dst[dstIdx++] = 0;
                dst[dstIdx++] = 0;
                dst[dstIdx++] = 0;

                uint8 bit = 0, bitIdx = 0;

                for(uint16 j=0;j < m_Height;j++) {
                    for(uint8 bitplane=0;bitplane < m_Planes;bitplane++) {
                        for(uint16 i=0;i < m_Width;i++) {
                            uint8 b = (src[i + (j * m_Width)] >> bitplane) & 1;
                            bit <<= 1;
                            bit |= b;
                            bitIdx++;
                            if (bitIdx == 8) {
                                dst[dstIdx++] = bit;
                                bit = 0;
                                bitIdx = 0;
                            }
                        }
                    }
                }

                if (bitIdx != 0) {
                    *dst = bit;
                    dst++;
                    bit = 0;
                    bitIdx = 0;
                }

                dst[dstIdx++] = 0; // Footer (2 * UWORD)
                dst[dstIdx++] = 0;
                dst[dstIdx++] = 0;
                dst[dstIdx++] = 0;
            }
            break;
        }
    }

    void BitMap::blitChunky(uint8* dst, int32 x, int32 y, int32 w, int32 h, int32 stride, uint8 transparent, bool isTransparent, int16 coloursShift) {
        if (isTransparent) {
            if (coloursShift)
                blitTransparentShift(dst, x, y, w, h, stride, m_Data, m_Width, m_Height, transparent, coloursShift);
            else
                blitTransparentNoShift(dst, x, y, w, h, stride, m_Data, m_Width, m_Height, transparent);
        }
        else {
            if (coloursShift)
                blitNoTransparentShift(dst, x, y, w, h, stride, m_Data, m_Width, m_Height, coloursShift);
            else
                blitNoTransparentNoShift(dst, x, y, w, h, stride, m_Data, m_Width, m_Height);
        }
    }

    BitMapBank::BitMapBank() {
    }

    BitMapBank::~BitMapBank() {
        for(uint32 i=0;i < m_BitMaps.size();i++) {
            BitMap* bitMap = m_BitMaps[i];
            apk_delete(bitMap);
        }
        m_BitMaps.clear();
    }

    BitMap* BitMapBank::createBitMap(uint32 id, uint16 w, uint16 h, uint16 planes) {
        BitMap* bitMap = apk_new BitMap(w, h, planes);
        bitMap->setId(id);
        m_BitMaps.push_back(bitMap);
        return bitMap;
    }

    BitMap* BitMapBank::findBitMap(uint32 id) {
        for(uint32 i=0;i < m_BitMaps.size();i++) {
            BitMap* bitMap = m_BitMaps[i];
            if (bitMap->getId() == id) {
                return bitMap;
            }
        }

        return NULL;
    }


    Bob::Bob() {
        m_BitMap = NULL;
        m_OwnedBitMap = false;
        m_X = 0;
        m_Y = 0;
        m_Transparency = 0;
        m_HasTransparency = false;
        m_CenterX = 0;
        m_CenterY = 0;
    }

    Bob::~Bob() {
        if (m_BitMap && m_OwnedBitMap) {
            apk_delete(m_BitMap);
            m_BitMap = NULL;
        }

        m_OwnedBitMap = false;
        m_X = 0;
        m_Y = 0;
        m_Transparency = 0;
        m_HasTransparency = false;
        m_CenterX = 0;
        m_CenterY = 0;
    }
    
    void Bob::setBitMap(BitMap* bitMap, bool own) {
        if (m_BitMap && m_OwnedBitMap) {
            apk_delete(m_BitMap);
        }

        m_BitMap = bitMap;
        m_OwnedBitMap = own;
    }

    void Bob::createBitMap(uint16 w, uint16 h) {
        if (m_BitMap && m_OwnedBitMap) {
            apk_delete(m_BitMap);
        }

        m_BitMap = apk_new BitMap(w, h);
        m_OwnedBitMap = true;
    }

}