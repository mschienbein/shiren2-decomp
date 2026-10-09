#include "common.h"

typedef unsigned char u8;

typedef struct VTable VTable;

typedef struct {
    char pad00[0x24];
    const void *vtable24;
} Obj80107760;

extern void *func_800A38FC(s32 size);
extern Obj80107760 *func_800EFC70(Obj80107760 *obj, s32 arg1, u8 arg2);
extern const VTable D_80149C60;

/* Factory slot of D_8015CC64 (void *(u8 variant, void *mem)): kind 0x52 actor built in
 * caller memory or a fresh 0xA0-byte block. */
void *func_80107760(u8 variant, void *mem) {
    if (mem == 0) {
        mem = func_800A38FC(0xA0);
    }
    func_800EFC70(mem, 0x52, variant);
    ((Obj80107760 *)mem)->vtable24 = &D_80149C60;
    return mem;
}
