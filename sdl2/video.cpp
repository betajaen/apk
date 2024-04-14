// APK - Copyright (c) 2024 by Robin Southern. https://github.com/betajaen/apk
// Licensed under the MIT License; see LICENSE file.

#include <apk/apk.h>
#include <apk/bitmap.h>
#include <apk/sdl2/emulation.h>

#include <SDL2/SDL.h>

namespace apk {

    namespace bank {
        void* getSpriteBankData(int32 bankNum, uint16 spriteNum, uint32* outSize, uint16* outWidth, uint16* outHeight, int16* offsetX, int16* offsetY);
    }

    extern bool s_FastMode;
    extern uint32 s_FastModeTime;
    extern bool s_quitRequested;
    SDL_Window* s_screen = NULL;
    constexpr int32 kScreenScale = 3;
    SDL_Surface* s_screenSurface = NULL;
    byte* s_Plane0 = NULL;
    byte* s_Plane1 = NULL;
    SDL_Color s_virtualPalette[256] = { 0 };
    byte s_palette[256*3] = { 0 };
    byte s_fadePalette[256*3] = { 0 };
    bool s_paletteDirty = false;
    static int32 sPaletteFading;
    static int32 sPaletteFadeSteps;
    static int32 sPaletteFadeTime;
    static int32 sPaletteFadeDest;
    static bool  sPaletteDirty = false;
    uint32 s_PlaneWidth = 0, s_PlaneHeight = 0, s_PlaneWidthHeight = 0;
    
    static char sDebugStr[41];

    static int32 s_MouseX = 0, s_MouseY = 0;
    static EmulatedSpriteHardware s_SpriteHardware;
    static BitMapBank s_SpriteHardwareBank;

    void gameBeginPause();
    void gameEndPause();
    
