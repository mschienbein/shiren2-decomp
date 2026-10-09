#include "common.h"

typedef unsigned char u8;

typedef struct VTable VTable;

typedef struct {
    char pad00[0x24];
    const void *vtable24;
} Obj80106880;

extern void *func_800A38FC(s32 size);
extern Obj80106880 *func_800EFC70(Obj80106880 *obj, s32 arg1, u8 arg2);
extern const unsigned char D_8015C080[200];

/* Factory slot of D_8015CC64 (void *(u8 variant, void *mem)): kind 0x4D actor built in
 * caller memory or a fresh 0xA0-byte block. */
void *func_80106880(u8 variant, void *mem) {
    if (mem == 0) {
        mem = func_800A38FC(0xA0);
    }
    func_800EFC70(mem, 0x4D, variant);
    ((Obj80106880 *)mem)->vtable24 = D_8015C080;
    return mem;
}
