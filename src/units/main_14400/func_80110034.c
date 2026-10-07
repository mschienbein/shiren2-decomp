#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef struct { char pad0; u8 kind; char pad2[0xC]; u8 unkE; u8 unkF; } Obj;
typedef struct { u8 a; u8 b; } Pair;
extern Pair D_8015D3F8[];
s32 func_8010B9F4(void *s);
s32 func_8010BC2C(void *obj, u8 index);
u8 func_80110034(Obj *obj) {
    if (obj->kind == 0x34) {
        s32 ok = 0;
        if ((s16)func_8010B9F4(obj) >= 0x63) {
            ok = (s8)(obj->unkE - obj->unkF) == 0;
        }
        if (ok) {
            return 0x54;
        }
    } else {
        s32 ready = 0;
        if (obj->kind == 0x36) {
            ready = (s8)obj->unkF == 5;
        }
        if (ready) {
            s32 all = 1;
            s32 i = 0;
            for (;;) {
                u8 r;
                if (i >= 5) {
                    break;
                }
                r = (u8)func_8010BC2C(obj, i);
                if (r != D_8015D3F8[i].a && r != D_8015D3F8[i].b) {
                    all = 0;
                    break;
                }
                i++;
            }
            if (all) {
                return 0x55;
            }
        }
    }
    return obj->kind;
}
