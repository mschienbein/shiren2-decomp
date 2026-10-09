#include "common.h"

typedef unsigned char u8;

typedef struct {
    u8 pad0[0x24];
    const void *vtable_24;
    u8 pad28[0xA0 - 0x28];
} Obj800FB750;

/* Initialized original vtable; its full type is unresolved. */
extern const unsigned char D_8015A2B0[192];

void *func_800A38FC(s32 size);
Obj800FB750 *func_800EFC70(Obj800FB750 *obj, s32 arg1, u8 arg2);

/* Factory slot of D_8015CC64: construct in caller memory or a fresh 0xA0-byte block. */
Obj800FB750 *func_800FB750(u8 id, Obj800FB750 *mem) {
    if (mem == 0) {
        mem = func_800A38FC(sizeof(Obj800FB750));
    }
    func_800EFC70(mem, 0x23, id);
    mem->vtable_24 = D_8015A2B0;
    return mem;
}
