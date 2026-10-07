#include "common.h"

typedef struct {
    char pad0[0xC];
    void *unkC;
} Entry8008F6D4;

typedef struct {
    char pad0[0x1C];
    void *unk1C;
    u32 count;
    Entry8008F6D4 *entries;
} Obj8008F6D4;

void func_80091544(void *ptr);

void func_8008F6D4(Obj8008F6D4 *obj) {
    u32 i;

    if (obj->unk1C != 0) {
        func_80091544(obj->unk1C);
        obj->unk1C = 0;
    }
    if (obj->entries != 0) {
        for (i = 0; i < obj->count; i++) {
            if (obj->entries[i].unkC != 0) {
                func_80091544(obj->entries[i].unkC);
            }
        }
        func_80091544(obj->entries);
        obj->entries = 0;
    }
}
