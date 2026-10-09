#include "common.h"
typedef struct { s32 x; s32 y; } Pair;
typedef struct { unsigned char pad0[0x80]; short adjust_80; short pad82; void (*set_84)(void *, Pair *); } Methods;
typedef struct { unsigned char pad0[0x20]; s32 field_20; s32 field_24; unsigned char pad28[0xC]; Pair field_34; unsigned char pad3C[0x10]; Methods *field_4C; } Obj;
extern void func_80045A24(s32 sound);
void func_80097498(Obj *obj, s32 direction) {
    Pair pos = obj->field_34;
    switch (direction) {
    case 0: if (pos.x < obj->field_20 - 1) pos.x++; break;
    case 1: if (pos.x > 0) pos.x--; break;
    case 2: if (pos.y < obj->field_24 - 1) pos.y++; break;
    case 3: if (pos.y > 0) pos.y--; break;
    }
    if (pos.y != obj->field_34.y || pos.x != obj->field_34.x) {
        obj->field_4C->set_84((unsigned char *)obj + obj->field_4C->adjust_80, &pos);
        func_80045A24(2);
    }
}
