#include "common.h"
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;

typedef struct { s32 cur; s32 rep; } Repeat;
typedef struct { Repeat menu; Repeat shoulder; Repeat cbtn; } PadRepeat;
typedef struct { u8 pad0[0xA]; u16 buttons; u8 padC[0xA4]; } Ctrl;
extern PadRepeat D_801A7320[];
extern Ctrl D_801DEAB4[];
extern u32 D_8013D890[];
extern u32 D_8013D8B4[];
extern u32 D_8013D8BC[];
void func_80074F34(s32 pad){
    Ctrl *c = &D_801DEAB4[pad];
    PadRepeat *p = &D_801A7320[pad];
    s32 i;
    s32 count;
    if ((p->cbtn.rep & 0xF) == 0) {
        if (!(c->buttons & 0x10)) {
            p->cbtn.cur = -1;
            p->cbtn.rep = 0;
        } else {
            for (i = 0; i < 1; i++) {
                s32 next = p->cbtn.cur + 1;
                next = (next < 1) ? next : 0;
                p->cbtn.cur = next;
                if (c->buttons & D_8013D8BC[next]) break;
            }
        }
    }
    count = 0;
    for (i = 0; i < 1; i++) {
        if (c->buttons & D_8013D8BC[i]) count++;
    }
    if (count >= 2) p->cbtn.rep++;
    else p->cbtn.rep = 0;
    if ((p->shoulder.rep & 0xF) == 0) {
        if (!(c->buttons & 0x60)) {
            p->shoulder.cur = -1;
            p->shoulder.rep = 0;
        } else {
            for (i = 0; i < 2; i++) {
                s32 t = p->shoulder.cur + 1;
                s32 next = 0;
                if (t < 2) next = t;
                p->shoulder.cur = next;
                if (c->buttons & D_8013D8B4[next]) break;
            }
        }
    }
    count = 0;
    for (i = 0; i < 2; i++) {
        if (c->buttons & D_8013D8B4[i]) count++;
    }
    if (count >= 2) p->shoulder.rep++;
    else p->shoulder.rep = 0;
    if ((p->menu.rep & 0xF) == 0) {
        if (!(c->buttons & 0xF8F)) {
            p->menu.cur = -1;
            p->menu.rep = 0;
        } else {
            for (i = 0; i < 9; i++) {
                s32 t = p->menu.cur + 1;
                s32 next = 0;
                if (t < 9) next = t;
                p->menu.cur = next;
                if (c->buttons & D_8013D890[next]) break;
            }
        }
    }
    count = 0;
    for (i = 0; i < 9; i++) {
        if (c->buttons & D_8013D890[i]) count++;
    }
    if (count >= 2) p->menu.rep++;
    else p->menu.rep = 0;
}
