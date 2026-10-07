#include "common.h"

typedef unsigned char u8;
typedef short s16;

typedef struct Obj Obj;
typedef struct { s16 delta; s16 index; s32 (*func)(Obj *, s32, s32, u8, s32); } VtblEntry;
typedef struct { u8 pad0[0x90]; VtblEntry entry90; } Vtbl;
struct Obj { u8 pad0[0x1E]; u8 flags; u8 pad1F[5]; Vtbl *vtbl; };
s32 func_80049CB4(s32 id, ...);
/* Trap slot +0x44 supplies seven pointers; only the sixth (target unit) is used. */
s32 func_80124C50(void *arg0, void *arg1, void *arg2, void *arg3, void *arg4, Obj *obj, void *arg6) {
    if (obj != 0 && (obj->flags & 0x7C)) {
        func_80049CB4(0x1E, obj);
        obj->vtbl->entry90.func((Obj *)((u8 *)obj + obj->vtbl->entry90.delta), 0, 0x13, 0xFE, 0);
    }
    return 1;
}
