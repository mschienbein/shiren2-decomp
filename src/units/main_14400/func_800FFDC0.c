#include "common.h"

typedef unsigned char u8;

void *func_800A38FC(s32 size);
/* Constructor: initializes the 0xA0-byte object with the kind and returns it. */
void *func_800FFE00(void *obj, u8 kind);

/* Factory slot of D_8015CC64 (dispatched by func_800A8694): construct in place when
 * storage is supplied, otherwise allocate it. */
void *func_800FFDC0(u8 kind, void *storage)
{
    if (storage != 0) {
        return func_800FFE00(storage, kind);
    }
    return func_800FFE00(func_800A38FC(0xA0), kind);
}
