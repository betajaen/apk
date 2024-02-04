// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include "apk/apk.h"
#include "apk/file.h"
#include "stdio.h"

namespace apk {

    constexpr APK_SIZE_TYPE kMaxPathLength = 256;

    struct ReadFileData {
        FILE*  fh;
        int32  size;
        int32  pos;
    };

    bool ReadFile::open(const char* path) {
        close();
        
        char diskPath[kMaxPathLength];
        sprintf_s(diskPath, sizeof(diskPath), "%s", path);

        FILE* fh = fopen(diskPath, "r");

        if (fh == NULL) {
            error("File '%s' could not be found in path!\n", diskPath);
            return false;
        }

        m_data = apk_new ReadFileData();
        m_data->fh = fh;
        m_data->pos = 0;
        m_data->size = 0;

        fseek(m_data->fh, 0, SEEK_END);
        m_data->size = ftell(m_data->fh);
        fseek(m_data->fh, 0, SEEK_SET);

        return true;
    }

    bool ReadFile::close() {
        if (m_data) {
            fclose(m_data->fh);
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
                    rv = fseek(m_data->fh, where, SEEK_SET);
                    break;
                }
                case SeekMode::Current: {
                    rv = fseek(m_data->fh, where, SEEK_CUR);
                    break;
                }
                case SeekMode::End: {
                    rv = fseek(m_data->fh, where, SEEK_END);
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
            m_data->pos = ftell(m_data->fh);
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

        int32 rv = fread(data, size, 1, m_data->fh);
        if (rv < 0) {
            return -1;
        }

        m_data->pos += rv;

        return rv;
    }





}