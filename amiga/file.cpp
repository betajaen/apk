// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include <apk/apk.h>
#include <apk/file.h>
#include <apk/text.h>

#include <proto/dos.h>

namespace apk { namespace fs {
    static char s_ProgDir[256] = "PROGDIR:";
}}

namespace apk { namespace path {

    PathType test(const char* path) {
        char fullPath[255];
        FileInfoBlock* fib;
        PathType rv = PathType::None;

        apk::strcpy_s(fullPath, sizeof(fullPath), fs::s_ProgDir);
        AddPart(fullPath, path, sizeof(fullPath));
        BPTR lock = Lock(fullPath, ACCESS_READ);

        if (lock != 0UL) {
            fib = (FileInfoBlock*) AllocDosObject(DOS_FIB, TAG_END);
            if (fib && Examine(lock, fib)) {
                rv = (fib->fib_DirEntryType < 0) ? PathType::File : PathType::DrawerVolume;
                FreeDosObject(DOS_FIB, fib);
            }
            UnLock(lock);

            return rv;
        }
        return rv;
    }

}}

namespace apk { namespace fs {

    void setProgramDir(const char* path) {
        if (apk::string_endswith(path, "/") || apk::string_endswith(path, ":")) {
            apk::strcpy_s(s_ProgDir, sizeof(s_ProgDir), path);
        }
        else {
            apk::sprintf_s(s_ProgDir, sizeof(s_ProgDir), "%s/", path);
        }

    }

    const char* getProgramDir() {
        return s_ProgDir;
    }

}}

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
            sprintf_s(diskPath, sizeof(diskPath), "%s%s", fs::s_ProgDir, path);
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
            sprintf_s(diskPath, sizeof(diskPath), "%s%s", fs::s_ProgDir, path);
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


    bool ReadFile::seek(int32 where, int32 mode) {
        assert(m_impl);

        switch(mode) {
            default:
                return false;
            case kSEEK_SET:
                return Seek(m_impl->fh, where, OFFSET_BEGINNING) != 0;
            case kSEEK_CUR:
                return Seek(m_impl->fh, where, OFFSET_CURRENT) != 0;
            case kSEEK_END:
                return Seek(m_impl->fh, where, OFFSET_END) != 0;
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
            sprintf_s(diskPath, sizeof(diskPath), "%s%s", fs::s_ProgDir, path);
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