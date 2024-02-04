// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include "apk/apk.h"
#include "apk/file.h"

#include <proto/dos.h>

namespace apk {

    constexpr APK_SIZE_TYPE kMaxPathLength = 256;

    class ReadFileData {
    public:
        BPTR  fh;
        LONG  size;
        LONG  pos;
    };

    bool ReadFile::open(const char* path) {
        close();
        
        char diskPath[kMaxPathLength];
        sprintf_s(diskPath, sizeof(diskPath), "%s", path);

        BPTR fh = Open(diskPath, MODE_OLDFILE);

        if (fh == 0UL) {
            error("File '%s' could not be found in path!\n", diskPath);
            return false;
        }

        m_data = apk_new ReadFileData();
        m_data->fh = fh;
        m_data->pos = 0;
        m_data->size = 0;

        Seek(fh, 0, OFFSET_END);
        m_data->size = Seek(fh, 0, OFFSET_BEGINNING);

        return true;
    }

    bool ReadFile::close() {
        if (m_data) {
            Close(m_data->fh);
            apk_delete(m_data);
            m_data = nullptr;
            return true;
        }
        return false;
    }

    int32 ReadFile::size() const {
        if (m_data) {
            return m_data->size;
        }
        else {
            return -1;
        }
    }

    int32 ReadFile::pos() const {
        if (m_data) {
            return m_data->pos;
        }
        else {
            return -1;
        }        
    }

    int32 ReadFile::seek(int32 where, SeekMode mode) {
        if (m_data) {

            int32 rv = -1;
            switch(mode) {
                default:
                    return -1;
                case SeekMode::Set: {
                    rv = Seek(m_data->fh, where, OFFSET_BEGINNING);
                    break;
                }
                case SeekMode::Current: {
                    rv = Seek(m_data->fh, where, OFFSET_CURRENT);
                    break;
                }
                case SeekMode::End: {
                    rv = Seek(m_data->fh, where, OFFSET_END);
                    break;
                }
                case SeekMode::CanSeek:
                    return 1;
                case SeekMode::GetPos:
                    return m_data->pos;
                case SeekMode::GetSize:
                    return m_data->size;
            }

            if (rv != 0) {
                return -1;
            }

            m_data->pos = Seek(m_data->fh, 0, OFFSET_CURRENT);
            return m_data->pos;
        }
        else {
            return -1;
        }
    }

    int32 ReadFile::read(void* data, int32 size) {
        if (m_data == nullptr) {
            return -1;
        }

        int32 rv =  Read(m_data->fh, data, size);
        if (rv < 0) {
            return -1;
        }

        m_data->pos += rv;

        return rv;
    }

}