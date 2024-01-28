// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include "apk/apk.h"
#include "apk/file.h"

#include <proto/dos.h>

namespace apk {

    class FileImpl {
    public:
        ULONG fh;
        ULONG size;
    };

    ReadFile::ReadFile() {
        m_impl = NULL;
    }

    ReadFile::~ReadFile() {
        close();
    }

    bool ReadFile::close() {
        if (m_impl) {
            Close(m_impl->fh);
            apk_delete(m_impl);
            m_impl = NULL;
            return true;
        }
        return false;
    }

    bool ReadFile::isOpen() const {
        return m_impl != NULL;
    }

    bool ReadFile::open(const char* path) {
        close();

        char diskPath[256] = { 0 };

        if (strchr(path, ':') == NULL) {
            sprintf_s(diskPath, sizeof(diskPath), "PROGDIR:%s", path);
        }
        else {
            sprintf_s(diskPath, sizeof(diskPath), "%s", path);
        }

        ULONG fh = Open(diskPath, MODE_OLDFILE);
        if (fh == 0UL) {
            return false;
        }

        m_impl = apk_new FileImpl();
        m_impl->fh = fh;
        Seek(fh, 0, OFFSET_END);
        m_impl->size = Seek(fh, 0, OFFSET_BEGINNING);

        return true;
    }

    uint32 ReadFile::size() const {

        if (m_impl == NULL) {
            return 0;
        }

        return m_impl->size;
    }

    bool ReadFile::exists(const char* path) {

        char diskPath[256] = { 0 };
        if (strchr(path, ':') == NULL) {
            sprintf_s(diskPath, sizeof(diskPath), "PROGDIR:%s", path);
        }
        else {
            sprintf_s(diskPath, sizeof(diskPath), "%s", path);
        }

        ULONG fh = Open(diskPath, MODE_OLDFILE);

        if (fh == 0UL) {
            return false;
        }

        Close(fh);

        return true;
    }


    int32 ReadFile::seek(int32 where, SeekMode mode) {
        assert(m_impl);

        switch(mode) {
            default:
                return -1;
            case SeekMode::Set:
                return Seek(m_impl->fh, where, OFFSET_BEGINNING);
            case SeekMode::Cur:
                return Seek(m_impl->fh, where, OFFSET_CURRENT);
            case SeekMode::End:
                return Seek(m_impl->fh, where, OFFSET_END);
        }
    }

    uint32 ReadFile::read(void* data, uint32 size) {
        assert(m_impl);

        return (ULONG) Read(m_impl->fh, data, size);
    }

    int16 ReadFile::readSint16BE() {
        int16 value;
        read(&value, sizeof(value));
        return endian::pod<int16, endian::Big>(value);
    }

    int32 ReadFile::readSint32BE() {
        int32 value;
        read(&value, sizeof(value));
        return endian::pod<int32, endian::Big>(value);
    }

    uint16 ReadFile::readUint16BE() {
        uint16 value;
        read(&value, sizeof(value));
        return endian::pod<uint16, endian::Big>(value);
    }

    uint32 ReadFile::readUint32BE() {
        uint32 value;
        read(&value, sizeof(value));
        return endian::pod<uint32, endian::Big>(value);
    }


    AppendFile::AppendFile() {
        m_impl = NULL;
    }

    AppendFile::~AppendFile() {
        close();
    }

    bool AppendFile::open(const char* path) {
        close();

        char diskPath[256] = { 0 };
        if (strchr(path, ':') == NULL) {
            sprintf_s(diskPath, sizeof(diskPath), "PROGDIR:%s", path);
        }
        else {
            sprintf_s(diskPath, sizeof(diskPath), "%s", path);
        }

        ULONG fh = Open(diskPath, MODE_NEWFILE);
        if (fh == 0UL) {
            return false;
        }

        m_impl = apk_new FileImpl();
        m_impl->fh = fh;
        m_impl->size = 0;

        return true;
    }

    bool AppendFile::close() {
         if (m_impl) {
            Close(m_impl->fh);
            apk_delete(m_impl);
            m_impl = NULL;
            return true;
        }
        return false;
    }

    bool AppendFile::isOpen() const {
        return m_impl != NULL;
    }

    uint32 AppendFile::write(void* data, uint32 size) {
        return (ULONG) Write(m_impl->fh, data, size);
    }

}