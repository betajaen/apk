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

#include <exec/types.h>
#include <exec/ports.h>

#include <proto/timer.h>
#include <inline/timer.h>


struct Device* TimerBase;

#include <clib/exec_protos.h>
#include <clib/alib_protos.h>

__chip UWORD s_Cursor[] = {
    0x0000, 0x0000,

    0x0000, 0x0000,
    0x0000, 0x0000,
    0x0000, 0x0000,
    0x0000, 0x0000,
    0x0000, 0x0000,
    0x0000, 0x0000,
    0x0000, 0x0000,
    0x0000, 0x0000,
    0x0000, 0x0000,
    0x0000, 0x0000,
    0x0000, 0x0000,
    0x0000, 0x0000,
    0x0000, 0x0000,
    0x0000, 0x0000,
    0x0000, 0x0000,
    0x0000, 0x0000,

    0x0000, 0x0000,
};

namespace apk {

    void gameBeginPause();
    void gameEndPause();

    namespace bank {
        void* getSpriteBankData(int32 bankNum, uint16 spriteNum, uint32* outSize, uint16* outWidth, uint16* outHeight, int16* offsetX, int16* offsetY);
    }

    namespace video {

		struct Window* mWindow;
        static bool mWindowLoopStop = FALSE;
        static struct timeval _time0;
        static bool isPaletteFading = FALSE;
        static int32 paletteFadingDelta = 0;
        static int32 paletteFadingTime = 0;

        void paletteFunction();


        class SystemTimer {

            struct MsgPort* _msgPort;
            struct timerequest* _req;
            uint32 _signalBit;

        public:

            SystemTimer() :
                _msgPort(NULL), _req(NULL) {
            }

            ~SystemTimer() {

            }

            bool open() {
                if (_msgPort) {
                    close();
                }

                _msgPort = CreatePort(NULL, 0);
                if (_msgPort == NULL) {
                    requester_okay("Error", "Could not open message port");
                    return false;
                }

                _req = (struct timerequest*) CreateExtIO(_msgPort, sizeof(struct timerequest));

                if (_req == NULL) {
                    DeletePort(_msgPort);
                    _msgPort = NULL;
                    requester_okay("Error", "Could not create timer request");
                    return false;
                }

                if (OpenDevice(TIMERNAME, UNIT_MICROHZ, (struct IORequest*)_req, 0) != 0) {
                    DeleteExtIO((struct IORequest*) _req);
                    _req = NULL;
                    DeletePort(_msgPort);
                    _msgPort = NULL;
                    return false;
                }

                TimerBase = (struct Device*) _req->tr_node.io_Device;

                GetSysTime(&_time0);

                return true;
            }

            void close() {
                 if (_req != NULL) {

                    AbortIO((struct IORequest*) _req);
                    WaitIO((struct IORequest*) _req);

                    CloseDevice((struct IORequest*) _req);
                    DeleteExtIO((struct IORequest*) _req);
                    _req = NULL;
                }

                if (_msgPort != NULL) {
                    DeletePort(_msgPort);
                    _msgPort = NULL;
                }
            }

            uint32 start(uint32 microSeconds) {
                _req->tr_time.tv_secs = 0;
                _req->tr_time.tv_micro = microSeconds;
                _req->tr_node.io_Command = TR_ADDREQUEST;
                SendIO(&_req->tr_node);

                return 1 << _msgPort->mp_SigBit;
            }

            bool isReady() {
                WaitPort(_msgPort);
                GetMsg(_msgPort);

                return true;
            }

            uint32 getSignalBit() const {
                if (_msgPort != NULL) {
                    return 1 << _msgPort->mp_SigBit;
                }
                else {
                    return 0;
                }
            }

        };



