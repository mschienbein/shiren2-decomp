#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x18]; s16 delta; s16 pad1A; void (*fn)(void *, s32, void *); } Vtbl800E95B8;
typedef struct { u8 pad0[0x18]; Vtbl800E95B8 *vtable; } Target800E95B8;
extern u8 D_80158DF8[];
void func_800E02D0(void *obj, Target800E95B8 *target);
void func_800CA4A4(Target800E95B8 *target, void *data);
void func_800E95B8(u8 *obj, Target800E95B8 *target) {
    Vtbl800E95B8 *vt;
    func_800E02D0(obj, target);
    func_800CA4A4(target, D_80158DF8);
    vt = target->vtable;
    vt->fn((u8 *)target + vt->delta, 8, obj + 0x78);
}
