// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include "apk/apk.h"

#include <proto/exec.h>
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/asl.h>

#include <dos/dos.h>
#include <workbench/workbench.h>
#include <inline/exec.h>
#include <inline/dos.h>

namespace apk {

    static char fname[255];

    const char* normalizePath(const char* drawer, const char* file, const char* extension) {
        strcpy_s(fname, sizeof(fname), drawer);
        AddPart(fname, file, sizeof(fname));

        if (string_endswith(fname, extension) == false) {
            sprintf_s(fname, sizeof(fname), "%s%s", fname, extension);
        }

        return fname;
    }

    const char* requester_load(const char* title, const char* drawer, const char* extension) {
        struct FileRequester *request;
        char pattern[24];

        if (extension != NULL) {
            sprintf_s(pattern, sizeof(pattern), "#?%s", extension);
        }
        else {
            strcpy_s(pattern, sizeof(pattern), "#?");
        }

        request = (struct FileRequester *)AllocAslRequestTags(ASL_FileRequest,
          ASL_Hail, (ULONG) title,
            ASLFR_InitialDrawer, (ULONG) drawer,
            ASLFR_InitialPattern, (ULONG) pattern,
            TAG_DONE);

        int rv = AslRequestTags(request, TAG_DONE);

        if (rv == FALSE) {
            FreeAslRequest(request);
            return NULL;
        }



        normalizePath(request->fr_Drawer, request->fr_File, extension);

        FreeAslRequest(request);
        return fname;
    }

    const char* requester_save(const char* title, const char* drawer, const char* extension) {
        struct FileRequester *request;
        char pattern[24];

        if (extension != NULL) {
            sprintf_s(pattern, sizeof(pattern), "#?.%s", extension);
        }
        else {
            strcpy_s(pattern, sizeof(pattern), "#?");
        }

        request = (struct FileRequester *)AllocAslRequestTags(ASL_FileRequest,
          ASL_Hail, (ULONG) title,
            ASLFR_InitialDrawer, (ULONG) drawer,
            ASLFR_InitialPattern, (ULONG) pattern,
            ASLFR_Flags1, FRF_DOSAVEMODE,
          TAG_DONE);

        int rv = AslRequestTags(request, TAG_DONE);

        if (rv == FALSE) {
            FreeAslRequest(request);
            return NULL;
        }

        normalizePath(request->fr_Drawer, request->fr_File, extension);

        FreeAslRequest(request);
        return fname;
    }

    void requester_okay(const char* title, const char* text) {
        EasyStruct str;
        str.es_StructSize = sizeof(EasyStruct);
        str.es_Flags = 0;
        str.es_GadgetFormat = (CONST_STRPTR)"OK";
        str.es_TextFormat = (CONST_STRPTR)text;
        str.es_Title = (CONST_STRPTR)title;

        EasyRequest(NULL, &str, NULL);
    }

    int32 requester_yesno(const char* title, const char* text) {
        EasyStruct str;
        str.es_StructSize = sizeof(EasyStruct);
        str.es_Flags = 0;
        str.es_GadgetFormat = (CONST_STRPTR)"YES|NO";
        str.es_TextFormat = (CONST_STRPTR)text;
        str.es_Title = (CONST_STRPTR)title;

        int rv = EasyRequest(NULL, &str, NULL);

        return rv;
    }

    int32 requester_options(const char* title, const char* text, const char* options) {
        EasyStruct str;
        str.es_StructSize = sizeof(EasyStruct);
        str.es_Flags = 0;
        str.es_GadgetFormat = (CONST_STRPTR)options;
        str.es_TextFormat = (CONST_STRPTR)text;
        str.es_Title = (CONST_STRPTR)title;

        int rv = EasyRequest(NULL, &str, NULL);

        return rv;
    }

}