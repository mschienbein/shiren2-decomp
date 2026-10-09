#include "common.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef float f32;

typedef struct {
    u8 pad0[0xA];
    u8 b0A;
    u8 padB[0x1F - 0xB];
    u8 b1F;
    u8 pad20[0x24 - 0x20];
    void *vtbl;
} Obj800F5A30;

extern u8 D_801598C0[];
extern void *func_800A38FC(s32 size);
extern void *func_800F4760(void *obj);

Obj800F5A30 *func_800F5A30(void) {
    Obj800F5A30 *obj = func_800A38FC(0x2C);

    func_800F4760(obj);
    obj->vtbl = D_801598C0;
    obj->b0A = 0x11;
    obj->b1F = 0x11;
    return obj;
}
