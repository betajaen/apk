// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include <apk/sdl2/emulation.h>
#include <stdio.h>

namespace apk {

    EmulatedSpriteHardware::EmulatedSpriteHardware() {
        for(uint32 i=0;i < 8;i++) {
            m_Bobs[i] = apk_new Bob();
            m_Bobs[i]->setColourOffset(16 + i * 4);
            m_Bobs[i]->setTransparency(true, 16 + i * 4);
            m_BobVisible[i] = false;
        }
    }

    EmulatedSpriteHardware::~EmulatedSpriteHardware() {
        for(uint32 i=0;i < 8;i++) {
            apk_delete(m_Bobs[i]);
            m_BobVisible[i] = false;
        }
    }

    void EmulatedSpriteHardware::blitChunky(uint8* dst, uint32 width, uint32 height, uint32 stride) {
        for(uint32 i=0;i < 8;i++) {
            if (m_BobVisible[i]) {
                m_Bobs[i]->blitChunky(dst, width, height, stride);
            }
        }
    }

}