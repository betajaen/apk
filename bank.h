// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "apk/pod.h"

namespace apk {

    namespace bank {

        int32 createSpriteBank(uint16 width, uint16 height, uint16 numSprites);
        void destroySpriteBank(int32 bank);

        void setSpriteBank(int32 bank, uint16 spriteNum, uint8* data);
        void setSpriteBankChunky(int32 bank, uint16 spriteNum, uint8* data, uint32 dataSize, uint8 numBitPlanes);
        void setSpriteOffset(int32 bankNum, uint16 spriteNum, int16 offsetX, int16 offsetY);

        void* getSpriteBankImageData(int32 bank, uint16 spriteNum, uint32* outSize);

    }

}