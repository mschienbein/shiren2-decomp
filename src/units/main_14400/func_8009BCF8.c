#include "common.h"
typedef short s16;
typedef unsigned char u8;
typedef struct { s32 kind; s32 code; } Input;
typedef struct { s32 unk0; s32 value; } Msg;
/* Slots +0x24/+0x2C take only self; +0x84 also takes a cursor position. */
typedef struct {
    s16 delta;
    s16 index;
    union {
        void (*self)(void *);
        void (*position)(void *, Msg *);
    } fn;
} VEntry;
typedef struct {
    char pad0[0x4C];
    VEntry *vtbl;
    char pad50[0x70 - 0x50];
    u8 text[8];         /* 0x70 */
    s32 cursor;         /* 0x78 */
    s32 length;         /* 0x7C */
    s32 mode;           /* 0x80 */
    u8 *table;          /* 0x84 */
    s32 unk88;          /* 0x88 */
} Menu;
extern Input D_801527E4;
extern Input D_801527EC;
extern s32 D_801528B8;
s32 func_8009BF80(Menu *, s32);
void func_80045A24(s32);
u8 *func_8006A810(void *, s32, s32);
void func_8009AF4C(Menu *, s32);
void func_80048764(Menu *);
s32 func_8009C0E4(void *);
/* g++ 2.x vtable call: entry n holds {this delta, index, function} */
#define VTHIS(obj, n) ((char *)(obj) + (obj)->vtbl[n].delta)
s32 func_8009BCF8(Menu *menu, Input *in) {
    s32 handled;
    s32 dir;
    u8 c;
    Msg msg;
    if (in->kind != 0) {
        handled = 0;
        if (menu->mode != 2) {
            dir = handled;
            if (in->kind == D_801527E4.kind && in->code == D_801527E4.code) {
                dir = 1;
            } else if (in->kind == D_801527EC.kind && in->code == D_801527EC.code) {
                dir = 2;
            }
            if (dir != 0) {
                if (func_8009BF80(menu, dir) != 0) {
                    func_80045A24(0);
                    if (menu->text[menu->length - 1] != 0) {
                        func_8006A810(&msg, 0, sizeof(msg));
                        msg.value = D_801528B8;
                        menu->vtbl[16].fn.position(VTHIS(menu, 16), &msg);
                    }
                }
                handled = 1;
            }
        }
        if (!handled) {
            c = menu->table[(in->kind - 1) * 10 + in->code];
            if (c != 0xCA) {
                func_80045A24(0);
                menu->text[menu->cursor] = c;
                if (menu->cursor < menu->length - 1) {
                    menu->cursor++;
                } else {
                    func_8006A810(&msg, 0, sizeof(msg));
                    msg.value = D_801528B8;
                    menu->vtbl[16].fn.position(VTHIS(menu, 16), &msg);
                }
            }
        }
    } else {
        func_80045A24(0);
        switch (in->code) {
            case 0:
                if (menu->mode == 0) {
                    func_8009AF4C(menu, 1);
                } else if (menu->mode == 1) {
                    func_8009AF4C(menu, 2);
                } else {
                    func_8009AF4C(menu, 0);
                }
                menu->vtbl[4].fn.self(VTHIS(menu, 4));
                break;
            case 3:
                if (menu->unk88 != 0) {
                    func_8009AF4C(menu, 3);
                    func_80048764(menu);
                    menu->vtbl[4].fn.self(VTHIS(menu, 4));
                }
                break;
            case 5:
                func_8009C0E4(menu);
                break;
            case 8:
                return 0;
        }
    }
    menu->vtbl[5].fn.self(VTHIS(menu, 5));
    return 1;
}
