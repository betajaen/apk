// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include "apk/apk.h"

#define MAX_SPRITE_BANKS 4
#define SPRITE_BITPLANES 2

namespace apk {

    namespace bank {


        struct SpriteBank {
            uint16 m_Width;
            uint16 m_Height;
            uint16 m_NumSprites;
            uint32 m_SpriteDataSize;
            uint8* m_Data;
            int16* m_OffsetXY;
        };

        SpriteBank s_SpriteBanks[MAX_SPRITE_BANKS] = { 0 };


        int32 createSpriteBank(uint16 width, uint16 height, uint16 numSprites) {
            assert(width > 0 && width <= 16);
            assert(height > 0);
            numSprites = max((uint16) 1, numSprites);
            SpriteBank* bank = &s_SpriteBanks[0];
            for(uint16 i=0;i < MAX_SPRITE_BANKS;i++) {
                if (bank->m_Width == 0) {
                    bank->m_Width = width;
                    bank->m_Height = height;
                    bank->m_NumSprites = numSprites;
                    bank->m_SpriteDataSize = (width * height);
                    uint32 allocationSize = bank->m_SpriteDataSize * numSprites;
                    bank->m_Data = (uint8*) apk_allocate(allocationSize);
                    bank->m_OffsetXY = (int16*) apk_allocate(sizeof(int16) * 2 * bank->m_NumSprites);

                    return i;
                }
            }
            return -1;
        }

        void destroySpriteBank(int32 bankNum) {
            if (bankNum > -1) {
                assert(bankNum < MAX_SPRITE_BANKS);
                SpriteBank* bank = &s_SpriteBanks[bankNum];
                if (bank->m_Width != 0) {
                    bank->m_Width = 0;
                    bank->m_Height = 0;
                    bank->m_NumSprites = 0;
                    bank->m_SpriteDataSize = 0;
                    apk_deallocate(bank->m_Data);
                    bank->m_Data = NULL;
                    apk_deallocate(bank->m_OffsetXY);
                    bank->m_OffsetXY = NULL;
                }
            }
        }

        void* getSpriteBankData(int32 bankNum, uint16 spriteNum, uint32* outSize, uint16* outWidth, uint16* outHeight, int16* outOffsetX, int16* outOffsetY) {
            if (bankNum > -1) {
                assert(bankNum < MAX_SPRITE_BANKS);
                SpriteBank* bank = &s_SpriteBanks[bankNum];
                if (bank->m_Width == 0) {
                    return NULL;
                }

                spriteNum = min(spriteNum, bank->m_NumSprites - 1);
                *outSize = bank->m_SpriteDataSize;
                *outWidth = bank->m_Width;
                *outHeight = bank->m_Height;
                *outOffsetX = bank->m_OffsetXY[spriteNum * 2 + 0];
                *outOffsetY = bank->m_OffsetXY[spriteNum * 2 + 1];
                return bank->m_Data + (bank->m_SpriteDataSize * (uint32) spriteNum);
            }
            
            return NULL;
        }

        void* getSpriteBankImageData(int32 bankNum, uint16 spriteNum, uint32* outSize) {
            if (bankNum > -1) {
                assert(bankNum < MAX_SPRITE_BANKS);
                SpriteBank* bank = &s_SpriteBanks[bankNum];
                if (bank->m_Width == 0) {
                    return NULL;
                }

                spriteNum = min(spriteNum, bank->m_NumSprites - 1);
                *outSize = bank->m_SpriteDataSize;
                return bank->m_Data + (bank->m_SpriteDataSize * (uint32) spriteNum);
            }
            
            return NULL;
        }

        void setSpriteBank(int32 bankNum, uint16 spriteNum, uint8* src) {
            // TODO:
        }

        void setSpriteBankChunky(int32 bankNum, uint16 spriteNum, uint8* data, uint32 dataSize, uint8 numBitPlanes) {
            assert(numBitPlanes > 0 && numBitPlanes <= SPRITE_BITPLANES);
            if (bankNum > -1) {
                SpriteBank* bank = &s_SpriteBanks[bankNum];
                assert(dataSize == bank->m_SpriteDataSize);
                uint8* dst = bank->m_Data + (bank->m_SpriteDataSize * (uint32) spriteNum);
                if (dst != NULL) {
                    apk::memcpy(dst, data, dataSize);

                }
                return;    
            }        
        }

        void setSpriteOffset(int32 bankNum, uint16 spriteNum, int16 offsetX, int16 offsetY) {
            if (bankNum > -1) {
                assert(bankNum < MAX_SPRITE_BANKS);
                SpriteBank* bank = &s_SpriteBanks[bankNum];
                if (bank->m_Width == 0) {
                    return;
                }

                bank->m_OffsetXY[spriteNum * 2 + 0] = offsetX;
                bank->m_OffsetXY[spriteNum * 2 + 1] = offsetY;
            }
        }

    }

}
