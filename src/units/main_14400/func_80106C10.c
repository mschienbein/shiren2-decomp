#include "common.h"

typedef unsigned char u8;

typedef struct VTable VTable;

typedef struct {
    char pad00[0x24];
    const void *vtable24;
} Obj80106C10;

extern void *func_800A38FC(s32 size);
extern Obj80106C10 *func_800EFC70(Obj80106C10 *obj, s32 arg1, u8 arg2);
extern VTable D_80149BA8;

/* Factory slot of D_8015CC64 (void *(u8 variant, void *mem)): kind 0x4E actor built in
 * caller memory or a fresh 0xA0-byte block. */
void *func_80106C10(u8 variant, void *mem) {
    if (mem == 0) {
        mem = func_800A38FC(0xA0);
    }
    func_800EFC70(mem, 0x4E, variant);
    ((Obj80106C10 *)mem)->vtable24 = &D_80149BA8;
    return mem;
}
