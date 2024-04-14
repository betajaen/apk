// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include <apk/pod.h>
#include <apk/array.h>

namespace apk {

    enum BitMapFormat {
        Chunky,
        AmigaSprite,

#if defined(APK_AMIGA)
        HardwareSprite = AmigaSprite
#else
        HardwareSprite = Chunky
#endif
    };
    
    class BitMap {
        private:
        uint8* m_Data;
        uint16 m_Width, m_Height;
        uint32 m_Id;
        uint16 m_Planes;
        public:

        BitMap(uint16 width, uint16 height, uint16 planes = 0);
        ~BitMap();

        inline uint32 getId() const {
            return m_Id;
        }

        inline void setId(uint32 id) {
            m_Id = id;
        }

        inline uint16 getWidth() const {
            return m_Width;
        }

        inline uint16 getHeight() const {
            return m_Height;
        }

        inline uint16 getPlanes() {
            return m_Planes;
        }

        inline uint8* getData() {
            return m_Data;
        }

        inline const uint8* getData() const {
            return m_Data;
        }

        void copyFrom(const uint8* src, BitMapFormat dstFormat = HardwareSprite);

        void blitChunky(uint8* dst, int32 x, int32 y, int32 w, int32 h, int32 stride, uint8 transparent, bool isTransparent, int16 coloursShift);

    };

    class BitMapBank {
        private:
        Array<BitMap*> m_BitMaps;
        public:
        BitMapBank();
        ~BitMapBank();

        BitMap* createBitMap(uint32 id, uint16 w, uint16 h, uint16 planes);
        BitMap* findBitMap(uint32 id);
    };

    class Bob {
        private:
        BitMap* m_BitMap;
        int16         m_X, m_Y;
        int16         m_CenterX, m_CenterY;
        int16         m_ColourOffset;
        uint8         m_Transparency;
        bool          m_HasTransparency;
        bool          m_OwnedBitMap;
        public:
        Bob();
        ~Bob();

        inline int16 getX() const {
            return m_X;
        }

        inline int16 getY() const {
            return m_Y;
        }

        inline int16 getCenterX() const {
            return m_CenterX;
        }

        inline int16 getCenterY() const {
            return m_CenterY;
        }

        inline void setX(int16 x) {
            m_X = x;
        }

        inline void setY(int16 y) {
            m_Y = y;
        }

        inline void setXY(int16 x, int16 y) {
            m_X = x;
            m_Y = y;
        }

        inline void setCenterX(int16 centerX) {
            m_CenterX = centerX;
        }

        inline void setCenterY(int16 centerY) {
            m_CenterY = centerY;
        }

        inline void setCenterXY(int16 centerX, int16 centerY) {
            m_CenterX = centerX;
            m_CenterY = centerY;
        }

        inline BitMap* getBitMap() const {
            return m_BitMap;
        }

        void setBitMap(BitMap* bitMap, bool own);

        void createBitMap(uint16 w, uint16 h);

        inline bool isTransparent() const {
            return m_HasTransparency;
        }

        inline void setTransparency(bool isTransparent, uint8 colour = 0) {
            m_HasTransparency = isTransparent;
            m_Transparency = colour;
        }

        inline int16 getColourOffset() const {
            return m_ColourOffset;
        }

        inline void setColourOffset(int16 offset) {
            m_ColourOffset = offset;
        }

        inline void blitChunky(uint8* dst, int32 w, int32 h, int32 stride) {
            if (m_BitMap) {
                m_BitMap->blitChunky(dst, m_X + m_CenterX, m_Y + m_CenterY, w, h, stride, m_Transparency, m_HasTransparency, m_ColourOffset);
            }
        }

    };

}