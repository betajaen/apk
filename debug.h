// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "debug.h"

namespace apk {

    void printf(const char* fmt, ...);
    void debug(int, const char* fmt, ...);
    void debug(const char* str, ...);
    void warning(const char* fmt, ...);
    void error(const char* fmt, ...);

    void debug_num(uint32 num);
    void debug_str(const char* str);
    void debug_clear();

}
