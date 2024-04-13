// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include <apk/compat.h>

void* APK_ATTR_WEAK operator new(APK_SIZE_TYPE size, void* p) {
    return p;
}

void* APK_ATTR_WEAK operator new(APK_SIZE_TYPE size, const char* comment) {
    return ::apk::_apk_allocate(size, comment);
}
