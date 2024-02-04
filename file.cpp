// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include <apk/file.h>
#include "file.h"

namespace apk {


    inline int32 StreamData<ReadFile>::read(void *dst, int32 size, int32 upper_bound)
    {
        uint32 end = udata.pos() + size;
        if (end >= upper_bound) {
            size = upper_bound - udata.pos();
        }
        return udata.read(dst, size);
    }

    int32 StreamData<ReadFile>::seek(int32 offset, SeekMode where, int32 lower_bound, int32 upper_bound)
    {
        switch(where) {
            default:
                return -1;
            case SeekMode::Set:
            {
                uint32 set_offset = lower_bound + offset;
                if (set_offset < 0 || set_offset < lower_bound || set_offset >= upper_bound)
                    return -1;
                
                return udata.seek(set_offset, SeekMode::Set);
            }
            case SeekMode::Current:
            {
                uint32 cur_offset = udata.pos() + offset;
                if (cur_offset < 0 || cur_offset < lower_bound || cur_offset >= upper_bound)
                    return -1;
                return udata.seek(offset, SeekMode::Current);
            }
            case SeekMode::End:
            {
                uint32 end_offset = upper_bound + offset;
                if (end_offset < 0 || end_offset < lower_bound || end_offset >= upper_bound)
                    return -1;

                return udata.seek(offset, SeekMode::End);
            }
            case SeekMode::GetPos:
            {
                return udata.pos();
            }
            case SeekMode::GetSize:
            {
                return udata.size();
            }
            case SeekMode::CanSeek:
            {
                return 1;
            }
        }

        return -1;
    }

    bool StreamData<ReadFile>::close()
    {
        return udata.close();
    }

}