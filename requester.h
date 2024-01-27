// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "pod.h"

namespace apk {

    const char* requester_load(const char* title, const char* drawer, const char* extension);
    const char* requester_save(const char* title, const char* drawer, const char* extension);
    void requester_okay(const char* title, const char* text);
    int32 requester_yesno(const char* title, const char* text);
    int32 requester_options(const char* title, const char* text, const char* options);

}
