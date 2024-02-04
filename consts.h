// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.
#pragma once

namespace apk {

    enum class SeekMode {
        Set,
        Current,
        End,
        GetPos,
        GetSize,
        CanSeek
    };

}