    static uint32 SDL2_To_AmigaKey(SDL_Keycode kc) {

        switch(kc) {
            default:                   return -1;
            case SDLK_UNKNOWN:         return -1;
            case SDLK_RETURN:          return -1;
            case SDLK_ESCAPE:          return APKK_ESCAPE;
            case SDLK_BACKSPACE:       return APKK_BACKSPACE;
            case SDLK_TAB:             return APKK_TAB;
            case SDLK_SPACE:           return APKK_SPACE;
            case SDLK_EXCLAIM:         return -1;
            case SDLK_QUOTEDBL:        return -1;
            case SDLK_HASH:            return -1;
            case SDLK_PERCENT:         return -1;
            case SDLK_DOLLAR:          return -1;
            case SDLK_AMPERSAND:       return -1;
            case SDLK_QUOTE:           return APKK_QUOTE;
            case SDLK_LEFTPAREN:       return -1;
            case SDLK_RIGHTPAREN:      return -1;
            case SDLK_ASTERISK:        return -1;
            case SDLK_PLUS:            return -1;
            case SDLK_COMMA:           return -1;
            case SDLK_MINUS:           return -1;
            case SDLK_PERIOD:          return -1;
            case SDLK_SLASH:           return -1;
            case SDLK_0:               return APKK_0;
            case SDLK_1:               return APKK_1;
            case SDLK_2:               return APKK_2;
            case SDLK_3:               return APKK_3;
            case SDLK_4:               return APKK_4;
            case SDLK_5:               return APKK_5;
            case SDLK_6:               return APKK_6;
            case SDLK_7:               return APKK_7;
            case SDLK_8:               return APKK_8;
            case SDLK_9:               return APKK_9;
            case SDLK_COLON:           return -1;
            case SDLK_SEMICOLON:       return APKK_SEMICOLON;
            case SDLK_LESS:            return -1;
            case SDLK_EQUALS:          return -1;
            case SDLK_GREATER:         return -1;
            case SDLK_QUESTION:        return -1;
            case SDLK_AT:              return -1;
            case SDLK_LEFTBRACKET:     return -1;
            case SDLK_BACKSLASH:       return -1;
            case SDLK_RIGHTBRACKET:    return -1;
            case SDLK_CARET:           return -1;
            case SDLK_UNDERSCORE:      return -1;
            case SDLK_BACKQUOTE:       return -1;
            case SDLK_a:               return APKK_A;
            case SDLK_b:               return APKK_B;
            case SDLK_c:               return APKK_C;
            case SDLK_d:               return APKK_D;
            case SDLK_e:               return APKK_E;
            case SDLK_f:               return APKK_F;
            case SDLK_g:               return APKK_G;
            case SDLK_h:               return APKK_H;
            case SDLK_i:               return APKK_I;
            case SDLK_j:               return APKK_J;
            case SDLK_k:               return APKK_K;
            case SDLK_l:               return APKK_L;
            case SDLK_m:               return APKK_M;
            case SDLK_n:               return APKK_N;
            case SDLK_o:               return APKK_O;
            case SDLK_p:               return APKK_P;
            case SDLK_q:               return APKK_Q;
            case SDLK_r:               return APKK_R;
            case SDLK_s:               return APKK_S;
            case SDLK_t:               return APKK_T;
            case SDLK_u:               return APKK_U;
            case SDLK_v:               return APKK_V;
            case SDLK_w:               return APKK_W;
            case SDLK_x:               return APKK_X;
            case SDLK_y:               return APKK_Y;
            case SDLK_z:               return APKK_Z;
            case SDLK_CAPSLOCK:        return -1;
            case SDLK_F1:              return APKK_F1;
            case SDLK_F2:              return APKK_F2;
            case SDLK_F3:              return APKK_F3;
            case SDLK_F4:              return APKK_F4;
            case SDLK_F5:              return APKK_F5;
            case SDLK_F6:              return APKK_F6;
            case SDLK_F7:              return APKK_F7;
            case SDLK_F8:              return APKK_F8;
            case SDLK_F9:              return APKK_F9;
            case SDLK_F10:             return APKK_F10;
            case SDLK_F11:             return -1;
            case SDLK_F12:             return -1;
            case SDLK_PRINTSCREEN:     return -1;
            case SDLK_SCROLLLOCK:      return -1;
            case SDLK_PAUSE:           return -1;
            case SDLK_INSERT:          return -1;
            case SDLK_HOME:            return -1;
            case SDLK_PAGEUP:          return -1;
            case SDLK_DELETE:          return APKK_DELETE;
            case SDLK_END:             return -1;
            case SDLK_PAGEDOWN:        return -1;
            case SDLK_RIGHT:           return APKK_RIGHT;
            case SDLK_LEFT:            return APKK_LEFT;
            case SDLK_DOWN:            return APKK_DOWN;
            case SDLK_UP:              return APKK_UP;
            case SDLK_NUMLOCKCLEAR:    return -1;
            case SDLK_KP_DIVIDE:       return -1;
            case SDLK_KP_MULTIPLY:     return -1;
            case SDLK_KP_MINUS:        return -1;
            case SDLK_KP_PLUS:         return APKK_NP_PLUS;
            case SDLK_KP_ENTER:        return -1;
            case SDLK_KP_1:            return APKK_NP_1;
            case SDLK_KP_2:            return APKK_NP_2;
            case SDLK_KP_3:            return APKK_NP_3;
            case SDLK_KP_4:            return APKK_NP_4;
            case SDLK_KP_5:            return APKK_NP_5;
            case SDLK_KP_6:            return APKK_NP_6;
            case SDLK_KP_7:            return APKK_NP_7;
            case SDLK_KP_8:            return APKK_NP_8;
            case SDLK_KP_9:            return APKK_NP_9;
            case SDLK_KP_0:            return APKK_NP_0;
            case SDLK_KP_PERIOD:       return -1;
            case SDLK_APPLICATION:     return -1;
            case SDLK_POWER:           return -1;
            case SDLK_KP_EQUALS:       return -1;
            case SDLK_F13:             return -1;
            case SDLK_F14:             return -1;
            case SDLK_F15:             return -1;
            case SDLK_F16:             return -1;
            case SDLK_F17:             return -1;
            case SDLK_F18:             return -1;
            case SDLK_F19:             return -1;
            case SDLK_F20:             return -1;
            case SDLK_F21:             return -1;
            case SDLK_F22:             return -1;
            case SDLK_F23:             return -1;
            case SDLK_F24:             return -1;
            case SDLK_EXECUTE:         return -1;
            case SDLK_HELP:            return APKK_HELP;
            case SDLK_MENU:            return -1;
            case SDLK_SELECT:          return -1;
            case SDLK_STOP:            return -1;
            case SDLK_AGAIN:           return -1;
            case SDLK_UNDO:            return -1;
            case SDLK_CUT:             return -1;
            case SDLK_COPY:            return -1;
            case SDLK_PASTE:           return -1;
            case SDLK_FIND:            return -1;
            case SDLK_MUTE:            return -1;
            case SDLK_VOLUMEUP:        return -1;
            case SDLK_VOLUMEDOWN:      return -1;
            case SDLK_KP_COMMA:        return -1;
            case SDLK_KP_EQUALSAS400:  return -1;
            case SDLK_ALTERASE:        return -1;
            case SDLK_SYSREQ:          return -1;
            case SDLK_CANCEL:          return -1;
            case SDLK_CLEAR:           return -1;
            case SDLK_PRIOR:           return -1;
            case SDLK_RETURN2:         return -1;
            case SDLK_SEPARATOR:       return -1;
            case SDLK_OUT:             return -1;
            case SDLK_OPER:            return -1;
            case SDLK_CLEARAGAIN:      return -1;
            case SDLK_CRSEL:           return -1;
            case SDLK_EXSEL:           return -1;
            case SDLK_KP_00:           return -1;
            case SDLK_KP_000:          return -1;
            case SDLK_THOUSANDSSEPARATOR:return -1;
            case SDLK_DECIMALSEPARATOR:return -1;
            case SDLK_CURRENCYUNIT:    return -1;
            case SDLK_CURRENCYSUBUNIT: return -1;
            case SDLK_KP_LEFTPAREN:    return -1;
            case SDLK_KP_RIGHTPAREN:   return -1;
            case SDLK_KP_LEFTBRACE:    return -1;
            case SDLK_KP_RIGHTBRACE:   return -1;
            case SDLK_KP_TAB:          return -1;
            case SDLK_KP_BACKSPACE:    return -1;
            case SDLK_KP_A:            return -1;
            case SDLK_KP_B:            return -1;
            case SDLK_KP_C:            return -1;
            case SDLK_KP_D:            return -1;
            case SDLK_KP_E:            return -1;
            case SDLK_KP_F:            return -1;
            case SDLK_KP_XOR:          return -1;
            case SDLK_KP_POWER:        return -1;
            case SDLK_KP_PERCENT:      return -1;
            case SDLK_KP_LESS:         return -1;
            case SDLK_KP_GREATER:      return -1;
            case SDLK_KP_AMPERSAND:    return -1;
            case SDLK_KP_DBLAMPERSAND: return -1;
            case SDLK_KP_VERTICALBAR:  return -1;
            case SDLK_KP_DBLVERTICALBAR:return -1;
            case SDLK_KP_COLON:        return -1;
            case SDLK_KP_HASH:         return -1;
            case SDLK_KP_SPACE:        return -1;
            case SDLK_KP_AT:           return -1;
            case SDLK_KP_EXCLAM:       return -1;
            case SDLK_KP_MEMSTORE:     return -1;
            case SDLK_KP_MEMRECALL:    return -1;
            case SDLK_KP_MEMCLEAR:     return -1;
            case SDLK_KP_MEMADD:       return -1;
            case SDLK_KP_MEMSUBTRACT:  return -1;
            case SDLK_KP_MEMMULTIPLY:  return -1;
            case SDLK_KP_MEMDIVIDE:    return -1;
            case SDLK_KP_PLUSMINUS:    return -1;
            case SDLK_KP_CLEAR:        return -1;
            case SDLK_KP_CLEARENTRY:   return -1;
            case SDLK_KP_BINARY:       return -1;
            case SDLK_KP_OCTAL:        return -1;
            case SDLK_KP_DECIMAL:      return -1;
            case SDLK_KP_HEXADECIMAL:  return -1;
            case SDLK_LCTRL:           return -1;
            case SDLK_LSHIFT:          return APKK_LSHIFT;
            case SDLK_LALT:            return APKK_LALT;
            case SDLK_LGUI:            return -1;
            case SDLK_RCTRL:           return -1;
            case SDLK_RSHIFT:          return APKK_RSHIFT;
            case SDLK_RALT:            return APKK_RALT;
            case SDLK_RGUI:            return -1;
            case SDLK_MODE:            return -1;
            case SDLK_AUDIONEXT:       return -1;
            case SDLK_AUDIOPREV:       return -1;
            case SDLK_AUDIOSTOP:       return -1;
            case SDLK_AUDIOPLAY:       return -1;
            case SDLK_AUDIOMUTE:       return -1;
            case SDLK_MEDIASELECT:     return -1;
            case SDLK_WWW:             return -1;
            case SDLK_MAIL:            return -1;
            case SDLK_CALCULATOR:      return -1;
            case SDLK_COMPUTER:        return -1;
            case SDLK_AC_SEARCH:       return -1;
            case SDLK_AC_HOME:         return -1;
            case SDLK_AC_BACK:         return -1;
            case SDLK_AC_FORWARD:      return -1;
            case SDLK_AC_STOP:         return -1;
            case SDLK_AC_REFRESH:      return -1;
            case SDLK_AC_BOOKMARKS:    return -1;
            case SDLK_BRIGHTNESSDOWN:  return -1;
            case SDLK_BRIGHTNESSUP:    return -1;
            case SDLK_DISPLAYSWITCH:   return -1;
            case SDLK_KBDILLUMTOGGLE:  return -1;
            case SDLK_KBDILLUMDOWN:    return -1;
            case SDLK_KBDILLUMUP:      return -1;
            case SDLK_EJECT:           return -1;
            case SDLK_SLEEP:           return -1;
            case SDLK_APP1:            return -1;
            case SDLK_APP2:            return -1;
            case SDLK_AUDIOREWIND:     return -1;
            case SDLK_AUDIOFASTFORWARD:return -1;
            case SDLK_SOFTLEFT:        return -1;
            case SDLK_SOFTRIGHT:       return -1;
            case SDLK_CALL:            return -1;
            case SDLK_ENDCALL:         return -1;
        }

        return -1;
    }

