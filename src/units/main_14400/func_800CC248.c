#include "common.h"

typedef short s16;

/* Port vtables D_8014A840 / D_8014A8E8 / D_801541F8: entry 2 (+0x10/+0x14) is the deleting
 * destructor (func_80044274 / func_800445B4 / func_800CA6D0: self, s32 flags); earlier
 * entries are not used here. */
typedef struct {
    char pad00[0x10];
    struct {
        s16 delta;
        s16 index;
        void (*fn)(void *self, s32 flags);
    } destroy;
} PortVtable;

typedef struct {
    char pad00[0x18];
    PortVtable *vtable;
} Port;

extern Port *D_80147F44;

void func_800CC248(void) {
    if (D_80147F44 != 0) {
        D_80147F44->vtable->destroy.fn((char *)D_80147F44 + D_80147F44->vtable->destroy.delta, 3);
    }
    D_80147F44 = 0;
}
