// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include <apk/pixel_format.h>

namespace apk {

    const char* GetPixelFormatString(PixelFormat pf) {
        switch(pf) {
            default:                        return "Unknown";
            case PixelFormat::None:         return "None";
            case PixelFormat::Planar1:      return "Planar1";
            case PixelFormat::Planar2:      return "Planar2";
            case PixelFormat::Planar3:      return "Planar3";
            case PixelFormat::Planar4:      return "Planar4";
            case PixelFormat::Planar5:      return "Planar5";
            case PixelFormat::Planar6:      return "Planar6";
            case PixelFormat::Planar7:      return "Planar7";
            case PixelFormat::Planar8:      return "Planar8";
            case PixelFormat::Chunky8:      return "Chunky8";
            case PixelFormat::RGB555:       return "RGB555";
            case PixelFormat::BGR555:       return "BGR555";
            case PixelFormat::RGB565:       return "RGB565";
            case PixelFormat::BGR565:       return "BGR565";     
            case PixelFormat::RGB24:        return "RGB24";
            case PixelFormat::BGR24:        return "BGR24";      
            case PixelFormat::RGBA32:       return "RGBA32";
            case PixelFormat::ARGB32:       return "ARGB32";
            case PixelFormat::BGRA32:       return "BGRA32";
            case PixelFormat::ABGR32:       return "ABGR32";
        }
    }

}
