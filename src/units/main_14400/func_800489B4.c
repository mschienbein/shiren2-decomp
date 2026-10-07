#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0xC];
    s32 valueC;
} Obj800489B4;

extern void func_800838E0(s32 index, s32 value);

void func_800489B4(Obj800489B4 *obj, s32 value) {
    s32 index = obj->valueC;

    if (index >= 0) {
        func_800838E0(index, value);
    }
}
