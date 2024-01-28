// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "apk/pod.h"
#include "apk/compat.h"
#include "apk/consts.h"
#include "apk/endian.h"

namespace apk {

    typedef APK_SSIZE_TYPE (*ReadStreamFunction)(const void* udata, void* dst, APK_SIZE_TYPE size);
    typedef APK_SSIZE_TYPE (*WriteStreamFunction)(const void* udata, const void* dst, APK_SIZE_TYPE size);
    typedef APK_SSIZE_TYPE (*SeekStreamFunction)(const void* udata, APK_SSIZE_TYPE amount, SeekMode where);

    class ReadStream {
        const void* m_udata;
        const ReadStreamFunction m_read_function;
        const SeekStreamFunction m_seek_function;
        const int32 m_endian;
    public:
        ReadStream(const ReadStreamFunction readFn, const SeekStreamFunction seekFn, const void* udata, const int32 endian);

        bool canSeek() const;

        APK_SSIZE_TYPE pos();
        
        APK_SSIZE_TYPE size();

        APK_SSIZE_TYPE seek(APK_SSIZE_TYPE size, SeekMode mode);

        APK_SSIZE_TYPE read(void* dst, APK_SIZE_TYPE size);

        bool skip(APK_SIZE_TYPE offset);

        byte readByte();

        uint16 readUint16();

        int16 readInt16();
        
        uint32 readUint32();

        int32 readInt32();

    };

}