#include "common.h"

typedef unsigned char u8;

/* Partial view: flag byte at +0x72, bit 0 = announced once. */
typedef struct {
    char pad0[0x72];
    u8 field_72;
} Obj;

extern s32 func_80049CB4(s32 id, ...);

void func_800E2124(Obj *obj) {
    u8 first = (obj->field_72 & 1) ^ 1;

    if (first) {
        obj->field_72 |= 1;
        func_80049CB4(0x1F, obj);
    }
}
