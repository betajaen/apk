// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

namespace apk {

    struct Event;

    namespace video {

        typedef void(*WindowEventFn)(void* user, Event& evt);
        typedef void(*WindowTimerFn)(void* user);

        bool createScreen(const char* title, uint16 width, uint16 height, uint8 depth);
        void destroyScreen();
        void windowStartLoop(uint32 waitTime_usec);
        void windowStopLoop();
        void pushWindowEventCallback(WindowEventFn, void*);
        void pushWindowTimerCallback(WindowTimerFn, void*);
        void popWindowEventCallback();
        void popWindowTimerCallback();
        void flipScreen();
        void writeChunkyPixels(uint8* data);
        void writeChunkyPixelsBlit(uint8* data, uint32 x, uint32 y, uint32 w, uint32 h, uint32 stride);
        void writeRect(uint32 l, uint32 t, uint32 r, uint32 b, uint8 col);
        void writePixel(uint32 x, uint32 y, uint8 col);
        void clearChunkyPixels(uint8 index);
        void setRGB(uint8 index, uint8 r, uint8 g, uint8 b);
        void setRGB(uint8* pal, uint32 begin, uint32 end);
        void clearPalette();
        void setCursorFromBank(int32 bankNum, uint16 spriteNum);
        void setCursor(uint8* image, uint32 size, uint32 width, uint32 height, int32 offsetX, int32 offsetY);
        void setCursorChunky(uint8* image, uint32 size, uint32 width, uint32 height, int32 offsetX, int32 offsetY);
        void paletteFadeIn(uint32 speed);
        void fillRect(uint32 l, uint32 t, uint32 w, uint32 h, uint8 col);
        void copy(uint32 l, uint32 t, uint32 w, uint32 h);
        void paste(uint32 l, uint32 t);
        void pasteIcon(uint8* img, uint32 x, uint32 y, uint32 w, uint32 h, uint8 transparent, uint8* pal);
        void forceUpdateScreen();
        void setDebugNum(uint32 num);
        void setDebugStr(const char* str);
        void clearDebug();

    }

}
