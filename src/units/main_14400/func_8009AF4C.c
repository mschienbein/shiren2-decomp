#include "common.h"

typedef unsigned char u8;
typedef signed short s16;

typedef struct { s32 x; s32 y; } Pair;

/* Slot 0x80/0x84 targets such as func_80095634(object, const Pair *). */
typedef struct {
    u8 pad0[0x80];
    s16 delta_80;
    s16 index_82;
    void (*set_84)(void *self, const Pair *value);
} VTable;

/* Layout parameters passed to func_800954E0 (initialized D_80138D68). */
typedef struct {
    s32 field00;
    s32 field04;
    s32 field08;
    s32 field0C;
} Params;

typedef struct {
    u8 pad0[0x4C];
    VTable *vtable;
    u8 pad50[0x80 - 0x50];
    s32 mode;
    const u8 *charmap;
} Obj;

extern const u8 D_801526B4[];
extern const u8 D_801526F0[];
extern const u8 D_8015272C[];
extern Params D_80138D68;

u8 *func_8006A810(u8 *dst, s32 value, s32 count);
void func_800954E0(Obj *record, Params *params);

void func_8009AF4C(Obj *obj, s32 mode) {
    Pair origin;

    switch (mode) {
    case 1:
        obj->charmap = D_801526F0;
        break;
    case 0:
        obj->charmap = D_801526B4;
        break;
    case 2:
        obj->charmap = D_8015272C;
        break;
    case 3:
        func_8006A810((u8 *)&origin, 0, sizeof(origin));
        obj->vtable->set_84((u8 *)obj + obj->vtable->delta_80, &origin);
        break;
    }
    if (mode == 2) {
        if (obj->mode != mode) {
            Params params = D_80138D68;
            params.field00 -= 2;
            func_800954E0(obj, &params);
        }
    } else if (obj->mode == 2) {
        func_800954E0(obj, &D_80138D68);
    }
    obj->mode = mode;
}