    namespace video {


    bool createScreen(const char* title, uint16 width, uint16 height, uint8 depth) {
        SDL_assert(s_screen == NULL);

        s_screen = SDL_CreateWindow(title,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width * kScreenScale,
            height * kScreenScale,
            SDL_WINDOW_SHOWN);

        SDL_assert(s_screen);


        s_screenSurface = SDL_GetWindowSurface(s_screen);

        s_PlaneWidth = width;
        s_PlaneHeight = height;
        s_PlaneWidthHeight = width * height;

        s_Plane0 = (byte*) apk_allocate(s_PlaneWidthHeight);
        s_Plane1 = (byte*) apk_allocate(s_PlaneWidthHeight);
        
        SDL_ShowCursor(SDL_DISABLE);

        return true;
    }

    void destroyScreen() {

        SDL_ShowCursor(SDL_ENABLE);

        if (s_Plane0 != NULL) {
            apk_deallocate(s_Plane0);
            s_Plane0 = NULL;
        }

        if (s_Plane1 != NULL) {
            apk_deallocate(s_Plane1);
            s_Plane1 = NULL;
        }

        if(s_screen != NULL) {
            s_screen = NULL;
            s_PlaneWidth = 0;
            s_PlaneHeight = 0;
        }
    }

    static void scaleCopy(SDL_Surface* dst, byte* plane0, byte* plane1, uint32 scale, uint32 w, uint32 h) {

        SDL_LockSurface(dst);

        uint8* s0 = plane0;
        uint8* s1 = plane1;
        uint8* d = (uint8*)dst->pixels;
        const APK_SIZE_TYPE stride =  w * 4 * scale;
        uint8 line[stride];

        for(uint32 y=0;y < h;y++) {

            uint8* l0 = s0, *l1 = s1;
            uint8* t = line;

            for (uint32 x=0;x < w;x++) {
                uint8 idx0 = *l0;
                uint8 idx1 = *l1;
                
                SDL_Color col0 = s_virtualPalette[idx1 ? idx1 : idx0];

                for (uint32 j=0;j < scale;j++) {
                    *t++ = col0.b;
                    *t++ = col0.g;
                    *t++ = col0.r;
                    *t++ = 0xFF;
                }

                l0++;
                l1++;
            }

            for (uint32 j=0;j < scale;j++) {
                memcpy(d, line, sizeof(line));
                d += sizeof(line);
            }

            s0 += w;
            s1 += w;
        }

        SDL_UnlockSurface(dst);
    }

