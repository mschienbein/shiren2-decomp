#include "common.h"

typedef unsigned char u8;

typedef struct {
    char pad0[0xA];
    u8 unkA;
} Obj80041CD0;

extern void *func_800A8CB0(s32 cell);
extern s32 func_800A8974(Obj80041CD0 *obj);

u8 func_80041CD0(u8 id) {
    Obj80041CD0 *obj = func_800A8CB0(id);

    if (func_800A8974(obj) != 0) {
        return 0;
    }
    return obj->unkA;
}
