// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include "apk/stream.h"
#include "apk/assert.h"

namespace apk {

        ReadStream::ReadStream(const ReadStreamFunction readFn, const SeekStreamFunction seekFn, const void* udata, const int32 endian)
            : m_udata(udata), m_read_function(readFn), m_seek_function(seekFn), m_endian(endian)
        {
        }

        bool ReadStream::canSeek() const {
            return m_seek_function != nullptr;
        }

        APK_SSIZE_TYPE ReadStream::pos() {
            if (m_seek_function == nullptr) {
                return -1;
            }
            else {
                return m_seek_function(m_udata, 0, SeekMode::GetPos);
            }
        }

        APK_SSIZE_TYPE ReadStream::size() {
            if (m_seek_function == nullptr) {
                return -1;
            }
            else {
                return m_seek_function(m_udata, 0, SeekMode::GetSize);
            }
        }

        APK_SSIZE_TYPE ReadStream::seek(APK_SSIZE_TYPE size, SeekMode mode) {
            assert(m_seek_function);
            return m_seek_function(m_udata, size, mode);
        }

        APK_SSIZE_TYPE ReadStream::read(void* dst, APK_SIZE_TYPE size) {
            assert(m_read_function);
            return m_read_function(m_udata, dst, size);
        }

        bool ReadStream::skip(APK_SIZE_TYPE offset) {
            return seek(offset, SeekMode::Cur) != 0;
        }

        byte ReadStream::readByte() {
            byte value;
            read(&value, sizeof(byte));
            return value;
        }

        uint16 ReadStream::readUint16() {
            uint16 val;
            read(&val, sizeof(uint16));
            return endian::endian_swap_rt<uint16>(val, m_endian);
        }

        int16 ReadStream::readInt16() {
            int16 val;
            read(&val, sizeof(int16));
            return endian::endian_swap_rt<int16>(val, m_endian);
        }
        
        uint32 ReadStream::readUint32() {
            uint32 val;
            read(&val, sizeof(uint32));
            return endian::endian_swap_rt<uint32>(val, m_endian);
        }

        int32 ReadStream::readInt32() {
            int32 val;
            read(&val, sizeof(int32));
            return endian::endian_swap_rt<int32>(val, m_endian);
        }

}