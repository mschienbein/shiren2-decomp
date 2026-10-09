#include "common.h"

typedef unsigned char u8;

typedef struct {
    char pad00[0x24];
    void *vtable;
} Obj800EFC70;

extern void *func_800A38FC(s32 size);
extern Obj800EFC70 *func_800EFC70(Obj800EFC70 *obj, s32 arg1, u8 arg2);
extern char D_80149810[];

/* Factory slot 0 of D_8015CC64 (dispatched by func_800A8694): construct kind 0x1D
 * in place when storage is supplied, otherwise allocate 0xA0 bytes. */
void *func_800F8E10(u8 kind, Obj800EFC70 *storage) {
    if (storage == 0) {
        storage = func_800A38FC(0xA0);
    }
    func_800EFC70(storage, 0x1D, kind);
    storage->vtable = D_80149810;
    return storage;
}
