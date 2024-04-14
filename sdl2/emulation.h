// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include <apk/pod.h>
#include <apk/bitmap.h>
#include <apk/array.h>

namespace apk {

    class EmulatedSpriteHardware {
        private:
            Bob* m_Bobs[8];
            bool m_BobVisible[8];
        public:

            EmulatedSpriteHardware();
            ~EmulatedSpriteHardware();

            Bob* getSprite(uint32 index) {
                if (index > 8) {
                    return NULL;
                }
                return m_Bobs[index];
            }

            void setVisible(uint32 index, bool isVisible) {
                if (index < 8) {
                    m_BobVisible[index] = isVisible;
                }
            }
            
            bool getVisible(uint32 index) {
                if (index < 8) {
                    return m_BobVisible[index];
                }
                return false;
            }

            void blitChunky(uint8* dst, uint32 width, uint32 height, uint32 stride);
    };

}