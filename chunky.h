// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include <apk/pod.h>

namespace apk {
    
    struct ChunkyBitMap {
        private:
        uint8* m_Data;
        uint16 m_Width, m_Height;
        public:

        ChunkyBitMap(uint16 width, uint16 height);
        ~ChunkyBitMap();

        uint16 getWidth() const {
            return m_Width;
        }

        uint16 getHeight() const {
            return m_Height;
        }

        uint8* getData() {
            return m_Data;
        }

        const uint8* getData() const {
            return m_Data;
        }

    };

    struct ChunkyBob {
        private:
        ChunkyBitMap* m_BitMap;
        int16         m_X, m_Y;
        int16         m_CenterX, m_CenterY;
        public:
        ChunkyBob();
        ~ChunkyBob();

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

        inline ChunkyBitMap* getChunkyBitMap() const {
            return m_BitMap;
        }

        inline void setChunkyBitMap(ChunkyBitMap* bitMap) {
            m_BitMap = bitMap;
        }

    };

}