        bool createWindow(struct Screen* screen, uint16 width, uint16 height, uint8 depth) {

            mWindow = OpenWindowTags(NULL,
                WA_Left, 0,
                WA_Top, 0,
                WA_Width, width,
                WA_Height, height,
                WA_CustomScreen, (ULONG) screen,
                WA_Backdrop, TRUE,
                WA_Borderless, TRUE,
                WA_DragBar, FALSE,
                WA_Activate, TRUE,
                WA_SimpleRefresh, TRUE,
                WA_CloseGadget, FALSE,
                WA_DepthGadget, FALSE,
                WA_RMBTrap, TRUE,
                WA_Flags, WFLG_REPORTMOUSE,
                WA_IDCMP, IDCMP_CLOSEWINDOW | IDCMP_VANILLAKEY | IDCMP_RAWKEY | IDCMP_IDCMPUPDATE | IDCMP_MOUSEMOVE | IDCMP_MOUSEBUTTONS,
                TAG_END
		    );

            if (mWindow == NULL) {
                requester_okay("Error", "Could open Window for Screen!");
                return false;
            }


            SetPointer(mWindow, s_Cursor, 16, 16, -6, 0);


            return true;
        }

        void destroyWindow() {


            if (mWindow) {
                ClearPointer(mWindow);

                CloseWindow(mWindow);
                mWindow = NULL;
            }
        }

        void setCursorFromBank(int32 bankNum, uint16 spriteNum) {
            uint32 spriteSize;
            uint16 width, height;
            int16 offsetX, offsetY;
            UWORD* spriteData = (UWORD*) bank::getSpriteBankData(bankNum, spriteNum, &spriteSize, &width, &height, &offsetX, &offsetY);

            if (spriteData) {
                SetPointer(mWindow, spriteData, height, width, offsetX, offsetY);
            }
        }
        
        void setCursor(uint8* image, uint32 size, uint32 width, uint32 height, int32 offsetX, int32 offsetY) {
            CopyMem(image, s_Cursor + 2, size);
            s_Cursor[2 + size] = 0;
            s_Cursor[2 + size + 1] = 0;
            SetPointer(mWindow, s_Cursor, height, width, offsetX, offsetY);
        }

        void setCursorChunky(uint8* image, uint32 size, uint32 width, uint32 height, int32 offsetX, int32 offsetY) {
            uint8 data[16 * 16];
            uint32 planeOffset = (16 / 8) * 16;

            uint32 dstIdx = 0;
            uint8 bit = 0, bitIdx = 0;

            for(uint16 j=0;j < height;j++) {
                for(uint8 plane=0;plane < 2;plane++) {
                    for(uint16 i=0;i < width;i++) {
                        uint8 b = (image[i + (j * width)] >> plane) & 1;
                        bit <<= 1;
                        bit |= b;
                        bitIdx++;
                        if (bitIdx == 8) {
                            data[dstIdx++] = bit;
                            bit = 0;
                            bitIdx = 0;
                        }
                    }
                }
            }

            setCursor(data, dstIdx, 16,16, offsetX, offsetY);

        }

        void windowStopLoop() {
            mWindowLoopStop = TRUE;
        }

        typedef void(*WindowEventFn)(void* user, Event& evt);
        typedef void(*WindowTimerFn)(void* user);

        template<typename T>
        struct UserCallback {
            T fn;
            void* data;

            UserCallback() = default;

            UserCallback(T fn_, void* data_)
                : fn(fn_), data(data_) {
            }

        };

        Stack<UserCallback<WindowEventFn>, 4> s_EventFns;
        Stack<UserCallback<WindowTimerFn>, 4> s_TimerFns;


