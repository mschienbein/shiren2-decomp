#include "common.h"

typedef unsigned char u8;

typedef struct Object Object;

extern void *func_800A38FC(s32 size);
extern Object *func_800FEE00(Object *arg, unsigned char value);

/* Factory-table entry (D_8015CC64): constructs into mem, allocating 0xA0 bytes when mem is null. */
Object *func_800FEDC0(u8 value, Object *mem)
{
    if (mem != 0) {
        return func_800FEE00(mem, value);
    }
    return func_800FEE00(func_800A38FC(0xA0), value);
}
