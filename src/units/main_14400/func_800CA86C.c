#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { char pad0[0x4C]; void *vtbl; char pad50[0x78 - 0x50]; } Window;
typedef struct { char data[0x20]; } Rect;
extern s32 D_801528D0[];
extern s32 D_80151E38[];
extern char D_80138D10[];
extern char D_80138D20[];
Window *func_800953C0(Window *);
s32 func_800CA76C(void *, u8);
char *func_80048480(u16);
s32 func_800327C0(char *, const char *, ...);
void func_8009C374(Window *, char *, char *);
s32 func_800957C0(Window *, Rect *, s32, void *, s32);
void func_800CA86C(void *arg) {
    Window win;
    Window *w;
    char text[0x100];
    Rect rect;
    s32 i;
    w = &win;
    func_800953C0(w);
    w->vtbl = D_801528D0;
    i = 0;
    while (1) {
        if (i >= 2) {
            break;
        }
        switch (func_800CA76C(arg, i)) {
            case 0:
                func_800327C0(text, func_80048480(0x4A7), i + 1);
                func_8009C374(&win, text, D_80138D10);
                func_800957C0(&win, &rect, 1, 0, 0);
                break;
            case 1:
            case 3:
                break;
            case 2:
            default:
                func_800327C0(text, func_80048480(0x4A8), i + 1);
                func_8009C374(&win, text, D_80138D20);
                func_800957C0(&win, &rect, 1, 0, 0);
                break;
        }
        i++;
    }
    win.vtbl = D_80151E38;
}
