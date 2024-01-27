// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include "apk/apk.h"
#include "apk/gfx.h"

namespace apk {
    bool s_FastMode = true;
    uint32 s_FastModeTime = 0;
    bool s_quitRequested = true;

    void gameMain();
}

int apk_main() {
    apk::s_quitRequested = false;

    apk::gameMain();

    apk::gfx::destroyScreen();
}

namespace apk {

    static ULONG ticks = 0;

    void delayMs(uint32 ms) {
        ticks += ms;
    }

    uint32 getMs() {
        ticks++;
        return ticks;
    }
}