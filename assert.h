// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

namespace apk {
    void doAssert(const char* file, int line);
}

#define assert(COND) if (!(COND)) { apk::doAssert(__FILE__, __LINE__); }