    static void surfaceCopy() {
        memset_aligned(s_Plane1, 0, s_PlaneWidth * s_PlaneHeight);
        s_SpriteHardware.blitChunky(s_Plane1, s_PlaneWidth, s_PlaneHeight, s_PlaneWidth);


        scaleCopy(s_screenSurface, s_Plane0, s_Plane1, kScreenScale, s_PlaneWidth, s_PlaneHeight);

        SDL_UpdateWindowSurface(s_screen);
        if (s_FastMode) {
            s_FastModeTime++;
        }
    }

    static void mouseCopy() {

    }

    void blit(uint8* data, uint32 size) {

        assert(size <= s_PlaneWidthHeight);

        uint8* pixels = (uint8*)s_Plane0;
        memcpy(pixels, data, size);
    }

    void cls(uint8 index) {
        memset(s_Plane0, index, s_PlaneWidthHeight);
    }

    void fillRect(uint32 l, uint32 t, uint32 w, uint32 h, uint8 col) {
        uint32 x0 = MAX(0U, l);
        uint32 y0 = MAX(0U, t);
        uint32 x1 = MIN(x0 + w, s_PlaneWidth);
        uint32 y1 = MIN(y0 + h, s_PlaneHeight);

        for(uint32 j=y0;j < y1;j++) {
            for(uint32 i=x0;i < x1;i++) {
                s_Plane0[i + j * s_PlaneWidth] = col;
            }
        }
    }

