// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include <apk/apk.h>
#include <apk/text.h>
#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/icon.h>
#include <workbench/startup.h>

namespace apk { namespace prefs {

	struct DiskObject* sDiskObject = NULL;

    const char* getPrefsString(const char* name, const char* defaultValue) {

        if (sDiskObject == NULL) {
            return defaultValue;
        }

        STRPTR tooltype = (STRPTR) FindToolType((CONST_STRPTR*) sDiskObject->do_ToolTypes, name);
        if (tooltype == NULL) {
            return defaultValue;
        }

        return tooltype;
    }

    uint32 getPrefsNumber(const char* name, uint32 defaultValue) {if (sDiskObject == NULL) {
            return defaultValue;
        }

        STRPTR tooltype = (STRPTR) FindToolType((CONST_STRPTR*) sDiskObject->do_ToolTypes, name);

        if (tooltype == NULL) {
            return defaultValue;
        }

        return string_to_uint32(tooltype);
    }

}}