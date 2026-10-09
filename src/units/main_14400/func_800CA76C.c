#include "common.h"

typedef unsigned char u8;
typedef struct { char pad0[0x14]; s32 field_14; } Resource;
typedef struct { char pad0[0x1C]; Resource *resource_1C; u8 kind_20; } Object;
extern void *func_80044620(s32 kind, s32 value, s32 *status);
extern void func_800CADFC(Object *obj);
extern void func_800CAA9C(Object *obj);

s32 func_800CA76C(Object *obj, u8 kind) {
    s32 state;
    obj->kind_20 = kind;
    if (kind < 10) {
        obj->resource_1C = func_80044620(1, kind, &state);
    } else {
        obj->resource_1C = func_80044620(2, kind - 10, &state);
    }
    switch (state) {
    /* ODD_C: load status 0 needs no follow-up; the label shapes codegen: without it 22 words differ (148 vs 160 bytes). */
    case 0:
        break;
    case 1:
    case 2:
        func_800CAA9C(obj);
        break;
    case 3:
        func_800CADFC(obj);
        if (obj->resource_1C->field_14 != 0) {
            state = 2;
            func_800CAA9C(obj);
        }
        break;
    }
    obj->resource_1C->field_14 = 0;
    return state;
}