        void windowStartLoop(uint32 waitTime_usec) {

            struct IntuiMessage* msg;

            SystemTimer mTimer;
            mTimer.open();

            ULONG timerBit = mTimer.start(waitTime_usec);
            ULONG windowBit = (1 << mWindow->UserPort->mp_SigBit);
		    ULONG signalBits = windowBit | timerBit | SIGBREAKF_CTRL_C;

            mWindowLoopStop = FALSE;
            uint32 mouseX = 0, mouseY = 0;
            int32 reportedMouse = -1;
            bool isPaused = false;

		    while (mWindowLoopStop == false) {

                ULONG signal = Wait(signalBits);

                if (signal & SIGBREAKF_CTRL_C) {
                    mWindowLoopStop = true;
                    break;
                }

                if (signal & windowBit) {

                    while ((msg = (struct IntuiMessage*)GetMsg(mWindow->UserPort)) != NULL)
                    {
                        Event evt;
                        evt.type = EVENT_NONE;

                        if (isPaused) {
                            if ((msg->Class == IDCMP_RAWKEY && msg->Code == 0x40) ||
                                (msg->Class == IDCMP_VANILLAKEY && msg->Code == ' '))
                            {
                                isPaused = false;
                                ReplyMsg((struct Message*)msg);
                                apk::gameEndPause();
                                continue;
                            }
                            else {
                                ReplyMsg((struct Message*)msg);
                            }
                            continue;
                        }

                        switch (msg->Class)
                        {
                            case IDCMP_CLOSEWINDOW: {
                                mWindowLoopStop = TRUE;
                            }
                            break;
                            case IDCMP_VANILLAKEY: {
                                if (msg->Code == 27) {
                                    mWindowLoopStop = TRUE;
                                }
                                else if (msg->Code == ' ') {
                                    isPaused = true;
                                    ReplyMsg((struct Message*)msg);
                                    apk::gameBeginPause();
                                    continue;
                                }
                            }
                            break;
                            case IDCMP_RAWKEY: {

                                if (msg->Code == 0x40) {
                                    isPaused = true;
                                    ReplyMsg((struct Message*)msg);
                                    apk::gameBeginPause();
                                    continue;
                                }

                                mouseX = msg->MouseX;
                                mouseY = msg->MouseY;

                                evt.type = EVENT_KEYINSTANT;
                                evt.kbd.keycode = msg->Code;
                                evt.kbd.shift = (msg->Qualifier & IEQUALIFIER_LSHIFT || msg->Qualifier & IEQUALIFIER_RSHIFT);
                                printf("[D] Raw Key = %ld, Shift = %ld\n", evt.kbd.keycode, evt.kbd.shift);
                            }
                            break;
                            case IDCMP_MOUSEMOVE: {
                                mouseX = msg->MouseX;
                                mouseY = msg->MouseY;
                                reportedMouse = 0;
                            }
                            break;
                            case IDCMP_MOUSEBUTTONS: {
                                mouseX = msg->MouseX;
                                mouseY = msg->MouseY;
                                reportedMouse = 1;

                                evt.mouse.x = mouseX;
                                evt.mouse.y = mouseY;

                                if (msg->Code == SELECTUP) {
                                    evt.type = EVENT_LBUTTONUP;
                                }
                                else if (msg->Code == SELECTDOWN){
                                    evt.type = EVENT_LBUTTONDOWN;
                                }

                                if (msg->Code == MENUUP) {
                                    evt.type = EVENT_RBUTTONUP;
                                }
                                else if (msg->Code == MENUDOWN){
                                    evt.type = EVENT_RBUTTONDOWN;
                                }

                            }
                            break;
                        }

                        ReplyMsg((struct Message*)msg);
                        if (evt.type != EVENT_NONE) {
                            const auto& cb = s_EventFns.top();
                            cb.fn(cb.data, evt);
                            if (reportedMouse == 1) {
                                reportedMouse = -1;
                            }
                        }
                    }
                }

                if (signal & timerBit) {
                    if (mTimer.isReady()) {
                        if (isPaused) {
                            mTimer.start(waitTime_usec);
                        }
                        else {
                            if (reportedMouse == 0) {
                                Event evt;
                                evt.type = EVENT_MOUSEMOVE;
                                evt.mouse.x = mouseX;
                                evt.mouse.y = mouseY;
                                const auto& wcb = s_EventFns.top();
                                wcb.fn(wcb.data, evt);
                                reportedMouse = -1;
                            }

                            const auto& tcb = s_TimerFns.top();
                            tcb.fn(tcb.data);

                            mTimer.start(waitTime_usec);

                            paletteFunction();
                        }

                    }
                }

		    }

		    mTimer.close();
        }

        void pushWindowEventCallback(WindowEventFn cb, void* data) {
            s_EventFns.push_back(UserCallback<WindowEventFn>(cb, data));
        }
        void pushWindowTimerCallback(WindowTimerFn cb, void* data) {
            s_TimerFns.push_back(UserCallback<WindowTimerFn>(cb, data));
        }

        void popWindowEventCallback() {
            s_EventFns.pop_back();
        }
        void popWindowTimerCallback() {
            s_TimerFns.pop_back();
        }

    }

    bool isQuitRequested() {
        return video::mWindowLoopStop;
    }

    

}
