// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#pragma once

#include "apk.h"

#define APKK_INVALID            -1
#define APKK_TIDLE              0x00
#define APKK_1                  0x01
#define APKK_2                  0x02
#define APKK_3                  0x03
#define APKK_4                  0x04
#define APKK_5                  0x05
#define APKK_6                  0x06
#define APKK_7                  0x07
#define APKK_8                  0x08
#define APKK_9                  0x09
#define APKK_0                  0x0A
#define APKK_KEY_0B             0x0B
#define APKK_KEY_0C             0x0C
#define APKK_KEY_0D             0x0D
#define APKK_KEY_0E             0x0E
#define APKK_NP_0               0x0F
#define APKK_Q                  0x10
#define APKK_W                  0x11
#define APKK_E                  0x12
#define APKK_R                  0x13
#define APKK_T                  0x14
#define APKK_Y                  0x15
#define APKK_U                  0x16
#define APKK_I                  0x17
#define APKK_O                  0x18
#define APKK_P                  0x19
#define APKK_KEY_1A             0x1A
#define APKK_KEY_1B             0x1B
#define APKK_KEY_1C             0x1C
#define APKK_NP_1               0x1D
#define APKK_NP_2               0x1E
#define APKK_NP_3               0x1F
#define APKK_A                  0x20
#define APKK_S                  0x21
#define APKK_D                  0x22
#define APKK_F                  0x23
#define APKK_G                  0x24
#define APKK_H                  0x25
#define APKK_J                  0x26
#define APKK_K                  0x27
#define APKK_L                  0x28
#define APKK_SEMICOLON          0x29
#define APKK_QUOTE              0x2A
#define APKK_KEY_2B             0x2B
#define APKK_KEY_2C             0x2C
#define APKK_NP_4               0x2D
#define APKK_NP_5               0x2E
#define APKK_NP_6               0x2F
#define APKK_KEY_30             0x30
#define APKK_Z                  0x31
#define APKK_X                  0x32
#define APKK_C                  0x33
#define APKK_V                  0x34
#define APKK_B                  0x35
#define APKK_N                  0x36
#define APKK_M                  0x37
#define APKK_KEY_38             0x38
#define APKK_KEY_39             0x39
#define APKK_KEY_3A             0x3A
#define APKK_KEY_3B             0x3B
#define APKK_NP_DOT             0x3C
#define APKK_NP_7               0x3D
#define APKK_NP_8               0x3E
#define APKK_NP_9               0x3F
#define APKK_SPACE              0x40
#define APKK_BACKSPACE          0x41
#define APKK_TAB                0x42
#define APKK_ENTER              0x43
#define APKK_RETURN             0x44
#define APKK_ESCAPE             0x45
#define APKK_DELETE             0x46
#define APKK_KEY_47             0x47
#define APKK_KEY_48             0x48
#define APKK_KEY_49             0x49
#define APKK_NP_MINUS           0x4A
#define APKK_KEY_4B             0x4B
#define APKK_UP                 0x4C
#define APKK_DOWN               0x4D
#define APKK_RIGHT              0x4E
#define APKK_LEFT               0x4F
#define APKK_F1                 0x50
#define APKK_F2                 0x51
#define APKK_F3                 0x52
#define APKK_F4                 0x53
#define APKK_F5                 0x54
#define APKK_F6                 0x55
#define APKK_F7                 0x56
#define APKK_F8                 0x57
#define APKK_F9                 0x58
#define APKK_F10                0x59
#define APKK_NP_LEFT_PAREN      0x5A
#define APKK_NP_RIGHT_PAREN     0x5B
#define APKK_NP_SLASH           0x5C
#define APKK_NP_MULTIPLY        0x5D
#define APKK_NP_PLUS            0x5E
#define APKK_HELP               0x5F
#define APKK_LSHIFT             0x60
#define APKK_RSHIFT             0x61
#define APKK_KEY_62             0x62
#define APKK_CONTROL            0x63
#define APKK_LALT               0x64
#define APKK_RALT               0x65
#define APKK_LAMIGA             0x66
#define APKK_RAMIGA             0x67
#define APKK_KEY_68             0x68
#define APKK_KEY_69             0x69
#define APKK_KEY_6A             0x6A
#define APKK_KEY_6B             0x6B
#define APKK_KEY_6C             0x6C
#define APKK_KEY_6D             0x6D
#define APKK_KEY_6E             0x6E
#define APKK_KEY_6F             0x6F
#define APKK_KEY_70             0x70
#define APKK_KEY_71             0x71
#define APKK_KEY_72             0x72
#define APKK_KEY_73             0x73
#define APKK_KEY_74             0x74
#define APKK_KEY_75             0x75
#define APKK_KEY_76             0x76
#define APKK_KEY_77             0x77
#define APKK_KEY_78             0x78
#define APKK_KEY_79             0x79
#define APKK_KEY_7A             0x7A
#define APKK_KEY_7B             0x7B
#define APKK_KEY_7C             0x7C
#define APKK_KEY_7D             0x7D
#define APKK_KEY_7E             0x7E
#define APKK_KEY_7F             0x7F

namespace apk {

    constexpr int32 EVENT_NONE = 0;
    constexpr int32 EVENT_LBUTTONDOWN = 1;
    constexpr int32 EVENT_LBUTTONUP = 2;
    constexpr int32 EVENT_RBUTTONDOWN = 3;
    constexpr int32 EVENT_RBUTTONUP = 4;
    constexpr int32 EVENT_MOUSEMOVE = 5;
    constexpr int32 EVENT_KEYDOWN = 6;
    constexpr int32 EVENT_KEYUP = 7;
    constexpr int32 EVENT_KEYINSTANT = 8;
    constexpr int32 EVENT_QUIT = 9;
    constexpr int32 EVENT_RETURN_TO_LAUNCHER = 10;
    constexpr int32 EVENT_FAST_MODE = 11;
    constexpr int32 EVENT_SKIP_PROTECTION = 12;
    constexpr int32 EVENT_PAUSE = 13;
    constexpr int32 EVENT_MENU = 14;

    struct Event {
        int32 type;
        struct Mouse {
            int16 x, y;
        } mouse;
        struct Keyboard {
            int32 keycode;
            bool shift;
            bool hasFlags(int32) {
                return false;
            }
        } kbd;
    };

}