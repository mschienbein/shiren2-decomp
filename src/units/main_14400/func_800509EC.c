#include "common.h"

typedef struct { s32 a, b; } Pair;
typedef struct { s32 x0; s32 x4; } Src;
typedef struct { char pad[0x5C]; s32 x5C; char pad2[8]; s32 x68; } Obj;
extern s32 D_8013968C;
Obj *func_800851B0(s32 id);

/* The incoming pointer variable is reused for the spawned object (register shape). */
void func_800509EC(Src *src) {
    Pair t;
    s32 id;
    if (src == 0) {
        t.b = 0;
        t.a = 0;
    } else {
        t.a = src->x4;
        t.b = src->x0;
    }
    switch (D_8013968C) {
    case 0x124: id = 0x103; break;
    case 0x125: id = 0xE1; break;
    default: return;
    }
    src = (Src *)func_800851B0(id);
    ((Obj *)src)->x5C = t.a;
    ((Obj *)src)->x68 = t.b;
}
