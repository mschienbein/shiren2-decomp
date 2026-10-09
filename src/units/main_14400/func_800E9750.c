#include "common.h"
typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef struct { s16 delta, index; void *(*get)(void *); } Accessor;
typedef struct { u8 pad00[0x98]; Accessor list98; } Vtable;
typedef struct { u8 pad00[0x24]; Vtable *vtable; } Object;
extern const u16 D_80156A0E;
extern u32 func_800E0EAC(Object *);
extern s32 func_800CF1C8(void *, u8);
/* Entity slot +0x74 override (D_80158E80 branch): u32 (void *self). The body narrows its own
 * result to a halfword (andi 0xFFFF at 0x800E97B4); callers narrow again after the call. */
u32 func_800E9750(Object *object)
{
    u32 total = func_800E0EAC(object);
    void *list = object->vtable->list98.get((u8 *)object + object->vtable->list98.delta);
    if (list != 0) {
        total += func_800CF1C8(list, 0x88) * D_80156A0E;
        if ((u16)total >= 100) {
            total = 99;
        }
    }
    return (u16)total;
}
