#include "common.h"

typedef unsigned char u8;

extern void *func_800A38FC(s32 size);
/* Constructor: initializes the 0xA0-byte object with the kind and returns it. */
extern void *func_801002E0(void *obj, u8 kind);

/* Factory slot of D_8015CC64 (dispatched by func_800A8694): construct in place when
 * storage is supplied, otherwise allocate 0xA0 bytes first. */
void *func_801002A0(u8 kind, void *storage) {
    if (storage != 0) {
        return func_801002E0(storage, kind);
    }
    return func_801002E0(func_800A38FC(0xA0), kind);
}