    void pasteIcon(uint8* img, uint32 x, uint32 y, uint32 w, uint32 h, uint8 transparent, uint8* pal) {
        uint32 x0 = MAX(0U, x);
        uint32 y0 = MAX(0U, y);
        uint32 x1 = MIN(x0 + w, s_PlaneWidth);
        uint32 y1 = MIN(y0 + h, s_PlaneHeight);

		uint32 srcX0 = 0, srcY0 = 0;
        for(int32 j=y0;j < y1;j++) {
			uint32 dstIdx = (j * s_PlaneWidth);
            uint32 srcIdx = (srcY0 * w);
            uint32 srcX0 = 0;
			for(int32 i=x0;i < x1;i++) {
				uint8 colour = img[srcIdx + srcX0];
                if (colour != transparent) {
				    s_Plane0[dstIdx + i] = pal[colour];
                }
                srcX0++;
			}
            srcY0++;
		}
    }

    void setRGB(uint8 index, uint8 r, uint8 g, uint8 b) {
        s_palette[index*3 + 0] = r;
        s_palette[index*3 + 1] = g;
        s_palette[index*3 + 2] = b;
        
        s_paletteDirty = true;
    }

    void setRGB(uint8* pal, uint32 begin, uint32 end) {
        for(int i=begin;i < end;i++) {
            uint8 r = *pal++;
            uint8 g = *pal++;
            uint8 b = *pal++;
            setRGB(i, r, g, b);
        }
    }

    void forceUpdateScreen() {
        surfaceCopy();
    }
    
    void writeChunkyPixels(uint8* data) {
        blit(data, s_PlaneWidthHeight);
        surfaceCopy();
    }

    void clearChunkyPixels(uint8 index) {
        cls(index);
        surfaceCopy();
    }

    void clearPalette() {
        for(uint32 i=0;i < 255*3;i++) {
            s_palette[i] = 0;
        }
        s_paletteDirty = true;
    }

    void paletteFadeIn(uint32 steps) {
        if (sPaletteFading == 0) {
            sPaletteFadeSteps = (int32) CLIP((int32)steps, (int32)1, (int32)255);
            sPaletteFadeTime = -255;
            sPaletteFadeDest = 0;
            sPaletteFading = 1;
        }
    }

    void paletteFadeOut(uint32 steps) {
        if (sPaletteFading == 0) {
            sPaletteFadeSteps = -(int32) CLIP((int32)steps, (int32)1, (int32)255);
            sPaletteFadeTime = 255;
            sPaletteFadeDest = 0;
            sPaletteFading = -1;
        }
    }

    static void loadPalette(byte* palette) {
        for(uint32 i=0;i < 256;i++) {
            SDL_Color col;
            col.r = *palette++;
            col.g = *palette++;
            col.b = *palette++;
            col.a = 0xFF;
            s_virtualPalette[i] = col;
        }
    }

