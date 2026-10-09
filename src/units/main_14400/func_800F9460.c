#include "common.h"

typedef unsigned char u8;

typedef struct Obj800F94A0 Obj800F94A0;

void *func_800A38FC(s32 size);
Obj800F94A0 *func_800F94A0(Obj800F94A0 *obj, u8 kind);

/* Factory slot 2 of D_8015CC64: construct in place when storage is supplied,
 * otherwise allocate 0xBC bytes. */
Obj800F94A0 *func_800F9460(u8 kind, Obj800F94A0 *storage)
{
    if (storage != 0) {
        return func_800F94A0(storage, kind);
    }
    return func_800F94A0(func_800A38FC(0xBC), kind);
}
