// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include "apk/apk.h"

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <proto/datatypes.h>

#include <intuition/intuition.h>
#include <intuition/screens.h>

#include <graphics/gfx.h>
#include <graphics/scale.h>
#include <graphics/displayinfo.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>

namespace apk { namespace video {

		extern struct Screen* mScreen;
		extern struct Window* mWindow;
		extern struct ScreenBuffer* mScreenBuffer;
		extern struct RastPort mRastPort;
    
        static ULONG sPalette[2 + (256 * 3)] = { 0 };
        static ULONG sFadePalette[2 + (256 * 3)] = { 0 };
        static int32 sPaletteFading;
        static int32 sPaletteFadeSteps;
        static int32 sPaletteFadeTime;
        static int32 sPaletteFadeDest;
        static bool  sPaletteDirty = false;

        void writeRect(uint32 l, uint32 t, uint32 r, uint32 b, uint8 col) {
          SetAPen(&mRastPort, col);

          Move(&mRastPort, l, t);
          Draw(&mRastPort, r, t);
          Draw(&mRastPort, r, b);
          Draw(&mRastPort, l, b);
          Draw(&mRastPort, l, t);
        }

        void clearChunkyPixels(uint8 index) {
            SetRast(&mRastPort, index);
        }

        void writePixel(uint32 x, uint32 y, uint8 col) {
            SetAPen(&mRastPort, col);
            WritePixel(&mRastPort, x, y);
        }

        void setRGB(uint8 index, uint8 r, uint8 g, uint8 b) {
            ULONG* dst = &sPalette[1 + index*3];
            *dst++ = r << 24 | 0x00FFffFF;
            *dst++ = g << 24 | 0x00FFffFF;
            *dst++ = b << 24 | 0x00FFffFF;

            sPaletteDirty = TRUE;
        }

        void setRGB(uint8* pal, uint32 begin, uint32 end) {
            ULONG* dst = &sPalette[1 + begin*3];
            for(uint32 i=begin;i < end;i++) {
                *dst++ = *pal++ << 24 | 0x00FFffFF;
                *dst++ = *pal++ << 24 | 0x00FFffFF;
                *dst++ = *pal++ << 24 | 0x00FFffFF;
            }

            sPaletteDirty = TRUE;
        }

        void clearPalette() {

            memset(sPalette, 0, sizeof(sPalette));

            sPalette[0] = 256L << 16 | 0;
            sPalette[4] = 0xFFffffff;
            sPalette[5] = 0xFFffffff;
            sPalette[6] = 0xFFffffff;

            sPaletteDirty = TRUE;
        }

        void paletteFadeIn(uint32 steps) {
            if (sPaletteFading == 0) {
                sPaletteFadeSteps = (int32) CLIP((int32)steps, (int32)1, (int32)255);
                sPaletteFadeTime = -255;
                sPaletteFadeDest = 0;
                sPaletteFading = 1;
            }
        }

        void paletteFadeOut(uint32 steps) {
            if (sPaletteFading == 0) {
                sPaletteFadeSteps = -(int32) CLIP((int32)steps, (int32)1, (int32)255);
                sPaletteFadeTime = 255;
                sPaletteFadeDest = 0;
                sPaletteFading = -1;
            }
        }

        void paletteFunction() {

            if (sPaletteFading == 0) {
                if (sPaletteDirty) {
                    sPaletteDirty = FALSE;
                    LoadRGB32(&mScreen->ViewPort, sPalette);
                }
            }
            else {
                sPaletteDirty = FALSE;

                ULONG* src = sPalette;
                ULONG* dst = sFadePalette;

                *dst = *src;
                src++;
                dst++;

                for(uint32 i=0;i < 256*3;i++) {
                    int32 col = (int32) ((*src) >> 24Ul);
                    col += sPaletteFadeTime;
                    if (col < 0) {
                        col = 0;
                    }
                    else if (col > 255) {
                        col = 255;
                    }
                    *dst = (((uint32)col) << 24UL) | 0x00FFffFF;
                    src++;
                    dst++;
                }
                
                sPaletteFadeTime += sPaletteFadeSteps;

                if (sPaletteFading == -1) {
                    if (sPaletteFadeTime > sPaletteFadeDest) {
                        LoadRGB32(&mScreen->ViewPort, sFadePalette);
                        return;
                    }
                }
                else {
                    if (sPaletteFadeTime < sPaletteFadeDest) {
                        LoadRGB32(&mScreen->ViewPort, sFadePalette);
                        return;
                    }
                }

                sPaletteFading = 0;
                LoadRGB32(&mScreen->ViewPort, sPalette);

            }

        }

}}
