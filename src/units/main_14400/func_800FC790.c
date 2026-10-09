#include "common.h"

typedef struct VTable VTable;

typedef struct {
    char pad0[0x24];
    VTable *vtbl_24;
    char pad28[0xA0 - 0x28];
} Obj_800FC790;

extern VTable D_8015A530;
extern void *func_800A38FC(s32 size);
extern Obj_800FC790 *func_800EFC70(Obj_800FC790 *obj, s32 arg1, unsigned char arg2);

/* Factory-table entry: construct in the supplied storage, or in a fresh 0xA0-byte block. */
Obj_800FC790 *func_800FC790(unsigned char kind, Obj_800FC790 *storage) {
    if (storage == 0) {
        storage = func_800A38FC(sizeof(Obj_800FC790));
    }
    func_800EFC70(storage, 0x27, kind);
    storage->vtbl_24 = &D_8015A530;
    return storage;
}