    static void paletteFunction() {

        if (sPaletteFading == 0) {
            if (s_paletteDirty) {
                s_paletteDirty = false;
                loadPalette(s_palette);
            }
        }
        else {
            s_paletteDirty = false;

            uint8* src = s_palette;
            uint8* dst = s_fadePalette;

            for(uint32 i=0;i < 256*3;i++) {
                int32 col = *src;
                col += sPaletteFadeTime;
                if (col < 0) {
                    col = 0;
                }
                else if (col > 255) {
                    col = 255;
                }
                *dst = col;
                src++;
                dst++;
            }
            
            sPaletteFadeTime += sPaletteFadeSteps;

            if (sPaletteFading == -1) {
                if (sPaletteFadeTime > sPaletteFadeDest) {
                    loadPalette(s_fadePalette);
                    return;
                }
            }
            else {
                if (sPaletteFadeTime < sPaletteFadeDest) {
                    loadPalette(s_fadePalette);
                    return;
                }
            }

            sPaletteFading = 0;
            loadPalette(s_palette);

        }

    }

    typedef void(*WindowEventFn)(void* user, Event& evt);
    typedef void(*WindowTimerFn)(void* user);

    template<typename T>
    struct UserCallback {
        T fn;
        void* data;

        UserCallback() = default;

        UserCallback(T fn_, void* data_)
            : fn(fn_), data(data_) {
        }

    };

    Stack<UserCallback<WindowEventFn>, 4> s_EventFns;
    Stack<UserCallback<WindowTimerFn>, 4> s_TimerFns;

#define USER_EVENT_TIMER 1
#define USER_EVENT_QUIT 2
    uint32 s_UserEventType;

    static Uint32 sdlTimerCallback(Uint32 interval, void* param) {
        SDL_Event timerEvent;
        timerEvent.type = s_UserEventType;
        timerEvent.user.code = USER_EVENT_TIMER;
        timerEvent.user.data1 = NULL;
        timerEvent.user.data2 = NULL;
        SDL_PushEvent(&timerEvent);
        return interval;
    }

    void windowStartLoop(uint32 waitTime_usec) {
        bool stopLoop = false;
        uint32 waitTime_msec = waitTime_usec / 1000;
        s_UserEventType = SDL_RegisterEvents(1);
        SDL_TimerID timer = SDL_AddTimer(waitTime_msec, sdlTimerCallback, NULL);
        SDL_Event evt;

        uint32 mouseX = 0, mouseY = 0;
        int32 reportedMouse = -1;
        bool isPaused = false;

        while(stopLoop == false) {
            bool forceDraw = false;
            SDL_WaitEvent(&evt);

            if (isPaused) {
                if (evt.type == SDL_KEYUP) {
                    int32 kc = SDL2_To_AmigaKey(evt.key.keysym.sym);

                    if (kc == APKK_SPACE) {
                        isPaused = false;
                        apk::gameEndPause();
                        continue;
                    }
                }
                continue;
            }

            if (evt.type == s_UserEventType) {
                    if (evt.user.code == USER_EVENT_TIMER) {
                        
                        if (reportedMouse == 0) {
                            Event evt;
                            evt.type = EVENT_MOUSEMOVE;
                            evt.mouse.x = mouseX;
                            evt.mouse.y = mouseY;
                            const auto& wcb = s_EventFns.top();
                            wcb.fn(wcb.data, evt);
                            reportedMouse = -1;
                        }

                        const auto& cb = s_TimerFns.top();
                        cb.fn(cb.data);

                        paletteFunction();
                    }
                    else if (evt.user.code == USER_EVENT_QUIT) {
                        stopLoop = true;
                    }
            }
            else {
                switch (evt.type) {
                    case SDL_QUIT: {
                        stopLoop = true;
                    }
                    break;
                    case SDL_KEYUP: {
                        int32 kc = SDL2_To_AmigaKey(evt.key.keysym.sym);

                        if (kc == APKK_SPACE) {
                            isPaused = true;
                            apk::gameBeginPause();
                            continue;
                        }

                        Event e;
                        e.type = EVENT_KEYINSTANT;
                        e.kbd.keycode = SDL2_To_AmigaKey(evt.key.keysym.sym);
                        e.kbd.shift = evt.key.keysym.mod & KMOD_LSHIFT || evt.key.keysym.mod & KMOD_RSHIFT;


                        const auto& cb = s_EventFns.top();
                        cb.fn(cb.data, e);
                    }
                    break;
                    case SDL_MOUSEMOTION:
                    {
                        mouseX = evt.button.x / kScreenScale;
                        mouseY = evt.button.y / kScreenScale;
                        s_MouseX = mouseX;
                        s_MouseY = mouseY;
                        reportedMouse = 0;
                        s_SpriteHardware.getSprite(0)->setXY(mouseX, mouseY);
                        forceDraw = true;
                    }
                    break;
                    case SDL_MOUSEBUTTONUP:
                    case SDL_MOUSEBUTTONDOWN:
                    {
                        Event e;
                        e.type = EVENT_NONE;
                        mouseX = evt.button.x / kScreenScale;
                        mouseY = evt.button.y / kScreenScale;
                        s_MouseX = mouseX;
                        s_MouseY = mouseY;
                        reportedMouse = 1;

                        e.mouse.x = mouseX;
                        e.mouse.y = mouseY;
                        s_SpriteHardware.getSprite(0)->setXY(mouseX, mouseY);
                        forceDraw = true;

                        if (evt.button.button == SDL_BUTTON_LEFT) {
                            if (evt.type == SDL_MOUSEBUTTONUP) {
                                e.type = EVENT_LBUTTONUP;
                            }
                            else {
                                e.type = EVENT_LBUTTONDOWN;
                            }
                        }
                        else if (evt.button.button == SDL_BUTTON_RIGHT){
                            if (evt.type == SDL_MOUSEBUTTONUP) {
                                e.type = EVENT_RBUTTONUP;
                            }
                            else {
                                e.type = EVENT_RBUTTONDOWN;
                            }
                        }

                        if (e.type != EVENT_NONE) {
                            const auto& cb = s_EventFns.top();
                            cb.fn(cb.data, e);

                            if (reportedMouse == 1) {
                                reportedMouse = 0;
                            }
                        }
                    }
                    break;
                }
            }

            if (forceDraw) {
                surfaceCopy();
            }

        }
        SDL_RemoveTimer(timer);
    }

