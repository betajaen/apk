// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include <apk/apk.h>

namespace apk { namespace prefs {

    const char* getPrefsString(const char* name, const char* defaultValue) {
        return defaultValue;
    }

    uint32 getPrefsNumber(const char* name, uint32 defaultValue) {
        return defaultValue;
    }

}}