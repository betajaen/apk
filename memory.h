// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "pod.h"
#include "compat.h"
#include "consts.h"

namespace apk {

    void memcpy(void* dst, const void* src, APK_SIZE_TYPE length);
    void memcpy_aligned(void* dst, const void* src, APK_SIZE_TYPE length);

    void memset(void* dst, uint32 val, APK_SIZE_TYPE length);
    void memset_aligned(void* dst, uint32 val, APK_SIZE_TYPE length);

    class MemoryBuffer {
        byte* m_data;
        uint32 m_size, m_pos;

    public:

        MemoryBuffer() {
            m_data = NULL;
            m_size = 0;
            m_pos = 0;
        }

        MemoryBuffer(byte* data, uint32 size) {
            m_data = data;
            m_size = size;
            m_pos = 0;
        }

        ~MemoryBuffer() {
            m_data = NULL;
            m_size = 0;
            m_pos = 0;
        }

        byte* getData() const {
            return m_data;
        }

        bool isEnd() const {
            return m_pos >= m_size;
        }

        void pos(uint32 pos) {
            assert(pos <= m_size);
            m_pos = pos;
        }

        int32 seek(int32 offset, SeekMode from) {
            assert(m_pos <= m_size);
            switch(from) {
                case SeekMode::Cur:
                    m_pos += offset;
                break;
                case SeekMode::Set:
                    m_pos = offset;
                break;
                case SeekMode::End:
                    m_pos = m_size + offset;
                break;
                case SeekMode::GetPos:
                    return m_pos;
                case SeekMode::GetSize:
                    return m_size;
            }
            assert(m_pos <= m_size);
        }

        uint32 pos() {
            return m_pos;
        }

        uint32 size() const {
            return m_size;
        }

        uint32 read(void* data, uint32 size) {
            if (size > m_size - m_pos) {
                size = m_size - m_pos;
            }

            memcpy(data, m_data + m_pos, size);

            m_pos += size;
            return size;
        }


    };

}
