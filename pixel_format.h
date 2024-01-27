// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include <apk/pod.h>

namespace apk {

    #define APK_PF_PLANAR_BIT        (1<<5)
    #define APK_PF_CHUNKY_BIT        (1<<6)
    #define APK_PF_RGB_BIT           (1<<7)
    #define APK_PF_BGR_BIT           (1<<8)
    #define APK_PF_RGBA_BIT          (1<<9)
    #define APK_PF_BGRA_BIT          (1<<10)
    #define APK_PF_ARGB_BIT          (1<<11)
    #define APK_PF_ABGR_BIT          (1<<12)

    enum class PixelFormat : uint32 {
        None              = 0,

        Planar1           = APK_PF_PLANAR_BIT + 1,
        Planar2           = APK_PF_PLANAR_BIT + 2,
        Planar3           = APK_PF_PLANAR_BIT + 3,
        Planar4           = APK_PF_PLANAR_BIT + 4,
        Planar5           = APK_PF_PLANAR_BIT + 5,
        Planar6           = APK_PF_PLANAR_BIT + 6,
        Planar7           = APK_PF_PLANAR_BIT + 7,
        Planar8           = APK_PF_PLANAR_BIT + 8,

        Chunky8           = APK_PF_CHUNKY_BIT + 8,

        RGB555            = APK_PF_RGB_BIT + 555,
        BGR555            = APK_PF_BGR_BIT + 555,

        RGB565            = APK_PF_RGB_BIT + 565,
        BGR565            = APK_PF_BGR_BIT + 565,
        
        RGB24             = APK_PF_RGB_BIT + 24, 
        BGR24             = APK_PF_BGR_BIT + 24,     
        
        RGBA32            = APK_PF_RGBA_BIT + 32, 
        ARGB32            = APK_PF_ARGB_BIT + 32, 
        BGRA32            = APK_PF_BGRA_BIT + 32,  
        ABGR32            = APK_PF_ABGR_BIT + 32,   
    };
    
    const char* GetPixelFormatString(PixelFormat pf);

}
