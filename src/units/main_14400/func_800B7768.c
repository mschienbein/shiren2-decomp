#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef signed char s8;
typedef short s16;

typedef struct { u8 pad0[0x18]; s16 offset_18; s16 pad1A; void (*fn_1C)(void *self, s32 arg, void *data); } VTable800B7768;
typedef struct { u8 pad0[0x18]; VTable800B7768 *vtable_18; } Obj800B7768;
extern u8 D_80153B84[];
extern u8 D_80147490[];
void func_800CA4A4(Obj800B7768 *obj, void *data);

void func_800B7768(Obj800B7768 *obj) {
    VTable800B7768 *vt;

    func_800CA4A4(obj, D_80153B84);
    vt = obj->vtable_18;
    vt->fn_1C((u8 *)obj + vt->offset_18, 1, D_80147490);
}
