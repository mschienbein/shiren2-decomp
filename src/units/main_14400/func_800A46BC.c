#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 value; } Dir;
typedef struct { s32 a; s32 b; } Tmp800A46BC;
typedef struct { u8 pad0[0x1D]; u8 flags_1D; } Obj800A46BC;
void *func_800A2594(Tmp800A46BC *out, void *arg, Dir cell);
u32 func_800B1C6C(Tmp800A46BC *tmp);
s32 func_800B502C(Tmp800A46BC *tmp);
s32 func_800A4754(Obj800A46BC *obj, void *arg, Dir *cell);

s32 func_800A46BC(Obj800A46BC *obj, void *arg, Dir *cell) {
    Tmp800A46BC tmp;
    Tmp800A46BC *t;

    func_800A2594(&tmp, arg, *cell);
    t = &tmp;
    if (func_800B1C6C(t) & 0x8000) {
        return 0;
    }
    if ((obj->flags_1D >> 7) && func_800B502C(t)) {
        return 0;
    }
    return func_800A4754(obj, arg, cell);
}
