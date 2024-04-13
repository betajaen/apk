// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include "apk/apk.h"

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/cybergraphics.h>
#include <proto/graphics.h>
#include <proto/datatypes.h>

#include <intuition/intuition.h>
#include <intuition/screens.h>
#include <cybergraphx/cybergraphics.h>

#include <graphics/gfx.h>
#include <graphics/scale.h>
#include <graphics/displayinfo.h>
#include <intuition/intuition.h>
#include <intuition/intuitionbase.h>

struct Library* CyberGfxBase = NULL;

static const ULONG PutChrProc = 0x16c04e75;  // move.b d0,(a3)+ ; rts


namespace apk {


    namespace video {

		struct Screen* mScreen;
		extern struct Window* mWindow;
		struct ScreenBuffer* mScreenBuffer;
		struct RastPort mRastPort;
        static char sDebugStr[41];

        bool createWindow(struct Screen* screen, uint16 width, uint16 height, uint8 depth);
        void destroyWindow();
        void clearPalette();

        bool createScreen(const char* title, uint16 width, uint16 height, uint8 depth) {

            CyberGfxBase = OpenLibrary("cybergraphics.library", 41);
            if (!CyberGfxBase) {
                requester_okay("Error!", "Cannot open cybergraphics.library V41.");
                return false;
            }

            ULONG modeId = BestCModeIDTags(
                CYBRBIDTG_NominalWidth, (ULONG) width,
                CYBRBIDTG_NominalHeight, (ULONG) height,
                CYBRBIDTG_Depth, (ULONG) depth,
                TAG_DONE
		    );

            if (modeId == INVALID_ID) {
                requester_okay("Error!", "Cannot find appropriate RTG Screen Mode");
                return 1;
            }

            sDebugStr[0] = 0;
            mScreen = OpenScreenTags(NULL,
                SA_DisplayID, modeId,
                SA_Left, 0UL,
                SA_Top, 0UL,
                SA_Width, (ULONG) width,
                SA_Height, (ULONG) height,
                SA_Depth, (ULONG) depth,
                SA_Title, (ULONG) title,
                SA_Type, CUSTOMSCREEN,
                SA_SysFont, 1,
                TAG_DONE
		    );

            mScreenBuffer = AllocScreenBuffer(
                mScreen,
                NULL,
                SB_SCREEN_BITMAP
            );


            if (mScreenBuffer == NULL) {
                requester_okay("Error", "Could open ScreenBuffer for Screen!");
                return false;
            }

            clearPalette();

            InitRastPort(&mRastPort);
            mRastPort.BitMap = mScreenBuffer->sb_BitMap;

            if (createWindow(mScreen, width, height, depth) == false)
                return false;

            return true;
        }

        void destroyScreen() {

            if (mScreenBuffer && mScreen) {
                FreeScreenBuffer(mScreen, mScreenBuffer);
                mScreenBuffer = NULL;
            }

            destroyWindow();

            if (mScreen) {
                CloseScreen(mScreen);
                mScreen = NULL;
            }

            if (CyberGfxBase) {
                CloseLibrary(CyberGfxBase);
            }

        }

        void flipScreen() {
        }

        void writeChunkyPixels(uint8* data) {
            WritePixelArray(data, 0, 0, 320, &mRastPort, 0, 0, 320, 200, RECTFMT_LUT8);
            if (sDebugStr[0] != 0) {
               Move(&mRastPort, 10, 10);
               SetAPen(&mRastPort, 1);
               Text(&mRastPort, sDebugStr, strlen(sDebugStr));
            }
        }

        void writeChunkyPixelsBlit(uint8* data, uint32 x, uint32 y, uint32 w, uint32 h, uint32 stride) {
            WritePixelArray(data, 0, 0, stride, &mRastPort, x, y, w, h, RECTFMT_LUT8);
        }

        void pasteIcon(uint8* img, uint32 x, uint32 y, uint32 w, uint32 h, uint8 transparent, uint8* pal) {
            WritePixelArray(img, 0, 0, w, &mRastPort, x, y, w, h, RECTFMT_LUT8);
        }

        void forceUpdateScreen() {
            /* Not needed on RTG */
        }

        void setDebugNum(uint32 num) {
            if (num == 0) {
                sDebugStr[0] = 0;
            }
            else {
                RawDoFmt("%lu", &num, (void(*)()) &PutChrProc, sDebugStr);
            }
        }

        void setDebugStr(const char* str) {
            if (str == NULL) {
                sDebugStr[0] = 0;
            }
            else {
                uint32 len = strlen(str);
                if (len > sizeof(sDebugStr)-1)
                    len = sizeof(sDebugStr)-1;
                CopyMem(str, sDebugStr, len);
                sDebugStr[len] = 0;
            }
        }

        void clearDebug() {
            sDebugStr[0] = 0;
        }


    }


}
