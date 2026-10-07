#include "common.h"

typedef unsigned char u8;

typedef short s16;
typedef struct {
    s16 delta;
    s16 index;
    void *func;
} VTableEntry;

typedef struct {
    u8 pad0[0xC];
    u32 valueC;
} Result800F9504;

typedef struct {
    s32 field0;
    VTableEntry *vtable4;
} Inner800F9504;

typedef struct {
    u8 pad0[0x8C];
    Inner800F9504 *inner8C;
} Obj800F9504;

u32 func_800F9504(Obj800F9504 *obj) {
    Inner800F9504 *inner = obj->inner8C;
    VTableEntry *entry = &inner->vtable4[4];

    if (((s32 (*)(void *))entry->func)((u8 *)inner + entry->delta) == 0) {
        return 0;
    }
    {
        VTableEntry *vt = obj->inner8C->vtable4;
        return ((Result800F9504 *(*)(void *, u32))vt[7].func)((u8 *)obj->inner8C + vt[7].delta, 0)->valueC;
    }
}
