// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include "apk/apk.h"

#include <proto/exec.h>

#define MAX_SPRITE_BANKS 4
#define SPRITE_BITPLANES 2

namespace apk {

    namespace bank {


        struct SpriteBank {
            uint16 m_Width;
            uint16 m_Height;
            uint16 m_NumSprites;
            uint32 m_SpriteDataSize;
            /* CHIP */ uint8* m_Data;
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
                    bank->m_SpriteDataSize = (sizeof(UWORD) * 4) + ((width / 8) * height) * SPRITE_BITPLANES;
                    uint32 allocationSize = bank->m_SpriteDataSize * numSprites;
                    bank->m_Data = (uint8*) AllocVec(allocationSize, MEMF_CHIP | MEMF_CLEAR);
                    bank->m_OffsetXY = (int16*) AllocVec(sizeof(int16) * 2 * bank->m_NumSprites, MEMF_CLEAR);

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
                    FreeVec(bank->m_Data);
                    bank->m_Data = NULL;
                    FreeVec(bank->m_OffsetXY);
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
                *outSize = bank->m_SpriteDataSize - (sizeof(UWORD) * 4);
                return bank->m_Data + (bank->m_SpriteDataSize * (uint32) spriteNum) + sizeof(UWORD) * 2;
            }
            
            return NULL;
        }

        void setSpriteBank(int32 bankNum, uint16 spriteNum, uint8* src) {
            uint32 spriteSize;
            void* dst = getSpriteBankImageData(bankNum, spriteNum, &spriteSize);
            if (dst != NULL) {
                CopyMem(src, dst, spriteSize);
            }
        }

        void setSpriteBankChunky(int32 bankNum, uint16 spriteNum, uint8* data, uint32 dataSize, uint8 numBitPlanes) {
            assert(numBitPlanes > 0 && numBitPlanes <= SPRITE_BITPLANES);
            if (bankNum > -1) {
                SpriteBank* bank = &s_SpriteBanks[bankNum];
                uint8* dst = bank->m_Data + (bank->m_SpriteDataSize * (uint32) spriteNum);
                if (dst != NULL) {

                    uint32 dstIdx = 0;
                    dst[dstIdx++] = 0;
                    dst[dstIdx++] = 0;
                    dst[dstIdx++] = 0;
                    dst[dstIdx++] = 0;

                    uint8 bit = 0, bitIdx = 0;

                    for(uint16 j=0;j < bank->m_Height;j++) {
                        for(uint8 bitplane=0;bitplane < numBitPlanes;bitplane++) {
                            for(uint16 i=0;i < bank->m_Width;i++) {
                                uint8 b = (data[i + (j * bank->m_Width)] >> bitplane) & 1;
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

                    dst[dstIdx++] = 0;
                    dst[dstIdx++] = 0;
                    dst[dstIdx++] = 0;
                    dst[dstIdx++] = 0;

                    assert(dstIdx == bank->m_SpriteDataSize);

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
