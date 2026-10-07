#include "common.h"

typedef struct { short delta; short index; s32 (*fn)(void *, s32, s32, unsigned char, s32); } VEntry;
typedef struct { char pad[0x90]; VEntry e; } VTable;
typedef struct { char pad[0x1E]; unsigned char x1E; char pad2[5]; VTable *vtbl; } Obj;
s32 func_80049CB4(s32 id, ...);
/* Trap slot +0x44: receiver, actor, source position, direction and item are unused. */
s32 func_80124D50(void *a0, void *a1, void *a2, void *a3, void *a4, Obj *obj, void *a6) {
    func_80049CB4(0xF0, a3);
    if (obj != 0 && (obj->x1E & 0x7C)) {
        obj->vtbl->e.fn((char *)obj + obj->vtbl->e.delta, 0, 10, 0xFE, 0);
    }
    return 1;
}
