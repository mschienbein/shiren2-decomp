#include "common.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef short s16;
typedef signed char s8;

typedef struct { u8 pad0[0x1C]; u16 flags; } Obj800429E0;
void *func_800A8CB0(s32 cell);
/* The only caller (func_80076F48) passes its loop index un-narrowed (daddu
 * a0,s5 at 0x800772C4); the callee narrows it (andi a0,0xFF). */
s32 func_800429E0(s32 id) {
    Obj800429E0 *obj = func_800A8CB0((u8)id);
    if (obj->flags & 0x40) return 1;
    return 0;
}
