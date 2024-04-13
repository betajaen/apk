// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.


#include <stdio.h>
#include <SDL2/SDL.h>
#include "apk/apk.h"
#include "apk/gfx.h"

namespace apk {

    bool s_FastMode = true;
    uint32 s_FastModeTime = 0;
    bool s_quitRequested = true;

    void gameMain();
    void gameBeginPause();
    void gameEndPause();

    bool isQuitRequested() {
        return s_quitRequested;
    }

    void delayMs(uint32 ms) {
        if (s_FastMode) {
            s_FastModeTime += ms;
        }
        else {
            SDL_Delay(ms);
        }
    }

    uint32 getMs() {
        if (s_FastMode) {
            return s_FastModeTime;
        }
        else {
            return SDL_GetTicks();
        }
    }
}

int main(void)
{
    SDL_assert(SDL_Init(SDL_INIT_EVERYTHING) == 0);

    apk::s_quitRequested = false;
    apk::gameMain();
    apk::video::destroyScreen();

    SDL_Quit();
}


namespace apk {

    void printf(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        ::printf("\n");
        va_end(args);
    }

    void debug(int l, const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ::printf("[DBG %d] ", l);
        vprintf(fmt, args);
        ::printf("\n");
        va_end(args);
    }

    void debug(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ::printf("[DBG] ");
        vprintf(fmt, args);
        ::printf("\n");
        va_end(args);
    }

    void warning(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ::printf("[WRN] ");
        vprintf(fmt, args);
        ::printf("\n");
        va_end(args);
    }

    void error(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        ::printf("[ERR] ");
        vprintf(fmt, args);
        ::printf("\n");
        va_end(args);
    }

    void doAssert(const char* file, int line) {
        ::printf("\n[ASSERT!] %s:%d\n", file, line);
        SDL_assert(false);
    }

}
