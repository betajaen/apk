// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "pod.h"
#include "compat.h"
#include "consts.h"
#include "stream.h"

namespace apk {

    struct ReadFileData;

    struct ReadFile {
    private:
        ReadFileData* m_data;

        inline void null_impl() {
            m_data = nullptr;
        }

        inline void swap_impl(ReadFile& s) {
            swap(s.m_data, m_data);
        }
        
    public:

        apk_unique_like(ReadFile, null_impl, close, swap_impl);

        bool open(const char*);

        bool close();

        bool isOpen() const {
            return m_data != nullptr;
        }

        int32 size() const;

        int32 pos() const;

        int32 seek(int32 where, SeekMode mode = SeekMode::Set);

        int32 read(void* data, int32 size);

    };

    template<>
    struct StreamData<ReadFile> {
        ReadFile udata;
        int32    src_endian;
        int32    ref;

        int32    read(void* dst, int32 size, int32 upper_bound);
        int32    seek(int32 offset, SeekMode where, int32 lower_bound, int32 upper_bound);
        bool     close();
    };

    typedef StreamData<ReadFile> ReadFileStreamData;

    typedef ReadStream<ReadFileStreamData> ReadFileStream;

    ReadFileStream OpenReadFileStream(const char* path, int32 endian);


    namespace fs {



    }

}
