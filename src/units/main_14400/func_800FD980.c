#include "common.h"

typedef unsigned char u8;

typedef struct {
    char pad0[0xA0];
} Obj_800FD9C0;

void *func_800A38FC(s32 size);
Obj_800FD9C0 *func_800FD9C0(Obj_800FD9C0 *obj, u8 id);

/* Factory slot of D_8015CC64: construct in caller memory or in a fresh 0xA0-byte block. */
Obj_800FD9C0 *func_800FD980(u8 id, Obj_800FD9C0 *mem) {
    if (mem != 0) {
        return func_800FD9C0(mem, id);
    }
    return func_800FD9C0(func_800A38FC(sizeof(Obj_800FD9C0)), id);
}
