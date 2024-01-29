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
            return endian::read_from(val, m_endian);
        }

        int16 ReadStream::readInt16() {
            int16 val;
            read(&val, sizeof(int16));
            return endian::read_from(val, m_endian);
        }
        
        uint32 ReadStream::readUint32() {
            uint32 val;
            read(&val, sizeof(uint32));
            return endian::read_from(val, m_endian);
        }

        int32 ReadStream::readInt32() {
            int32 val;
            read(&val, sizeof(int32));
            return endian::read_from(val, m_endian);
        }


        uint16 ReadStream::readUint16BE() {
            uint16 val;
            read(&val, sizeof(uint16));
            return endian::endian_swap<uint16, endian::Big, endian::Native>(val);
        }

        int16 ReadStream::readInt16BE() {
            int16 val;
            read(&val, sizeof(int16));
            return endian::endian_swap<int16, endian::Big, endian::Native>(val);
        }
        
        uint32 ReadStream::readUint32BE() {
            uint32 val;
            read(&val, sizeof(uint32));
            return endian::endian_swap<uint32, endian::Big, endian::Native>(val);
        }

        int32 ReadStream::readInt32BE() {
            int32 val;
            read(&val, sizeof(int32));
            return endian::endian_swap<int32, endian::Big, endian::Native>(val);
        }


        uint16 ReadStream::readUint16LE() {
            uint16 val;
            read(&val, sizeof(uint16));
            return endian::endian_swap<uint16, endian::Little, endian::Native>(val);
        }

        int16 ReadStream::readInt16LE() {
            int16 val;
            read(&val, sizeof(int16));
            return endian::endian_swap<int16, endian::Little, endian::Native>(val);
        }
        
        uint32 ReadStream::readUint32LE() {
            uint32 val;
            read(&val, sizeof(uint32));
            return endian::endian_swap<uint32, endian::Little, endian::Native>(val);
        }

        int32 ReadStream::readInt32LE() {
            int32 val;
            read(&val, sizeof(int32));
            return endian::endian_swap<int32, endian::Little, endian::Native>(val);
        }

        WriteStream::WriteStream(const WriteStreamFunction writeFn, const SeekStreamFunction seekFn, const void* udata, const int32 endian)
            : m_udata(udata), m_write_function(writeFn), m_seek_function(seekFn), m_endian(endian)
        {
        }

        bool WriteStream::canSeek() const {
            return m_seek_function != nullptr;
        }

        APK_SSIZE_TYPE WriteStream::pos() {
            if (m_seek_function == nullptr) {
                return -1;
            }
            else {
                return m_seek_function(m_udata, 0, SeekMode::GetPos);
            }
        }

        APK_SSIZE_TYPE WriteStream::size() {
            if (m_seek_function == nullptr) {
                return -1;
            }
            else {
                return m_seek_function(m_udata, 0, SeekMode::GetSize);
            }
        }

        APK_SSIZE_TYPE WriteStream::seek(APK_SSIZE_TYPE size, SeekMode mode) {
            assert(m_seek_function);
            return m_seek_function(m_udata, size, mode);
        }

        APK_SSIZE_TYPE WriteStream::write(const void* dst, APK_SIZE_TYPE size) {
            assert(m_write_function);
            return m_write_function(m_udata, dst, size);
        }

        bool WriteStream::skip(APK_SIZE_TYPE offset) {
            return seek(offset, SeekMode::Cur) != 0;
        }

        void WriteStream::writeByte(byte b) {
            byte value;
            write(&value, sizeof(byte));
        }

        void WriteStream::writeUint16(uint16 val) {
            val = endian::write_to(val, m_endian);
            write(&val, sizeof(val));
        }

        void WriteStream::writeInt16(int16 val) {
            val = endian::write_to(val, m_endian);
            write(&val, sizeof(val));
        }
        
        void WriteStream::writeUint32(uint32 val) {
            val = endian::write_to(val, m_endian);
            write(&val, sizeof(val));
        }

        void WriteStream::writeInt32(int32 val) {
            val = endian::write_to(val, m_endian);
            write(&val, sizeof(val));
        }

        

}