// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include "apk/apk.h"
#include "apk/assert.h"

#include <proto/exec.h>
#include <proto/dos.h>

typedef VOID(*PUTCHARPROC)();

static const ULONG PutChar = 0x16c04e75;
static const ULONG LenChar = 0x52934e75;
static char BufChar[1024] = { 0 };

namespace apk { namespace video {
    void setDebugNum(uint32 num);
    void setDebugStr(const char* str);
    void clearDebug();
}}

namespace apk {

    void printf(const char* fmt, ...) {
		const char* arg = (const char*)(&fmt + 1);
		RawDoFmt((CONST_STRPTR) fmt, (APTR) arg, (PUTCHARPROC)&PutChar, &BufChar[0]);
		PutStr(BufChar);
    }

    void debug(int l, const char* fmt, ...) {
		const char* arg = (const char*)(&fmt + 1);
		RawDoFmt((CONST_STRPTR) fmt, (APTR) arg, (PUTCHARPROC)&PutChar, &BufChar[0]);
        PutStr("[D] ");
		PutStr(BufChar);
        PutStr("\n");
    }

    void debug(const char* fmt, ...) {
		const char* arg = (const char*)(&fmt + 1);
		RawDoFmt((CONST_STRPTR) fmt, (APTR) arg, (PUTCHARPROC)&PutChar, &BufChar[0]);
        PutStr("[D] ");
		PutStr(BufChar);
        PutStr("\n");
    }

    void warning(const char* fmt, ...) {
		const char* arg = (const char*)(&fmt + 1);
		RawDoFmt((CONST_STRPTR) fmt, (APTR) arg, (PUTCHARPROC)&PutChar, &BufChar[0]);
        PutStr("[W] ");
		PutStr(BufChar);
        PutStr("\n");
    }

    void error(const char* fmt, ...) {
		const char* arg = (const char*)(&fmt + 1);
		RawDoFmt((CONST_STRPTR) fmt, (APTR) arg, (PUTCHARPROC)&PutChar, &BufChar[0]);
        PutStr("[E] ");
		PutStr(BufChar);
        PutStr("\n");
    }

    void doAssert(const char* file, int line) {
        static char tmp[200];
        sprintf_s(tmp, sizeof(tmp), "Program asserted at\n%s:%ld", file, line);
        requester_okay("Assert!", tmp);
        while(1);
    }


    void debug_num(uint32 num) {
        apk::video::setDebugNum(num);
    }

    void debug_str(const char* str) {
        apk::video::setDebugStr(str);
    }

    void debug_clear() {
        apk::video::clearDebug();
    }

}