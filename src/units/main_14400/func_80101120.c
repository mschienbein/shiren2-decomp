#include "common.h"

typedef unsigned char u8;

typedef struct {
    char pad0[0xA0];
} Obj_80101160;

void *func_800A38FC(s32 size);
Obj_80101160 *func_80101160(Obj_80101160 *obj, u8 id);

/* Factory slot of D_8015CC64: construct in caller memory or a fresh 0xA0-byte block. */
Obj_80101160 *func_80101120(u8 id, Obj_80101160 *mem) {
    if (mem != 0) {
        return func_80101160(mem, id);
    }
    return func_80101160(func_800A38FC(sizeof(Obj_80101160)), id);
}
