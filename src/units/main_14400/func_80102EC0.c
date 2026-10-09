#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;

typedef struct Object80102F00 Object80102F00;

extern void *func_800A38FC(s32 size);
extern Object80102F00 *func_80102F00(Object80102F00 *object, u8 value);

/* Factory slot of D_8015CC64: construct in caller storage or in a new 0xA8-byte block. */
Object80102F00 *func_80102EC0(u8 value, Object80102F00 *storage)
{
    if (storage != 0) {
        return func_80102F00(storage, value);
    } else {
        return func_80102F00(func_800A38FC(0xA8), value);
    }
}