    void windowStopLoop() {
        SDL_Event stopEvent;
        stopEvent.type = s_UserEventType;
        stopEvent.user.code = USER_EVENT_QUIT;
        stopEvent.user.data1 = NULL;
        stopEvent.user.data2 = NULL;
        SDL_PushEvent(&stopEvent);
    }

    void pushWindowEventCallback(WindowEventFn cb, void* data) {
            s_EventFns.push_back(UserCallback<WindowEventFn>(cb, data));
    }
    void pushWindowTimerCallback(WindowTimerFn cb, void* data) {
        s_TimerFns.push_back(UserCallback<WindowTimerFn>(cb, data));
    }

    void popWindowEventCallback() {
        s_EventFns.pop_back();
    }
    void popWindowTimerCallback() {
        s_TimerFns.pop_back();
    }

    void createSpriteBitMapFromChunky(uint32 id, uint16 w, uint16 h, uint8* data) {
        BitMap* bitMap = s_SpriteHardwareBank.createBitMap(id, w, h, 0);
        bitMap->copyFrom(data);
    }

    void setSpriteBanked(uint8 spriteNum, uint16 bitMapId) {
        Bob* bob = s_SpriteHardware.getSprite(spriteNum);
        if (bob) {
            BitMap* bitMap = s_SpriteHardwareBank.findBitMap(bitMapId);
            if (bitMap) {
                bob->setBitMap(bitMap, false);
            }
        }
    }

    void setSpriteVisible(uint8 spriteNum, bool isVisible) {
        s_SpriteHardware.setVisible(spriteNum, isVisible);
    }

    void setSpritePosition(uint8 spriteNum, int32 x, int32 y) {
        Bob* bob = s_SpriteHardware.getSprite(spriteNum);
        if (bob) {
            bob->setXY(x, y);
        }
    }

    void setSpriteOffset(uint8 spriteNum, int32 offsetX, int32 offsetY) {
        Bob* bob = s_SpriteHardware.getSprite(spriteNum);
        if (bob) {
            bob->setCenterXY(offsetX, offsetY);
        }
    }

    int32 getSpritePositionX(uint8 spriteNum) {
        Bob* bob = s_SpriteHardware.getSprite(spriteNum);
        if (bob) {
            return bob->getX();
        }
        return 0;
    }

    int32 getSpritePositionY(uint8 spriteNum) {
        Bob* bob = s_SpriteHardware.getSprite(spriteNum);
        if (bob) {
            return bob->getY();
        }
        return 0;
    }
    

}}

namespace apk {
    void debug_str(const char* str) {
        apk::strcpy_s(sDebugStr, sizeof(sDebugStr), str);
    }
}
