#include "common.h"

typedef unsigned char u8;

extern void *func_800A8CB0(s32 cell);
extern s32 func_800A6E90(void *obj);

/* The caller func_8008AA20 passes its loop index without narrowing (move a0,s1 at
 * 0x8008AC70/0x8008ADE4), so the parameter is a full int narrowed here (andi 0xFF). */
s32 func_80042A2C(s32 id) {
    return func_800A6E90(func_800A8CB0((u8)id));
}
