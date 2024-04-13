// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "pod.h"
#include "compat.h"
#include "consts.h"
#include "array.h"
#include "list.h"
#include "assert.h"
#include "file.h"
#include "endian.h"
#include "memory.h"
#include "input.h"
#include "requester.h"
#include "math.h"
#include "stack.h"
#include "text.h"
#include "time.h"
#include "debug.h"
#include "pool.h"
#include "bank.h"

namespace apk {


    template<typename T>
    void SWAP(T& l, T& r) {
        T t = l;
        l = r;
        r = t;
    }

    template<typename T>
    void SORT(T& l, T& r) {
        if (l > r) {
            SWAP(l, r);
        }
    }

    bool isQuitRequested();



}


#define APK_ALIGNED /* aligned */

#define ARRAYSIZE(X) ((sizeof(X)) / (sizeof(X[0])))