// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "apk/pod.h"
#include "apk/compat.h"
#include "apk/consts.h"
#include "apk/endian.h"
#include "apk/assert.h"

namespace apk {

    constexpr int32 kStreamMaxSize = 0x7FFFFFFF; // ~2 GiB.

    struct MemoryBuffer;

    template<typename T>
    struct StreamData;

    template<typename TData>
    struct ReadStream {
        protected:
            TData* m_data;
            int32  m_lower_bound;
            int32  m_upper_bound;
            
            inline void setNull() {
                m_data = nullptr;
                m_lower_bound = 0L;
                m_upper_bound = 0L;
            }
            
            void gain(TData* newData) {
                if (m_data) {
                    lose();
                }

                m_data = newData;

                if (m_data) {
                    m_data->ref++;
                }
            }

            void lose() {
                if (m_data) {
                    m_data->ref--;
                    if (m_data->ref == 0) {
                        m_data->close();
                        apk_deallocate(m_data);
                    }
                    m_data = nullptr;
                }
                m_lower_bound = 0;
                m_upper_bound = 0;
            }

            ReadStream(TData* udata,  int32 lower_bound, int32 upper_bound) {
                m_data = udata;
                m_lower_bound = lower_bound;
                m_upper_bound = upper_bound;
                
            }

        public:

            friend ReadStream OpenFileStream(const char* path, int32 endian);

            ReadStream() {
                setNull();
            }

            ReadStream(const ReadStream& s) {
                setNull();
                gain(s.m_data);
                m_upper_bound = s.m_upper_bound;
                m_lower_bound = s.m_lower_bound;
            }

            ReadStream(ReadStream&& s) {
                setNull();
                swap(s.m_data, m_data);
                swap(s.m_upper_bound, m_upper_bound);
                swap(s.m_lower_bound, m_lower_bound);
            }

            ~ReadStream() {
                lose();
            }

            ReadStream& operator=(const ReadStream& s) {
                lose();
                gain(s.m_data);
                m_upper_bound = s.m_upper_bound;
                m_lower_bound = s.m_lower_bound;
            }

            ReadStream& operator=(ReadStream&& s) {
                lose();
                swap(s.m_data, m_data);
                swap(s.m_upper_bound, m_upper_bound);
                swap(s.m_lower_bound, m_lower_bound);
            }

            int32 seek(int32 offset, SeekMode mode) {
                if (m_data) {
                    return m_data->seek(offset, mode, m_lower_bound, m_upper_bound);
                }
                else {
                    return -1;
                }
            }

            inline bool trySeek(int32 offset, SeekMode mode) {
                return seek(offset, mode) != -1;
            }

            int32 read(void* dst, int32 size) {
                if (m_data) {
                    return m_data->read(dst, size, m_upper_bound);
                }
                else {
                    return -1;
                }
            }

            inline int32 readByte() {
                byte v;
                int32 rv = read(&v, sizeof(v));
                if (rv != sizeof(v)) {
                    return -1;
                }
                return v;
            }

            template<typename T>
            bool tryReadPod(T& value) {
                T v = {};
                int32 rv = read(&v, sizeof(T));
                if (rv != sizeof(T)) {
                    return false;
                }
                value = endian::read_from(v, m_data->src_endian); 
                return true;
            }

            template<typename T>
            bool tryReadPod(T& value, const T& defaultValue) {
                T v = {};
                int32 rv = read(&v, sizeof(T));
                if (rv != sizeof(T)) {
                    value = endian::read_from(v, m_data->src_endian); 
                    return true;
                }
                else {
                    value = defaultValue;
                    return false;
                }
            }

    };


}