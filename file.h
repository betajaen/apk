// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "pod.h"
#include "consts.h"

namespace apk {

    class FileImpl;

    class ReadFile {

        FileImpl* m_impl;

    public:

        ReadFile();
        ~ReadFile();

        bool open(const char*);

        bool close();

        bool isOpen() const;

        uint32 size() const;

        int32 seek(int32 where, SeekMode mode = SeekMode::Set);

        uint32 read(void* data, uint32 size);

        int16 readSint16BE();

        int32 readSint32BE();

        uint16 readUint16BE();

        uint32 readUint32BE();

        static bool exists(const char*);

    };


    class AppendFile {

        FileImpl* m_impl;

    public:

        AppendFile();
        ~AppendFile();

        bool open(const char*);

        bool close();

        bool isOpen() const;

        uint32 write(void* data, uint32 size);

    };



}
